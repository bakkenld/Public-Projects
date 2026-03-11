#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
#include "llist.h"

#define UNUSED(x) (void)(x)

// Test node_create and node_destroy (The foundation)
static void test_node_lifecycle(void **state) {
    UNUSED(state);
    int val = 100;
    llist_node_t *node = node_create(&val, sizeof(int));
    assert_non_null(node);
    assert_int_equal(*(int*)node->p_data, 100);
    assert_int_equal(node_destroy(node), 0);
}

// Test llist_clear (Emptying the list without killing the container)
static void test_llist_clear(void **state) {
    UNUSED(state);
    llist_t *list = llist_create();
    int val = 5;
    llist_add_first(list, node_create(&val, sizeof(int)));
    llist_add_first(list, node_create(&val, sizeof(int)));
    
    assert_int_equal(list->num_nodes, 2);
    assert_int_equal(llist_clear(&list), 0);
    assert_int_equal(list->num_nodes, 0);
    assert_null(list->p_head);
    
    llist_destroy(&list);
}

// Test llist_num_sort
static void test_llist_sort(void **state) {
    UNUSED(state);
    llist_t *list = llist_create();
    int data[] = {30, 10, 20};
    for(int i=0; i<3; i++) llist_add_last(list, node_create(&data[i], \
        sizeof(int)));

    assert_int_equal(llist_num_sort(list), 0);
    assert_int_equal(*(int*)list->p_head->p_data, 10);
    assert_int_equal(*(int*)list->p_tail->p_data, 30);
    
    llist_destroy(&list);
}

static void test_llist_add_index_and_delete(void **state) {
    UNUSED(state);
    llist_t *list = llist_create();
    int v1 = 10, v2 = 20, v3 = 30;

    llist_add_first(list, node_create(&v1, sizeof(int))); // [10]
    llist_add_last(list, node_create(&v3, sizeof(int)));  // [10, 30]
    llist_add_index(list, node_create(&v2, sizeof(int)), 1); // [10, 20, 30]

    assert_int_equal(list->num_nodes, 3);
    
    // Delete middle
    assert_int_equal(llist_delete_index(list, 1), 0); // [10, 30]
    assert_int_equal(list->num_nodes, 2);
    assert_int_equal(*(int*)list->p_head->p_next->p_data, 30);

    llist_destroy(&list);
}

static void test_llist_peeking(void **state) {
    UNUSED(state);
    llist_t *list = llist_create();
    int val = 99;
    size_t sz = 0;
    llist_add_first(list, node_create(&val, sizeof(int)));

    int *peeked = (int*)llist_peek_index(list, 0, &sz);
    assert_int_equal(*peeked, 99);
    assert_int_equal(sz, sizeof(int));
    assert_int_equal(list->num_nodes, 1); // Peek shouldn't remove

    llist_destroy(&list);
}

static void test_llist_popping(void **state) {
    UNUSED(state);
    llist_t *list = llist_create();
    int val = 77;
    llist_add_first(list, node_create(&val, sizeof(int)));

    void *popped_data = llist_pop_first(list, NULL);
    assert_non_null(popped_data);
    assert_int_equal(*(int*)popped_data, 77);
    assert_int_equal(list->num_nodes, 0); // Pop must remove

    free(popped_data); // CRITICAL: Caller frees popped data
    llist_destroy(&list);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_node_lifecycle),
        cmocka_unit_test(test_llist_clear),
        cmocka_unit_test(test_llist_sort),
        cmocka_unit_test(test_llist_add_index_and_delete),
        cmocka_unit_test(test_llist_peeking),
        cmocka_unit_test(test_llist_popping),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}