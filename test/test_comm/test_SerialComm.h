
#include <unity.h>
#include <Godmode.h>

#include "comm/SerialComm.h"


using namespace dcclt::comm;


void test_SerialComm()
{
    // Test data
    const unsigned long rate = 9600;

    #ifndef HAVE_HWSERIAL2
        CommInterface* s = new SerialComm(Serial, rate);
        const int port = 0;
    #else
        CommInterface* s = new SerialComm(Serial2, rate);
        const int port = 2;
    #endif
    
    // Test state setup
    GodmodeState* state = GODMODE();
    state->reset();
    state->resetClock();
    state->resetPins();
    state->serialPort[port].dataIn = "";
    state->serialPort[port].dataOut = "";

    // Test initial state of serial communication
    s->begin();
    TEST_ASSERT_FALSE(s->available());

    // Test reading data from Serial
    state->serialPort[port].dataIn = "a";
    while(s->available())
    {
        char r = s->read();
        TEST_ASSERT_EQUAL('a', r);
    }
    TEST_ASSERT_FALSE(s->available());

    // Test writing data to Serial
    s->write('<');
    TEST_ASSERT_EQUAL(String("<"), String(state->serialPort[port].dataOut));

    delete s;
}
