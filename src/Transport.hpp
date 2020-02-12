#ifndef TRANSPORT_HPP
#define TRANSPORT_HPP

#include <list>
#include <any>
#include <map>
#include <utility>
#include <queue>
#include <thread>
#include <condition_variable>
#include <iostream>

#include "Tools.h"

namespace Transport
{
    using TimeoutCallBackFunc = void ();
    using Milliseconds        = unsigned int;
    using Any                 = ::Types::Any;

    /*!
     * \brief The transport class
     * This class should provide interchangeble abstraction
     * for data exchange between different processes.
     */
    class QueueTransport
    {
    private:
        ::std::queue < Any >      m_queue;
        ::std::mutex              m_mutex;
        ::std::condition_variable m_cond_var;
        /// To stop wait for the data arrival.
        bool                      m_stop = false;
        TimeoutCallBackFunc *     m_call_back_ptr = nullptr;

    public:
        void post ( const Any & );
        void post ( Any && );

        void stopWait ( void )
        {
            // Look 'get*' methods for the implementation.
            m_stop = true;
            // False notification - receivers should check received data.
            m_cond_var.notify_all();
        }

        void setTimeOutCallBack ( TimeoutCallBackFunc * ptr )
        {
            m_call_back_ptr = ptr;
        }

        Any get         ( void );
        Any getBlock    ( Milliseconds );
        Any getBlock    ( void );
        Any getNonBlock ( void ); //TO DO
    }; // end class
} // end namespace

#endif /* TRANSPORT_HPP */
