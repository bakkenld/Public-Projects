#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
#include "squeue.h"

#define UNUSED(x) (void)(x)

/**
 * @brief Validates the First-In-First-Out integrity of the queue.
 */
static void test_squeue_fifo_ordering(void **state) {
    UNUSED(state);
    squeue_t *q = squeue_create();
    int input[] = {10, 20, 30};
    size_t out_sz = 0;

    // Push: [10] -> [10, 20] -> [10, 20, 30]
    for (int i = 0; i < 3; i++) {
        squeue_push(q, node_create(&input[i], sizeof(int)));
    }
    assert_int_equal(q->num_nodes, 3);

    // First Pop: Should be 10
    int *p1 = (int *)squeue_pop(q, &out_sz);
    assert_non_null(p1);
    assert_int_equal(*p1, 10);
    assert_int_equal(out_sz, sizeof(int));
    free(p1);

    // Second Pop: Should be 20
    int *p2 = (int *)squeue_pop(q, NULL);
    assert_int_equal(*p2, 20);
    free(p2);

    // Third Pop: Should be 30
    int *p3 = (int *)squeue_pop(q, NULL);
    assert_int_equal(*p3, 30);
    free(p3);

    // Queue should now be empty
    assert_int_equal(q->num_nodes, 0);
    squeue_destroy(&q);
}

/**
 * @brief Validates that peek consistently returns the front without removing it
 */
static void test_squeue_peek_consistency(void **state) {
    UNUSED(state);
    squeue_t *q = squeue_create();
    int val = 500;
    squeue_push(q, node_create(&val, sizeof(int)));

    // Peek multiple times; data should remain and be correct
    for (int i = 0; i < 3; i++) {
        int *peeked = (int *)squeue_peek(q, NULL);
        assert_non_null(peeked);
        assert_int_equal(*peeked, 500);
        assert_int_equal(q->num_nodes, 1);
    }

    squeue_destroy(&q);
}

/**
 * @brief Validates that the queue handles underflow (popping while empty) 
 * gracefully.
 */
static void test_squeue_underflow_safety(void **state) {
    UNUSED(state);
    squeue_t *q = squeue_create();
    
    // Attempt to pop from an empty queue
    void *data = squeue_pop(q, NULL);
    assert_null(data);
    
    // Attempt to peek an empty queue
    void *peeked = squeue_peek(q, NULL);
    assert_null(peeked);

    squeue_destroy(&q);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_squeue_fifo_ordering),
        cmocka_unit_test(test_squeue_peek_consistency),
        cmocka_unit_test(test_squeue_underflow_safety),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}