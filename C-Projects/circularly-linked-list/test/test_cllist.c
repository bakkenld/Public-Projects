#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdlib.h>
#include <cmocka.h>
#include "cllist.h"

// Helper to suppress unused parameter warnings from strict compilers
#define UNUSED(x) (void)(x)

/**
 * @brief Setup: Allocates a new list before every test.
 */
static int setup(void **state)
{
    UNUSED(state);
    cllist_t *list = cllist_create();
    if (NULL == list)
    {
        return -1;
    }
    *state = list;
    return 0;
}

/**
 * @brief Teardown: Frees the list after every test.
 */
static int teardown(void **state)
{
    UNUSED(state);
    cllist_t *list = (cllist_t *)*state;
    cllist_destroy(&list);
    return 0;
}

/**
 * @brief Verifies that adding nodes to front and back maintains the tail->head link.
 */
static void test_cllist_insert_integrity(void **state)
{
    UNUSED(state);
    cllist_t *list = (cllist_t *)*state;
    int v1 = 10, v2 = 20, v3 = 30;

    cllist_add_last(list, node_create(&v1, sizeof(int)));
    cllist_add_last(list, node_create(&v2, sizeof(int)));
    
    assert_ptr_equal(list->p_tail->p_next, list->p_head);
    assert_int_equal(*(int*)list->p_tail->p_next->p_data, 10);

    cllist_add_first(list, node_create(&v3, sizeof(int)));
    
    // Tail should always point back to the new head
    assert_ptr_equal(list->p_tail->p_next, list->p_head);
}

/**
 * @brief Validates that pop and delete operations correctly "re-circle" the list.
 */
static void test_cllist_delete_and_pop(void **state)
{
    UNUSED(state);
    cllist_t *list = (cllist_t *)*state;
    int vals[] = {100, 200, 300};
    size_t sz_out;

    for(int i = 0; i < 3; i++) {
        cllist_add_last(list, node_create(&vals[i], sizeof(int)));
    }

    void *data = cllist_pop_last(list, &sz_out);
    assert_int_equal(*(int*)data, 300);
    free(data); // pop returns data, we must free it

    // Check circularity after pop
    assert_int_equal(list->num_nodes, 2);
    assert_ptr_equal(list->p_tail->p_next, list->p_head);

    cllist_delete_first(list);
    
    // Check circularity after delete (1 node left)
    assert_int_equal(list->num_nodes, 1);
    assert_ptr_equal(list->p_tail->p_next, list->p_head);
}

/**
 * @brief Ensures cllist_clear prevents the infinite loop.
 */
static void test_cllist_clear_safety(void **state)
{
    UNUSED(state);
    cllist_t *list = (cllist_t *)*state;
    int val = 50;

    cllist_add_last(list, node_create(&val, sizeof(int)));
    cllist_add_last(list, node_create(&val, sizeof(int)));

    assert_int_equal(cllist_clear(list), 0);
    assert_int_equal(list->num_nodes, 0);
    assert_null(list->p_head);
    assert_null(list->p_tail);
}

/**
 * @brief Tests that indexing operations correctly maintain circularity.
 */
static void test_cllist_index_circularity(void **state)
{
    UNUSED(state);
    cllist_t *list = (cllist_t *)*state;
    int v1 = 5, v2 = 15;

    cllist_add_index(list, node_create(&v1, sizeof(int)), 0);
    assert_ptr_equal(list->p_tail->p_next, list->p_head);

    cllist_add_index(list, node_create(&v2, sizeof(int)), 1);
    assert_ptr_equal(list->p_tail->p_next, list->p_head);
}

/**
 * @brief The "Infinite Walk": Prove the list loops by traversing multiple steps
 */
static void test_cllist_rotation(void **state)
{
    UNUSED(state);
    cllist_t *list = (cllist_t *)*state;
    int vals[] = {1, 2, 3};
    for(int i=0; i<3; i++) cllist_add_last(list, node_create(&vals[i], sizeof(int)));

    llist_node_t *curr = list->p_head;
    
    // Walk 7 steps: 1->2->3 -> 1->2->3 -> 1
    for(int i = 0; i < 6; i++) {
        curr = curr->p_next;
    }

    // After 6 steps in a 3-node list, we should be back at the head
    assert_ptr_equal(curr, list->p_head);
    assert_int_equal(*(int*)curr->p_data, 1);
}

int main(void)
{
    // We use cmocka_unit_test_setup_teardown to register the helpers
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_cllist_insert_integrity, setup, teardown),
        cmocka_unit_test_setup_teardown(test_cllist_delete_and_pop, setup, teardown),
        cmocka_unit_test_setup_teardown(test_cllist_clear_safety, setup, teardown),
        cmocka_unit_test_setup_teardown(test_cllist_index_circularity, setup, teardown),
        cmocka_unit_test_setup_teardown(test_cllist_rotation, setup, teardown),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}

// End of file
