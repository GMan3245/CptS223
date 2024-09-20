#include "testqueue.h"

// main function
int main()
{
    test_size();
    test_isFull();
    test_isEmpty();

    test_enqueue();
    test_enqueue2();

    test_dequeue();
    test_dequeue2();

    test_peek();
    test_peak();

    return 0;
}