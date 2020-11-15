
#include <unity.h>

#ifdef PLATFORM_NATIVE
#include "./test_SerialComm.h"
#endif


void setup() 
{
    UNITY_BEGIN();
}

void loop() 
{ 
    // --- SerialComm ---
#ifdef PLATFORM_NATIVE
    RUN_TEST(test_SerialComm);
#endif
    // ---
    UNITY_END();

}

int main() 
{
   setup();
   loop(); 
   return 0;
}