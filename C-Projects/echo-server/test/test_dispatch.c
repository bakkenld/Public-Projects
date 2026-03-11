#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <unistd.h>
#include <poll.h>
#include "dispatch.h"

// Helper to suppress unused parameter warnings from strict compilers
#define UNUSED(x) (void)(x)

/**
 * @brief Test successful initialization and destruction
 */
static void test_dispatch_init_destroy(void **state) {
    UNUSED(state);
    dispatch_manager_t *mgr = dispatch_init(10);
    assert_non_null(mgr);
    assert_int_equal(mgr->fd_size, 10);
    assert_int_equal(mgr->fd_count, 0);
    assert_non_null(mgr->p_fds);
    assert_non_null(mgr->p_returned);
    
    dispatch_destroy(mgr);
}

/**
 * @brief Test initialization with default size (passing 0)
 */
static void test_dispatch_init_default(void **state) {
    UNUSED(state);
    dispatch_manager_t *mgr = dispatch_init(0);
    assert_non_null(mgr);
    // Code says it defaults to 8 if num is 0
    assert_int_equal(mgr->fd_size, 8);
    dispatch_destroy(mgr);
}

/**
 * @brief Test adding file descriptors and array expansion
 */
static void test_add_to_fds(void **state) {
    UNUSED(state);
    dispatch_manager_t *mgr = dispatch_init(2);
    
    // Add two FDs (fill the initial array)
    add_to_fds(mgr, 100);
    add_to_fds(mgr, 101);
    
    assert_int_equal(mgr->fd_count, 2);
    assert_int_equal(mgr->p_fds[0].fd, 100);
    
    // This should trigger a realloc (expand size from 2 to 4)
    add_to_fds(mgr, 102);
    
    assert_int_equal(mgr->fd_count, 3);
    assert_int_equal(mgr->fd_size, 4);
    assert_int_equal(mgr->p_fds[2].fd, 102);
    
    // Clean up. Note: dispatch_destroy calls close() on these fds.
    // In a real test environment, you'd use pipe() or dummy fds.
    dispatch_destroy(mgr);
}

/**
 * @brief Test deleting a file descriptor (swap-to-back logic)
 */
static void test_del_from_fds(void **state) {
    UNUSED(state);
    dispatch_manager_t *mgr = dispatch_init(5);
    add_to_fds(mgr, 10); // Index 0
    add_to_fds(mgr, 20); // Index 1
    add_to_fds(mgr, 30); // Index 2
    
    // Delete index 0 (fd 10). Should swap index 2 (fd 30) into index 0.
    del_from_fds(mgr, 0);
    
    assert_int_equal(mgr->fd_count, 2);
    assert_int_equal(mgr->p_fds[0].fd, 30);
    
    dispatch_destroy(mgr);
}

/**
 * @brief Test returning FDs from workers and reintegration
 */
static void test_dispatch_return_and_reintegrate(void **state) {
    UNUSED(state);
    dispatch_manager_t *mgr = dispatch_init(5);
    
    // Simulate a worker returning an FD
    dispatch_return_fd(mgr, 50);
    assert_int_equal(mgr->return_count, 1);
    assert_int_equal(mgr->p_returned[0], 50);
    
    // Reintegrate it back into the main poll array
    reintegrate_returned_fds(mgr);
    assert_int_equal(mgr->return_count, 0);
    assert_int_equal(mgr->fd_count, 1);
    assert_int_equal(mgr->p_fds[0].fd, 50);
    
    dispatch_destroy(mgr);
}

/**
 * @brief Test processing connections when revents are set
 */
static void test_process_connections(void **state) {
    UNUSED(state);
    dispatch_manager_t *mgr = dispatch_init(5);
    int listener = 3;
    
    add_to_fds(mgr, listener); // Index 0
    add_to_fds(mgr, 44);       // Index 1 (client)
    
    // Simulate poll() activity on client fd 44
    mgr->p_fds[1].revents = POLLIN;
    
    int ready = process_connections(listener, mgr);
    
    // Should return 44 and remove it from the poll list
    assert_int_equal(ready, 44);
    assert_int_equal(mgr->fd_count, 1); // Only listener remains
    
    dispatch_destroy(mgr);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_dispatch_init_destroy),
        cmocka_unit_test(test_dispatch_init_default),
        cmocka_unit_test(test_add_to_fds),
        cmocka_unit_test(test_del_from_fds),
        cmocka_unit_test(test_dispatch_return_and_reintegrate),
        cmocka_unit_test(test_process_connections),
    };
    
    return cmocka_run_group_tests(tests, NULL, NULL);
}