
#include <unity.h>
#include <AutoPtr.h>


using namespace jtv::std;


// Simple struct for testing
class Rectangle
{
public:

    Rectangle(int w, int h) : width(w), height(h)
    { ++count; }

    virtual ~Rectangle()
    { --count; }

    int width;
    int height;

    // Static bool to check pointer status
    static size_t count;
};

class Trapezoid : public Rectangle
{
public:
    Trapezoid(int w, int h, float angle) 
        : Rectangle(w,h), angle(angle) {};

    virtual ~Trapezoid() {};

    float angle;

};

// Static bool to check pointer status
size_t Rectangle::count = 0;

void testFunctionRef(const AutoPtr<Rectangle>& p, int w, int h)
{
    p->width = w;
    p->height = h;
}

void testFunctionCopy(AutoPtr<Rectangle> p, int w, int h)
{
    (*p).width = w;
    (*p).height = h;
}


void test_AutoPtr_ctor_nullptr()
{
    AutoPtr<Rectangle> ptr;
    // Validate the ptr is null
    TEST_ASSERT_FALSE(ptr);
}

void test_AutoPtr_ctor_default()
{
    {
        AutoPtr<Rectangle> ptr(new Rectangle(6,3));
        // Validate the ptr hold data
        TEST_ASSERT_TRUE(ptr);
        // Validate that a rectangle was created
        TEST_ASSERT_EQUAL(1, Rectangle::count);
    }

    // Validate that the rectangle is deleted
    TEST_ASSERT_EQUAL(0, Rectangle::count);
}

void test_AutoPtr_ctor_copy()
{
    {
        AutoPtr<Rectangle> ptr_1(new Rectangle(6,3));
        AutoPtr<Rectangle> ptr_2(ptr_1);
        
        // Validate that both ptr hold data
        TEST_ASSERT_TRUE(ptr_1);
        TEST_ASSERT_TRUE(ptr_2);
        
        // Validate that both ptr point to the same rectangle
        TEST_ASSERT_EQUAL(ptr_1.operator->(), ptr_2.operator->());

        // Validate that a single rectangle was created
        TEST_ASSERT_EQUAL(1, Rectangle::count);
    }
    
    // Validate that the rectangle is deleted
    TEST_ASSERT_EQUAL(0, Rectangle::count);
}

void test_AutoPtr_ctor_cast()
{
    AutoPtr<Rectangle> ptr_1(new Trapezoid(6, 3, 30.0f));
    AutoPtr<Trapezoid> ptr_2(ptr_1);
    // Assert that the cast ctor has not lost the data
    TEST_ASSERT_EQUAL(30.0f, ptr_2->angle);
}

void test_AutoPtr_assign_op()
{
    {
        AutoPtr<Rectangle> ptr_1;
        AutoPtr<Rectangle> ptr_2(new Rectangle(6,3));
        ptr_1 = ptr_2;
        // Validate that both ptr hold data
        TEST_ASSERT_TRUE(ptr_1);
        TEST_ASSERT_TRUE(ptr_2);

        // Validate that both ptr point to the same rectangle
        TEST_ASSERT_EQUAL(ptr_1.operator->(), ptr_2.operator->());

        // Validate that a single rectangle was created
        TEST_ASSERT_EQUAL(1, Rectangle::count);
    }
    
    // Validate that the rectangle is deleted
    TEST_ASSERT_EQUAL(0, Rectangle::count);
}

void test_AutoPtr_deref_op()
{
    Rectangle* r = new Rectangle(6, 3);
    AutoPtr<Rectangle> ptr(r);
    // Assert that the deref object is the same as the ptr
    TEST_ASSERT_EQUAL(r, &(*ptr));
}

void test_AutoPtr_arrow_op()
{
    Rectangle* r = new Rectangle(6, 3);
    AutoPtr<Rectangle> ptr(r);
    // Validate that the ptr operator returns the right object
    TEST_ASSERT_EQUAL(r, ptr.operator->());
}

