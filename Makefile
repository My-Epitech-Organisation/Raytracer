##
## EPITECH PROJECT, 2025
## Raytracer
## File description:
## Makefile
##

BUILD_DIR   := build

TARGET      := raytracer

TEST_TARGET := raytracer_tests

CMAKE_FLAGS := -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=OFF

CMAKE_TEST_FLAGS := -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON

CP = cp

all:
	@echo "Building project..."
	@cmake -B $(BUILD_DIR) -S . $(CMAKE_FLAGS)
	@cmake --build $(BUILD_DIR)
	@$(CP) $(BUILD_DIR)/src/$(TARGET) ./

re: fclean all

tests_run:
	@echo "Running tests..."
	@cmake -B $(BUILD_DIR) -S . $(CMAKE_TEST_FLAGS)
	@cmake --build $(BUILD_DIR) --target $(TEST_TARGET)
	@$(BUILD_DIR)/tests/$(TEST_TARGET)

clean:
	@echo "Cleaning up build directory..."
	@cmake --build $(BUILD_DIR) --target clean || true

fclean: clean
	@echo "Cleaning up build directory..."
	@rm -rf $(BUILD_DIR)
	@rm -f $(TARGET)
	@echo "Cleaning up generated documentation..."
	@rm -rf $(DOCS_OUT)

normalize:
	@echo "Applying clang format to all C++ files..."
	@echo "Using clang-format version: $$(clang-format --version)"
	@find . \( -name "*.cpp" -o -name "*.hpp" \) | xargs clang-format -i

cov: fclean
	@echo "Generating coverage report..."
	@cmake -B $(BUILD_DIR) -S . $(CMAKE_TEST_FLAGS) \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_CXX_FLAGS="--coverage" \
		-DCMAKE_C_FLAGS="--coverage"
	@cmake --build $(BUILD_DIR) --target $(TEST_TARGET)
	@$(BUILD_DIR)/tests/$(TEST_TARGET)
	@cd $(BUILD_DIR) && lcov --capture --directory . --output-file \
		coverage.info --ignore-errors gcov --ignore-errors mismatch \
		--ignore-errors unused --memory 0
	@cd $(BUILD_DIR) && lcov --remove coverage.info '*/_deps/*' \
		'*/usr/include/*' '*/tests/*' --output-file coverage_filtered.info
	@cd $(BUILD_DIR) && \
		genhtml coverage_filtered.info --output-directory coverage

	@echo "Coverage report generated at: build/coverage/index.html"

check_normalize:
	@echo "Checking code formatting with clang-format..."
	@echo "Using clang-format version: $$(clang-format --version)"
	@find . \( -name "*.cpp" -o -name "*.hpp" \) -exec clang-format -n -Werror {} \;

doc:
	@echo "Generating documentation..."
	@doxygen Doxyfile
	@# Cross-platform open command - uses appropriate opener by OS
	@if [ "$(shell uname)" = "Darwin" ]; then \
		open docs/html/index.html; \
	elif [ "$(shell uname)" = "Linux" ]; then \
		xdg-open docs/html/index.html 2>/dev/null || \
		echo "Documentation generated at: docs/html/index.html"; \
	else \
		echo "Documentation generated at: docs/html/index.html"; \
	fi

.PHONY: all re tests_run clean fclean normalize check_normalize cov doc
