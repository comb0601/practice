#!/bin/bash

# Build All Lessons Script
# Compiles all 100 lessons to verify code quality

set -e  # Exit on error

echo "======================================================"
echo "Building All 100 Part 4 Lessons"
echo "======================================================"
echo ""

BASE_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$BASE_DIR/build_all"
SUCCESS_COUNT=0
FAIL_COUNT=0
FAILED_LESSONS=()

# Create build directory
mkdir -p "$BUILD_DIR"

# Function to build a single lesson
build_lesson() {
    local lesson_dir=$1
    local lesson_name=$(basename "$lesson_dir")

    echo -n "Building $lesson_name... "

    # Create lesson build directory
    local lesson_build="$BUILD_DIR/$lesson_name"
    mkdir -p "$lesson_build"

    # Try to build with CMake
    if cd "$lesson_dir" && \
       cmake -S . -B "$lesson_build" -DCMAKE_BUILD_TYPE=Release > /dev/null 2>&1 && \
       cmake --build "$lesson_build" --config Release > /dev/null 2>&1; then
        echo "✓ SUCCESS"
        ((SUCCESS_COUNT++))
        return 0
    else
        echo "✗ FAILED"
        ((FAIL_COUNT++))
        FAILED_LESSONS+=("$lesson_name")
        return 1
    fi
}

# Build all lessons
for lesson_dir in "$BASE_DIR"/Lesson*/; do
    if [ -d "$lesson_dir" ]; then
        build_lesson "$lesson_dir" || true  # Continue even if one fails
    fi
done

# Summary
echo ""
echo "======================================================"
echo "Build Summary"
echo "======================================================"
echo "Total lessons: $((SUCCESS_COUNT + FAIL_COUNT))"
echo "Successful:    $SUCCESS_COUNT"
echo "Failed:        $FAIL_COUNT"
echo ""

if [ ${#FAILED_LESSONS[@]} -gt 0 ]; then
    echo "Failed lessons:"
    for lesson in "${FAILED_LESSONS[@]}"; do
        echo "  - $lesson"
    done
    echo ""
fi

if [ $FAIL_COUNT -eq 0 ]; then
    echo "✓ All lessons built successfully!"
    exit 0
else
    echo "✗ Some lessons failed to build"
    exit 1
fi
