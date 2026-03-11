# Project-specific variables
BUILD_DIR = build
TEST_BINARY = test_suite

# Default logging level
LOG_LEVEL ?= 0

# Default llist build level (full)
LLIST_FULL_BUILD ?= 1

.PHONY: all run clean test valgrind

# Default target: Create build directory, run cmake, and compile
all:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake -DTARGET_LOG_LEVEL=$(LOG_LEVEL) \
	-DLLIST_FULL_BUILD=$(LLIST_FULL_BUILD) .. && make

# Test target: Compiles and then runs the unit tests
test: all
	@echo "Running unit tests..."
	@cd $(BUILD_DIR) && ctest --output-on-failure
	@cat $(BUILD_DIR)/Testing/Temporary/LastTest.log
	@# Alternatively, run the binary directly:
	@# ./$(BUILD_DIR)/$(TEST_BINARY)

# Valgrind target: Compiles and then runs valgrind
valgrind: all
	@echo "Starting Valgrind Leak Check..."
	@valgrind --leak-check=full \
	          --show-leak-kinds=all \
	          --track-origins=yes \
			  --log-file=build/valgrind_leak.log \
	          ./$(BUILD_DIR)/$(TEST_BINARY)
	@echo "Leak check complete"

# Clean target: Removes the entire build directory
clean:
	@echo "Cleaning up build artifacts..."
	@rm -rf $(BUILD_DIR)
	@echo "Clean complete."