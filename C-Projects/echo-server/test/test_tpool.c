#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include "cmocka.h"
#include "../lib/tpool/tpool.h"

// Helper to suppress unused parameter warnings from strict compilers
#define UNUSED(x) (void)(x)

// Helper struct for testing execution
typedef struct {
    int value;
    int *output_array;
    int *index;
    pthread_mutex_t *mutex;
} test_data_t;

// The work function used for testing
void mock_work_fn(void *arg) {
    test_data_t *data = (test_data_t *)arg;
    
    pthread_mutex_lock(data->mutex);
    data->output_array[(*data->index)++] = data->value;
    pthread_mutex_unlock(data->mutex);
    
    // Simulate some "work" time
    usleep(10000); 
}

/**
 * Test 1: Ensure pool creation and immediate destruction works.
 */
static void test_tpool_create_destroy(void **state) {
    UNUSED(state);
    tpool_t *tm = tpool_create(4);
    assert_non_null(tm);
    tpool_destroy(tm);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_tpool_create_destroy),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}

// End of file
