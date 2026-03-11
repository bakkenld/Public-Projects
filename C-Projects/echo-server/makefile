# Project-specific variables
BUILD_DIR = build
BINARY = echo_server
TEST_BINARY = test_suite

# Default logging level
LOG_LEVEL ?= 0

.PHONY: all run clean test valgrind helgrind

# Default target: Create build directory, run cmake, and compile
all:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake -DTARGET_LOG_LEVEL=$(LOG_LEVEL) .. && make

# Run target: Compiles and then executes the server
run: all
	@echo "Launching $(BINARY) with LOG_LEVEL=$(LOG_LEVEL) ..."
	@./$(BUILD_DIR)/$(BINARY)

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
	          ./$(BUILD_DIR)/$(BINARY) & \
	SERVER_PID=$$!; \
	sleep 2; \
	echo "Sending SIGINT to PID $$SERVER_PID..."; \
	kill -INT $$SERVER_PID; \
	wait $$SERVER_PID || true
	@echo "Leak check complete"

# Helgrind target: Compiles and then runs helgrind
helgrind: all
	@echo "Starting Helgrind Race Condition Check..."
	@valgrind --tool=helgrind -s \
			  --log-file=build/helgrind.log \
	          ./$(BUILD_DIR)/$(BINARY) & \
	SERVER_PID=$$!; \
	sleep 2; \
	kill -INT $$SERVER_PID; \
	wait $$SERVER_PID || true
	@echo "Helgrind check complete"

# Clean target: Removes the entire build directory
clean:
	@echo "Cleaning up build artifacts..."
	@rm -rf $(BUILD_DIR)
	@echo "Clean complete."