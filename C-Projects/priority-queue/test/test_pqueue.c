#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
#include "../include/pqueue.h"

// Helper to suppress unused parameter warnings from strict compilers
#define UNUSED(x) (void)(x)

/**
 * @brief Test that items are ordered by priority, not by push order.
 */
static void test_pqueue_priority_ordering(void **state) {
    UNUSED(state);
    pqueue_t *pq = pqueue_create();
    
    int val_low = 10;    // Priority 10 (Last)
    int val_high = 1;    // Priority 1  (First)
    int val_mid = 5;     // Priority 5  (Middle)
    
    // Push in mixed order
    pqueue_push(pq, &val_low, sizeof(int), 10);
    pqueue_push(pq, &val_high, sizeof(int), 1);
    pqueue_push(pq, &val_mid, sizeof(int), 5);
    
    assert_int_equal(pq->num_nodes, 3);

    size_t out_sz = 0;
    
    // First Pop: Must be Priority 1
    pqueue_item_t *item1 = (pqueue_item_t *)pqueue_pop(pq, &out_sz);
    assert_non_null(item1);
    assert_int_equal(item1->priority, 1);
    assert_int_equal(*(int *)(item1->p_data), 1);
    free(item1); // Free the envelope node data

    // Second Pop: Must be Priority 5
    pqueue_item_t *item2 = (pqueue_item_t *)pqueue_pop(pq, NULL);
    assert_int_equal(item2->priority, 5);
    free(item2);

    // Third Pop: Must be Priority 10
    pqueue_item_t *item3 = (pqueue_item_t *)pqueue_pop(pq, NULL);
    assert_int_equal(item3->priority, 10);
    free(item3);

    pqueue_destroy(&pq);
}

/**
 * @brief Test that peek looks at the highest priority item.
 */
static void test_pqueue_peek(void **state) {
    UNUSED(state);
    pqueue_t *pq = pqueue_create();
    int data = 42;
    
    pqueue_push(pq, &data, sizeof(int), 1);
    
    pqueue_item_t *peeked = (pqueue_item_t *)pqueue_peek(pq, NULL);
    assert_non_null(peeked);
    assert_int_equal(peeked->priority, 1);
    assert_int_equal(*(int *)(peeked->p_data), 42);
    
    pqueue_destroy(&pq);
}

/**
 * @brief Test empty queue safety.
 */
static void test_pqueue_empty(void **state) {
    UNUSED(state);
    pqueue_t *pq = pqueue_create();
    
    assert_null(pqueue_pop(pq, NULL));
    assert_null(pqueue_peek(pq, NULL));
    
    pqueue_destroy(&pq);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_pqueue_priority_ordering),
        cmocka_unit_test(test_pqueue_peek),
        cmocka_unit_test(test_pqueue_empty),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}

// End of file
