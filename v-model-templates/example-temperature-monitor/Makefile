CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Werror -pedantic
BUILD_DIR := build
TEST_BINARY := $(BUILD_DIR)/test_temp_monitor

.PHONY: test clean

test: $(TEST_BINARY)
	./$(TEST_BINARY)

$(TEST_BINARY): 05-coding/temp_monitor.c 05-coding/temp_monitor.h 06-unit-test/test_temp_monitor.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -I05-coding 05-coding/temp_monitor.c 06-unit-test/test_temp_monitor.c -o $(TEST_BINARY)

clean:
	rm -f $(TEST_BINARY)
