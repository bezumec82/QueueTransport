#include <thread>
#include <any>
#include <iostream>
#include <functional>

#include "Transport.hpp"
#include "Debug.hpp"

#define CYCLES  4

Transport::QueueTransport transport;

void writer (void)
{

    for( int i = 0; i < CYCLES; i++ )
    {
        PRINTF(CYN, "Push to queue # %i.\n", i);
    #if (0)
        std::any a( std::string("some data") );
        transport.post(a);
    #else
        transport.post(std::make_any<std::string>(std::string("some data")));
    #endif
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    PRINTF(RED, "Stop the transport.\n");
    transport.stopWait();
}

void readAny (std::any& a)
{
    /* !!! Carefully cast 'any' !!! */
    try
    {
        std::cout << a.type().name() << " : " << std::any_cast < std::string > (a) << '\n';
        std::cout << "EXCEPTION"     << " : " << std::any_cast < int > (a)         << '\n';
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
}

void reader (void)
{
    PRINTF(GRN, "Start non - blocking read thread.\n");
    while (true)
    {
        std::any a = transport.getBlock(100);
        if (!a.has_value())
        {
            break;
        }
        readAny(a);
    }
}

void blockReader (void)
{
    PRINTF(GRN, "Start blocking read thread.\n");
    while (true)
    {
        std::any a = transport.getBlock();
        if (!a.has_value())
        {
            PRINTF(RED, "'any' is empty.\n");
            break;
        }
        /* !!! Carefully cast 'any' !!! */
        try
        {
            PRINTF(GRN, "Received : %s.\n", std::any_cast < std::string >(a).c_str() );
        }
        catch (const std::exception& e)
        {
            std::cout << e.what() << '\n';
        }
    }
}


void timeOutCallBack (void)
{
    PRINTF(RED, "Wait timeout callBack is called.\n");
}

#define TEST_BLOCKING_READ false

int main()
{
    PRINTF(GRN, "Starting transport test.\n");
    transport.setTimeOutCallBack(timeOutCallBack);
    std::thread wt(writer);

    #if TEST_BLOCKING_READ
        std::thread brt(blockReader);
    #else
        std::thread rt(reader);
    #endif
 
    wt.join();
    #if TEST_BLOCKING_READ
        brt.join();
    #else
        rt.join();
    #endif
    PRINTF(GRN, "Test finished.\n");
}

