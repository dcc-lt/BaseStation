
#include <unity.h>
#include <RefCount.h>

using namespace jtv::std;

void setUp() {}
void tearDown() {}

void test_RefCount()
{
    RefCount r;
    TEST_ASSERT_EQUAL(1, r.increment());
    TEST_ASSERT_EQUAL(2, r.increment());
    TEST_ASSERT_EQUAL(1, r.decrement());
    TEST_ASSERT_EQUAL(0, r.decrement());
}
