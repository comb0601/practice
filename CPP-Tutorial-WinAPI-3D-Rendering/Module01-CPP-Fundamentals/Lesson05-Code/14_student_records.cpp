/*
 * Program: Student Records System
 * Description: Managing student data using parallel arrays
 * Compilation: g++ 14_student_records.cpp -o 14_student_records
 * Execution: ./14_student_records
 */

#include <iostream>
#include <cstring>
#include <iomanip>
using namespace std;

const int MAX_STUDENTS = 100;

void displayStudents(char names[][50], int ages[], double gpas[], int count) {
    cout << "\n=== Student Records ===" << endl;
    cout << left << setw(20) << "Name" << setw(10) << "Age" << setw(10) << "GPA" << endl;
    cout << string(40, '-') << endl;

    for (int i = 0; i < count; i++) {
        cout << left << setw(20) << names[i]
             << setw(10) << ages[i]
             << fixed << setprecision(2) << gpas[i] << endl;
    }
    cout << endl;
}

int findStudentByName(char names[][50], int count, const char* searchName) {
    for (int i = 0; i < count; i++) {
        if (strcmp(names[i], searchName) == 0) {
            return i;
        }
    }
    return -1;  // Not found
}

double calculateAverageGPA(double gpas[], int count) {
    double sum = 0;
    for (int i = 0; i < count; i++) {
        sum += gpas[i];
    }
    return sum / count;
}

int findHighestGPAIndex(double gpas[], int count) {
    int maxIndex = 0;
    for (int i = 1; i < count; i++) {
        if (gpas[i] > gpas[maxIndex]) {
            maxIndex = i;
        }
    }
    return maxIndex;
}

void sortByGPA(char names[][50], int ages[], double gpas[], int count) {
    // Bubble sort - descending order
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (gpas[j] < gpas[j + 1]) {
                // Swap GPA
                double tempGPA = gpas[j];
                gpas[j] = gpas[j + 1];
                gpas[j + 1] = tempGPA;

                // Swap Age
                int tempAge = ages[j];
                ages[j] = ages[j + 1];
                ages[j + 1] = tempAge;

                // Swap Name
                char tempName[50];
                strcpy(tempName, names[j]);
                strcpy(names[j], names[j + 1]);
                strcpy(names[j + 1], tempName);
            }
        }
    }
}

int countStudentsAboveGPA(double gpas[], int count, double threshold) {
    int above = 0;
    for (int i = 0; i < count; i++) {
        if (gpas[i] >= threshold) {
            above++;
        }
    }
    return above;
}

int main() {
    cout << "=== Student Records Management System ===" << endl << endl;

    // Parallel arrays to store student data
    char names[MAX_STUDENTS][50] = {
        "Alice Johnson",
        "Bob Smith",
        "Carol Davis",
        "David Wilson",
        "Emma Brown"
    };

    int ages[MAX_STUDENTS] = {20, 19, 21, 20, 22};

    double gpas[MAX_STUDENTS] = {3.8, 3.5, 3.9, 3.2, 3.7};

    int studentCount = 5;

    // Display all students
    displayStudents(names, ages, gpas, studentCount);

    // Calculate average GPA
    cout << "1. Class Statistics:" << endl;
    double avgGPA = calculateAverageGPA(gpas, studentCount);
    cout << "Average GPA: " << fixed << setprecision(2) << avgGPA << endl << endl;

    // Find highest GPA
    cout << "2. Top Student:" << endl;
    int topIndex = findHighestGPAIndex(gpas, studentCount);
    cout << "Highest GPA: " << names[topIndex]
         << " with " << gpas[topIndex] << endl << endl;

    // Search for a student
    cout << "3. Search Student:" << endl;
    const char* searchName = "Bob Smith";
    int index = findStudentByName(names, studentCount, searchName);

    if (index != -1) {
        cout << "Found: " << names[index] << endl;
        cout << "  Age: " << ages[index] << endl;
        cout << "  GPA: " << gpas[index] << endl;
    } else {
        cout << searchName << " not found!" << endl;
    }
    cout << endl;

    // Count students above threshold
    cout << "4. Students with GPA >= 3.5:" << endl;
    int count = countStudentsAboveGPA(gpas, studentCount, 3.5);
    cout << "Count: " << count << " students" << endl << endl;

    // Sort by GPA
    cout << "5. Students Sorted by GPA (Descending):" << endl;
    sortByGPA(names, ages, gpas, studentCount);
    displayStudents(names, ages, gpas, studentCount);

    // Add new student
    cout << "6. Add New Student:" << endl;
    strcpy(names[studentCount], "Frank Miller");
    ages[studentCount] = 19;
    gpas[studentCount] = 3.6;
    studentCount++;

    cout << "Added Frank Miller" << endl;
    displayStudents(names, ages, gpas, studentCount);

    // GPA distribution
    cout << "7. GPA Distribution:" << endl;
    int excellent = 0, good = 0, average = 0;

    for (int i = 0; i < studentCount; i++) {
        if (gpas[i] >= 3.7) excellent++;
        else if (gpas[i] >= 3.3) good++;
        else average++;
    }

    cout << "Excellent (>= 3.7): " << excellent << endl;
    cout << "Good (3.3 - 3.6):   " << good << endl;
    cout << "Average (< 3.3):    " << average << endl;

    return 0;
}
