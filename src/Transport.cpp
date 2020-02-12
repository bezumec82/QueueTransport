#include "Transport.hpp"

using namespace Transport;

/*!
 * \brief post - threadsafe method to pass data to the input queue;
 *               after data will be added, waiting thread will be waked up.
 * \param data - any data what queue is holding
 */
void QueueTransport::post ( const Any & data )
{
    {
        ::std::lock_guard < ::std::mutex > lock(m_mutex);
        m_queue.emplace(data);
    }
    #if DEBUG_TRANSPORT
        PRINTF(MAG, "Awakening waiters.\n");
    #endif
    m_cond_var.notify_one();
}

void QueueTransport::post ( Any && data )
{
    {
        ::std::lock_guard < ::std::mutex > lock(m_mutex);
        m_queue.emplace(::std::move(data) );
    }
    #if DEBUG_TRANSPORT
        PRINTF(MAG, "Awakening waiters.\n");
    #endif
    m_cond_var.notify_one();
}

/*!
 * \brief getBlock - blocking mechanism to read queue
 * \return - data that is queue holding
 */
Any QueueTransport::getBlock ( Milliseconds time_out )
{
    // Error usage - timeout is not set
    if ( time_out == 0 ) return this->getBlock(); // I expect copy ellision here.
    while ( !m_stop )
    {
        ::std::unique_lock < ::std::mutex > uniLock(m_mutex);
        if( m_cond_var.wait_for  (
                                uniLock,
                                ::std::chrono::milliseconds(time_out),
                                // 'predicate' returns '​false' if the waiting should be continued.
                                [&]{ return !m_queue.empty(); }
                                )==true )
        {
            #if DEBUG_TRANSPORT
                PRINTF(GRN, "Awakened!\n");
            #endif
            break; // All good - continue execution.
        }
        else
        {
            #if DEBUG_TRANSPORT
                PRINTF(YEL, "Timeout!\n");
            #endif
            if (m_call_back_ptr != nullptr) { m_call_back_ptr(); }
            continue;
        } //end if( m_cond_var.wait_for ...
    } //end while
    return this->get();
}

Any QueueTransport::getBlock ( void )
{
    {
        ::std::unique_lock < ::std::mutex > uniLock(m_mutex);
        m_cond_var.wait  (
                        uniLock,
                        /* 'predicate' returns '​false' if the waiting should be continued.
                         * Queue is not empty or transport is stopped. */
                        [&]{ return !m_queue.empty()| m_stop; }
                        );
    } //Destroy 'uniLock' just in case.
    #if DEBUG_TRANSPORT
        PRINTF(YEL, "Timeout!\n");
    #endif
    return this->get();
}

/*!
 * \brief get - simple mechanism to get data from queue.
 *              Aquire mutex -> get data -> pop queue.
 */
std::any QueueTransport::get ( void )
{
    Any ret_val;
    if ( !m_stop )
    {// Take element from the queue.
        ::std::lock_guard < std::mutex > lock(m_mutex);
        #if DEBUG_TRANSPORT
            PRINTF(YEL, "Get front from queue.\n");
        #endif
        try
        {
            ret_val = m_queue.front(); // Get the  first element.
            m_queue.pop();             // Destroy the first element.
        }
        catch ( const ::std::exception& e )
        {
            ::std::cout << e.what();
        }
    }
    else
    {
        #if DEBUG_TRANSPORT
            PRINTF(RED, "Transport is stopped.\n");
        #endif
        ret_val.reset(); // Explicitly empty.
    }
    return ret_val;
}


