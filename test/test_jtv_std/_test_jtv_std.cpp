#ifdef UNIT_TEST

#include <unity.h>

#include "./test_RefCount.h"
#include "./test_AutoPtr.h"


void setup() 
{
    UNITY_BEGIN();
}
void loop() 
{ 
    // --- RefCount ---
    RUN_TEST(test_RefCount);

    // --- AutoPtr ---
    RUN_TEST(test_AutoPtr_ctor_nullptr);
    RUN_TEST(test_AutoPtr_ctor_default);
    RUN_TEST(test_AutoPtr_ctor_copy);
    RUN_TEST(test_AutoPtr_ctor_cast);
    RUN_TEST(test_AutoPtr_assign_op);
    RUN_TEST(test_AutoPtr_deref_op);
    RUN_TEST(test_AutoPtr_arrow_op);
    RUN_TEST(test_AutoPtr_bool_op);
    RUN_TEST(test_AutoPtr_equal_op);
    RUN_TEST(test_AutoPtr_equal_op_ptr);
    RUN_TEST(test_AutoPtr_equal_op_other);
    RUN_TEST(test_AutoPtr_as);
    RUN_TEST(test_AutoPtr_release);
    RUN_TEST(test_AutoPtr_fct_ref);
    RUN_TEST(test_AutoPtr_fct_copy);
    // ---
    UNITY_END();

}
int main() 
{
   setup();
   loop(); 
}

#endif