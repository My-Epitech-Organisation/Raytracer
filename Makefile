##
## EPITECH PROJECT, 2025
## Raytracer
## File description:
## Makefile
##

BUILD_DIR   := build

TARGET      := raytracer

TEST_TARGET := raytracer_tests

CMAKE_FLAGS := -DCMAKE_BUILD_TYPE=Debug

all:
	@cmake -B $(BUILD_DIR) -S . $(CMAKE_FLAGS)
	@cmake --build $(BUILD_DIR)

re: fclean all

tests_run: all
	@cmake --build $(BUILD_DIR) --target $(TEST_TARGET)
	@$(BUILD_DIR)/tests/$(TEST_TARGET)

clean:
	@cmake --build $(BUILD_DIR) --target clean || true

fclean: clean
	@rm -rf $(BUILD_DIR)

normalize:
	@echo "Applying clang format to all C++ files..."
	@find . -name "*.cpp" -o -name "*.hpp" | xargs clang-format -i


.PHONY: all re tests_run clean fclean normalize