void test_AutoPtr_bool_op()
{
    AutoPtr<Rectangle> ptr_1;
    // Validate that nullptr is false
    TEST_ASSERT_FALSE(ptr_1);
    
    AutoPtr<Rectangle> ptr_2(new Rectangle(6,3));
    // Validate that not nullptr is true
    TEST_ASSERT_TRUE(ptr_2);
}

void test_AutoPtr_equal_op()
{
    AutoPtr<int> ptr_1(new int(1));
    AutoPtr<int> ptr_2(ptr_1);
    AutoPtr<int> ptr_3(new int(1));
    // Validate is equal
    TEST_ASSERT_TRUE(ptr_1 == ptr_2);
    TEST_ASSERT_FALSE(ptr_1 != ptr_2);
    // Validate not equal 
    TEST_ASSERT_FALSE(ptr_1 == ptr_3);
    TEST_ASSERT_TRUE(ptr_1 != ptr_3);
}

void test_AutoPtr_equal_op_ptr()
{
    int* i1 = new int(1);
    int* i2 = new int(1);
    AutoPtr<int> ptr(i1);
    // Validate is equal
    TEST_ASSERT_TRUE(ptr == i1);
    TEST_ASSERT_FALSE(ptr != i1);
    // Validate not equal
    TEST_ASSERT_FALSE(ptr == i2);
    TEST_ASSERT_TRUE(ptr != i2);
}

void test_AutoPtr_equal_op_other()
{
    AutoPtr<Rectangle> ptr_1(new Trapezoid(6, 3, 30.0f));
    AutoPtr<Trapezoid> ptr_2(ptr_1);
    AutoPtr<int> ptr_3(new int(1));
    // Validate is equal
    TEST_ASSERT_TRUE(ptr_1 == ptr_2);
    TEST_ASSERT_FALSE(ptr_1 == ptr_3);
    // Validate not equal
    TEST_ASSERT_FALSE(ptr_1 != ptr_2);
    TEST_ASSERT_TRUE(ptr_1 != ptr_3);
}

void test_AutoPtr_as()
{
    AutoPtr<Rectangle> ptr_1(new Trapezoid(6, 3, 30.0f));
    // Assert that the cast 
    TEST_ASSERT_EQUAL(30.0f, ptr_1.as<Trapezoid>()->angle);
}

void test_AutoPtr_release()
{
    AutoPtr<Rectangle> ptr(new Rectangle(6,3));
    // Validate that the rectangle is created
    TEST_ASSERT_EQUAL(1, Rectangle::count);

    TEST_ASSERT_TRUE(ptr);
    ptr.release();
    // Validate the underlying ptr is null
    TEST_ASSERT_FALSE(ptr);

    // Validate that the rectangle is deleted
    TEST_ASSERT_EQUAL(0, Rectangle::count);
}

void test_AutoPtr_fct_ref()
{
    {
        AutoPtr<Rectangle> ptr(new Rectangle(6,3));
        testFunctionRef(ptr, 36, 9);
        // Validate that interacting with an AutoPtr passed by ref changes the
        // underlying object
        TEST_ASSERT_EQUAL(36, (*ptr).width);
        TEST_ASSERT_EQUAL(9, (*ptr).height);
    }
    // Validate that the rectangle is deleted
    TEST_ASSERT_EQUAL(0, Rectangle::count);
}

void test_AutoPtr_fct_copy()
{
    {
        AutoPtr<Rectangle> ptr(new Rectangle(6,3));
        testFunctionCopy(ptr, 12, 6);
        // Validate that interacting with an AutoPtr passed by copy changes the
        // underlying object
        TEST_ASSERT_EQUAL(12, ptr->width);
        TEST_ASSERT_EQUAL(6, ptr->height);
    }
    // Validate that the rectangle is deleted
    TEST_ASSERT_EQUAL(0, Rectangle::count);
}
