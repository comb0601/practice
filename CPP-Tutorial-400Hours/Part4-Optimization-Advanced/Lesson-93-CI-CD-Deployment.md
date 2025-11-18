# Lesson 93: Continuous Integration and Deployment

**Duration**: 6 hours
**Difficulty**: Advanced
**Prerequisites**: Lessons 1-92, Git, Build Systems

## Table of Contents
1. CI/CD Fundamentals
2. Build Automation
3. GitHub Actions
4. Jenkins Pipeline
5. Docker Integration
6. Deployment Strategies

---

## 1. CI/CD Pipeline Overview

```
Code Commit → Build → Test → Deploy → Monitor
    ↓           ↓       ↓       ↓         ↓
  GitHub    Compile  Unit   Staging   Metrics
  PR/Push   Binary   Tests  Production Alerts
```

---

## 2. GitHub Actions Workflow

```yaml
# .github/workflows/cpp-ci.yml
name: C++ CI

on:
  push:
    branches: [ main, develop ]
  pull_request:
    branches: [ main ]

jobs:
  build-and-test:
    runs-on: ubuntu-latest

    steps:
    - uses: actions/checkout@v2

    - name: Install Dependencies
      run: |
        sudo apt-get update
        sudo apt-get install -y build-essential cmake

    - name: Configure CMake
      run: cmake -B build -DCMAKE_BUILD_TYPE=Release

    - name: Build
      run: cmake --build build --config Release

    - name: Run Tests
      run: cd build && ctest --output-on-failure

    - name: Run Static Analysis
      run: |
        sudo apt-get install -y clang-tidy
        clang-tidy src/*.cpp -- -std=c++17

    - name: Code Coverage
      run: |
        sudo apt-get install -y lcov
        cmake -B build-coverage -DCMAKE_BUILD_TYPE=Debug -DCOVERAGE=ON
        cmake --build build-coverage
        cd build-coverage && ctest
        lcov --capture --directory . --output-file coverage.info
        lcov --list coverage.info

    - name: Upload Artifacts
      uses: actions/upload-artifact@v2
      with:
        name: binary
        path: build/my_program
```

---

## 3. Docker for C++ Projects

```dockerfile
# Dockerfile
FROM ubuntu:22.04

# Install build tools
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy source code
COPY . .

# Build project
RUN cmake -B build -DCMAKE_BUILD_TYPE=Release && \
    cmake --build build --config Release

# Run application
CMD ["./build/my_program"]
```

```bash
# Build and run
docker build -t my-cpp-app .
docker run my-cpp-app
```

---

## 4. Jenkins Pipeline

```groovy
// Jenkinsfile
pipeline {
    agent any

    stages {
        stage('Checkout') {
            steps {
                git 'https://github.com/user/repo.git'
            }
        }

        stage('Build') {
            steps {
                sh 'cmake -B build'
                sh 'cmake --build build'
            }
        }

        stage('Test') {
            steps {
                sh 'cd build && ctest --output-on-failure'
            }
        }

        stage('Static Analysis') {
            steps {
                sh 'cppcheck --enable=all src/'
            }
        }

        stage('Package') {
            steps {
                sh 'cd build && cpack'
            }
        }

        stage('Deploy') {
            when {
                branch 'main'
            }
            steps {
                sh './deploy.sh'
            }
        }
    }

    post {
        always {
            junit 'build/test-results/*.xml'
            archiveArtifacts 'build/*.deb'
        }

        failure {
            mail to: 'team@company.com',
                 subject: "Build Failed: ${env.JOB_NAME}",
                 body: "Build ${env.BUILD_NUMBER} failed"
        }
    }
}
```

---

## 5. CMake Configuration for CI

```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.15)
project(MyProject VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable testing
enable_testing()

# Options for CI
option(BUILD_TESTS "Build tests" ON)
option(ENABLE_COVERAGE "Enable code coverage" OFF)
option(ENABLE_SANITIZERS "Enable sanitizers" OFF)

# Source files
add_executable(my_program
    src/main.cpp
    src/module1.cpp
    src/module2.cpp
)

# Tests
if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

# Coverage
if(ENABLE_COVERAGE)
    target_compile_options(my_program PRIVATE --coverage)
    target_link_options(my_program PRIVATE --coverage)
endif()

# Sanitizers
if(ENABLE_SANITIZERS)
    target_compile_options(my_program PRIVATE
        -fsanitize=address,undefined
    )
    target_link_options(my_program PRIVATE
        -fsanitize=address,undefined
    )
endif()

# Installation
install(TARGETS my_program DESTINATION bin)

# CPack for packaging
set(CPACK_PACKAGE_NAME "MyProject")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_GENERATOR "DEB;RPM;TGZ")
include(CPack)
```

---

## 6. Deployment Script

```bash
#!/bin/bash
# deploy.sh

set -e

echo "Starting deployment..."

# Build release binary
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release

# Run tests
cd build && ctest --output-on-failure
cd ..

# Package
cd build && cpack
cd ..

# Deploy to server
scp build/*.deb deploy@server:/tmp/
ssh deploy@server "sudo dpkg -i /tmp/*.deb"

# Health check
curl -f http://server:8080/health || exit 1

echo "Deployment successful!"
```

---

## Summary

✅ **Automated Builds**: Every commit triggers build
✅ **Automated Testing**: Tests run automatically
✅ **Code Quality**: Static analysis integrated
✅ **Deployment**: Push-button deployment

**Next**: Documentation and API Design!
