/*
 * Program: Vector Operations (using arrays)
 * Description: Mathematical vector operations - dot product, magnitude, addition
 * Compilation: g++ 10_vector_operations.cpp -o 10_vector_operations
 * Execution: ./10_vector_operations
 */

#include <iostream>
#include <cmath>
using namespace std;

void printVector(double vec[], int size, const char* name) {
    cout << name << " = (";
    for (int i = 0; i < size; i++) {
        cout << vec[i];
        if (i < size - 1) cout << ", ";
    }
    cout << ")" << endl;
}

void addVectors(double v1[], double v2[], double result[], int size) {
    for (int i = 0; i < size; i++) {
        result[i] = v1[i] + v2[i];
    }
}

void subtractVectors(double v1[], double v2[], double result[], int size) {
    for (int i = 0; i < size; i++) {
        result[i] = v1[i] - v2[i];
    }
}

double dotProduct(double v1[], double v2[], int size) {
    double result = 0;
    for (int i = 0; i < size; i++) {
        result += v1[i] * v2[i];
    }
    return result;
}

double magnitude(double vec[], int size) {
    double sumSquares = 0;
    for (int i = 0; i < size; i++) {
        sumSquares += vec[i] * vec[i];
    }
    return sqrt(sumSquares);
}

void scalarMultiply(double vec[], double scalar, double result[], int size) {
    for (int i = 0; i < size; i++) {
        result[i] = vec[i] * scalar;
    }
}

void normalize(double vec[], double result[], int size) {
    double mag = magnitude(vec, size);
    if (mag != 0) {
        for (int i = 0; i < size; i++) {
            result[i] = vec[i] / mag;
        }
    }
}

// Cross product (3D only)
void crossProduct(double v1[], double v2[], double result[]) {
    result[0] = v1[1] * v2[2] - v1[2] * v2[1];
    result[1] = v1[2] * v2[0] - v1[0] * v2[2];
    result[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

int main() {
    cout << "=== Vector Operations ===" << endl << endl;

    // 3D vectors
    double v1[] = {3.0, 4.0, 0.0};
    double v2[] = {1.0, 2.0, 3.0};
    double result[3];

    // Display vectors
    cout << "1. Vector Representation:" << endl;
    printVector(v1, 3, "v1");
    printVector(v2, 3, "v2");
    cout << endl;

    // Vector addition
    cout << "2. Vector Addition:" << endl;
    addVectors(v1, v2, result, 3);
    printVector(v1, 3, "v1");
    printVector(v2, 3, "v2");
    printVector(result, 3, "v1 + v2");
    cout << endl;

    // Vector subtraction
    cout << "3. Vector Subtraction:" << endl;
    subtractVectors(v1, v2, result, 3);
    printVector(v1, 3, "v1");
    printVector(v2, 3, "v2");
    printVector(result, 3, "v1 - v2");
    cout << endl;

    // Scalar multiplication
    cout << "4. Scalar Multiplication:" << endl;
    scalarMultiply(v1, 2.0, result, 3);
    printVector(v1, 3, "v1");
    cout << "Scalar = 2.0" << endl;
    printVector(result, 3, "v1 * 2.0");
    cout << endl;

    // Dot product
    cout << "5. Dot Product:" << endl;
    printVector(v1, 3, "v1");
    printVector(v2, 3, "v2");
    double dot = dotProduct(v1, v2, 3);
    cout << "v1 · v2 = " << dot << endl << endl;

    // Magnitude
    cout << "6. Vector Magnitude:" << endl;
    printVector(v1, 3, "v1");
    double mag = magnitude(v1, 3);
    cout << "|v1| = " << mag << endl << endl;

    // Normalization
    cout << "7. Vector Normalization:" << endl;
    printVector(v1, 3, "v1");
    normalize(v1, result, 3);
    printVector(result, 3, "normalized v1");
    cout << "Magnitude of normalized vector: " << magnitude(result, 3) << endl << endl;

    // Cross product (3D)
    cout << "8. Cross Product (3D only):" << endl;
    printVector(v1, 3, "v1");
    printVector(v2, 3, "v2");
    crossProduct(v1, v2, result);
    printVector(result, 3, "v1 × v2");
    cout << endl;

    // Angle between vectors
    cout << "9. Angle Between Vectors:" << endl;
    printVector(v1, 3, "v1");
    printVector(v2, 3, "v2");
    double cosTheta = dotProduct(v1, v2, 3) / (magnitude(v1, 3) * magnitude(v2, 3));
    double angleRad = acos(cosTheta);
    double angleDeg = angleRad * 180.0 / M_PI;
    cout << "Angle = " << angleDeg << " degrees" << endl;

    return 0;
}
