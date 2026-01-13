# Simple wrapper Makefile for CMake
BUILD_DIR = build
BINARY = VFS_Demo
CORES = 12

all: $(BUILD_DIR)
	@echo "Compiling..."
	@cd $(BUILD_DIR) && make -j$(CORES)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake ..

run: all
	@./$(BUILD_DIR)/$(BINARY)

clean:
	@rm -rf $(BUILD_DIR)
	@echo "Build directory removed."

.PHONY: all run clean