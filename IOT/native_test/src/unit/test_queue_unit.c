#include <stdlib.h>
#include "unity.h"
#include "queue.h"

string_queue_t q;

void setUp(void)
{
    q = queue_create_string_queue(3);
}

void tearDown(void)
{
    free(q);
}

void test_queue_enqueue_and_dequeue_with_numbers(void)
{
    int x = 1, y = 2, z = 3;

    queue_enqueue(q, &x);
    queue_enqueue(q, &y);
    queue_enqueue(q, &z);

    TEST_ASSERT_EQUAL(1, *(int *)queue_dequeue(q));
    TEST_ASSERT_EQUAL(2, *(int *)queue_dequeue(q));
    TEST_ASSERT_EQUAL(3, *(int *)queue_dequeue(q));
}

void test_queue_enqueue_and_dequeue_with_strings(void)
{
    queue_enqueue(q, "Hello");
    queue_enqueue(q, "World");
    queue_enqueue(q, "Test");

    TEST_ASSERT_EQUAL_STRING("Hello", queue_dequeue(q));
    TEST_ASSERT_EQUAL_STRING("World", queue_dequeue(q));
    TEST_ASSERT_EQUAL_STRING("Test", queue_dequeue(q));
}

void test_queue_enqueue_and_dequeue_with_structs(void)
{
    typedef struct
    {
        int id;
        char name[20];
    } Item;

    Item *item1 = malloc(sizeof(Item));
    *item1 = (Item){1, "Item 1"};
    Item *item2 = malloc(sizeof(Item));
    *item2 = (Item){2, "Item 2"};
    Item *item3 = malloc(sizeof(Item));
    *item3 = (Item){3, "Item 3"};

    queue_enqueue(q, item1);
    queue_enqueue(q, item2);
    queue_enqueue(q, item3);

    TEST_ASSERT_EQUAL(item1->name, ((Item *)queue_dequeue(q))->name);
    TEST_ASSERT_EQUAL(item2->name, ((Item *)queue_dequeue(q))->name);
    TEST_ASSERT_EQUAL(item3->name, ((Item *)queue_dequeue(q))->name);

    free(item1);
    free(item2);
    free(item3);
}

void test_queue_enqueue_and_dequeue_a_lot(void)
{
    queue_enqueue(q, "Hello");
    queue_enqueue(q, "World");
    queue_enqueue(q, "Test");

    TEST_ASSERT_EQUAL_STRING("Hello", queue_dequeue(q));
    queue_enqueue(q, "67");
    TEST_ASSERT_EQUAL_STRING("World", queue_dequeue(q));
    queue_enqueue(q, "seks syv");
    TEST_ASSERT_EQUAL_STRING("Test", queue_dequeue(q));
    TEST_ASSERT_EQUAL_STRING("67", queue_dequeue(q));
    TEST_ASSERT_EQUAL_STRING("seks syv", queue_dequeue(q));
}

void test_queue_is_full(void)
{
    TEST_ASSERT_FALSE(queue_isFull(q));
    queue_enqueue(q, "Hello");
    TEST_ASSERT_FALSE(queue_isFull(q));
    queue_enqueue(q, "World");
    TEST_ASSERT_FALSE(queue_isFull(q));
    queue_enqueue(q, "Test");
    TEST_ASSERT_TRUE(queue_isFull(q));
    queue_dequeue(q);
    TEST_ASSERT_FALSE(queue_isFull(q));
    queue_dequeue(q);
    TEST_ASSERT_FALSE(queue_isFull(q));
    queue_dequeue(q);
    TEST_ASSERT_FALSE(queue_isFull(q));
}

void test_queue_is_empty(void)
{
    TEST_ASSERT_TRUE(queue_isEmpty(q));
    queue_enqueue(q, "Hello");
    TEST_ASSERT_FALSE(queue_isEmpty(q));
    queue_enqueue(q, "World");
    TEST_ASSERT_FALSE(queue_isEmpty(q));
    queue_enqueue(q, "Test");
    TEST_ASSERT_FALSE(queue_isEmpty(q));
    queue_dequeue(q);
    TEST_ASSERT_FALSE(queue_isEmpty(q));
    queue_dequeue(q);
    TEST_ASSERT_FALSE(queue_isEmpty(q));
    queue_dequeue(q);
    TEST_ASSERT_TRUE(queue_isEmpty(q));
}

void test_queue_overflow(void)
{
    queue_enqueue(q, "Hello");
    queue_enqueue(q, "World");
    queue_enqueue(q, "Test");
    queue_enqueue(q, "Overflow");

    TEST_ASSERT_EQUAL_STRING("Hello", queue_dequeue(q));
    TEST_ASSERT_EQUAL_STRING("World", queue_dequeue(q));
    TEST_ASSERT_EQUAL_STRING("Test", queue_dequeue(q));

    TEST_ASSERT_NULL(queue_dequeue(q));
}

void test_queue_underflow(void)
{
    TEST_ASSERT_NULL(queue_dequeue(q));
}

void test_queue_peek(void)
{
    TEST_ASSERT_NULL(queue_peek(q));
    queue_enqueue(q, "Hello");
    TEST_ASSERT_EQUAL_STRING("Hello", queue_peek(q));
    queue_enqueue(q, "World");
    TEST_ASSERT_EQUAL_STRING("Hello", queue_peek(q));
    queue_dequeue(q);
    TEST_ASSERT_EQUAL_STRING("World", queue_peek(q));
}

void test_queue_string_longer_than_max(void)
{
    char long_string[1000];
    for (int i = 0; i < 1000 - 1; i++)
    {
        long_string[i] = 'A';
    }
    long_string[1000 - 1] = '\0';

    queue_enqueue(q, long_string);
    queue_enqueue(q, "Short");
    TEST_ASSERT_EQUAL_STRING(long_string, queue_dequeue(q));
    TEST_ASSERT_EQUAL_STRING("Short", queue_dequeue(q));
}