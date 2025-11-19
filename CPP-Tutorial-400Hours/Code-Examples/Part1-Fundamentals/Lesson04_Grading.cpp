/*
 * Lesson 4: Grade Calculator - Practical If-Else Application
 *
 * This program is a comprehensive grading system that:
 * - Calculates grades for multiple subjects
 * - Determines pass/fail status
 * - Calculates GPA
 * - Provides detailed performance analysis
 * - Demonstrates practical use of conditional statements
 *
 * Compile: cl Lesson04_Grading.cpp
 */

#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

int main() {
    cout << "============================================" << endl;
    cout << "     STUDENT GRADE CALCULATOR SYSTEM       " << endl;
    cout << "============================================" << endl << endl;

    // Student information
    string studentName;
    int studentID;

    cout << "Enter student name: ";
    getline(cin, studentName);
    cout << "Enter student ID: ";
    cin >> studentID;

    // Subject scores
    double math, english, science, history, computerScience;

    cout << "\nEnter marks for each subject (0-100):" << endl;
    cout << "Mathematics: ";
    cin >> math;
    cout << "English: ";
    cin >> english;
    cout << "Science: ";
    cin >> science;
    cout << "History: ";
    cin >> history;
    cout << "Computer Science: ";
    cin >> computerScience;

    // Calculate statistics
    double totalMarks = math + english + science + history + computerScience;
    double averageMarks = totalMarks / 5.0;
    double percentage = (totalMarks / 500.0) * 100.0;

    // Display student report
    cout << "\n============================================" << endl;
    cout << "              GRADE REPORT                  " << endl;
    cout << "============================================" << endl;
    cout << "Student Name: " << studentName << endl;
    cout << "Student ID: " << studentID << endl;
    cout << "--------------------------------------------" << endl;

    // Display individual subject grades
    cout << fixed << setprecision(2);
    cout << left << setw(20) << "Subject" << setw(10) << "Marks" << setw(10) << "Grade" << endl;
    cout << string(40, '-') << endl;

    // Function to determine letter grade
    auto getGrade = [](double marks) -> char {
        if (marks >= 90) return 'A';
        else if (marks >= 80) return 'B';
        else if (marks >= 70) return 'C';
        else if (marks >= 60) return 'D';
        else return 'F';
    };

    // Display each subject
    cout << setw(20) << "Mathematics" << setw(10) << math << setw(10) << getGrade(math) << endl;
    cout << setw(20) << "English" << setw(10) << english << setw(10) << getGrade(english) << endl;
    cout << setw(20) << "Science" << setw(10) << science << setw(10) << getGrade(science) << endl;
    cout << setw(20) << "History" << setw(10) << history << setw(10) << getGrade(history) << endl;
    cout << setw(20) << "Computer Science" << setw(10) << computerScience << setw(10) << getGrade(computerScience) << endl;

    cout << string(40, '-') << endl;
    cout << setw(20) << "Total Marks" << totalMarks << " / 500" << endl;
    cout << setw(20) << "Average Marks" << averageMarks << " / 100" << endl;
    cout << setw(20) << "Percentage" << percentage << "%" << endl;

    // Calculate overall grade
    char overallGrade;
    string gradeDescription;

    if (percentage >= 90) {
        overallGrade = 'A';
        gradeDescription = "Outstanding";
    } else if (percentage >= 80) {
        overallGrade = 'B';
        gradeDescription = "Excellent";
    } else if (percentage >= 70) {
        overallGrade = 'C';
        gradeDescription = "Good";
    } else if (percentage >= 60) {
        overallGrade = 'D';
        gradeDescription = "Satisfactory";
    } else {
        overallGrade = 'F';
        gradeDescription = "Needs Improvement";
    }

    cout << setw(20) << "Overall Grade" << overallGrade << " (" << gradeDescription << ")" << endl;

    // Calculate GPA (4.0 scale)
    double gpa;
    if (percentage >= 90) {
        gpa = 4.0;
    } else if (percentage >= 80) {
        gpa = 3.0;
    } else if (percentage >= 70) {
        gpa = 2.0;
    } else if (percentage >= 60) {
        gpa = 1.0;
    } else {
        gpa = 0.0;
    }

    cout << setw(20) << "GPA" << gpa << " / 4.0" << endl;

    // Determine pass/fail status
    cout << "\n============================================" << endl;
    cout << "              STATUS REPORT                 " << endl;
    cout << "============================================" << endl;

    bool allPassed = (math >= 60 && english >= 60 && science >= 60 &&
                      history >= 60 && computerScience >= 60);

    if (allPassed) {
        cout << "Result: PASSED" << endl;

        if (percentage >= 75) {
            cout << "Distinction: YES" << endl;
        } else {
            cout << "Distinction: NO" << endl;
        }
    } else {
        cout << "Result: FAILED" << endl;
        cout << "Failed subjects:" << endl;

        if (math < 60) cout << "  - Mathematics (needs " << (60 - math) << " more marks)" << endl;
        if (english < 60) cout << "  - English (needs " << (60 - english) << " more marks)" << endl;
        if (science < 60) cout << "  - Science (needs " << (60 - science) << " more marks)" << endl;
        if (history < 60) cout << "  - History (needs " << (60 - history) << " more marks)" << endl;
        if (computerScience < 60) cout << "  - Computer Science (needs " << (60 - computerScience) << " more marks)" << endl;
    }

    // Performance analysis
    cout << "\n============================================" << endl;
    cout << "           PERFORMANCE ANALYSIS             " << endl;
    cout << "============================================" << endl;

    // Find best and worst subjects
    double maxScore = math;
    string bestSubject = "Mathematics";
    double minScore = math;
    string worstSubject = "Mathematics";

    if (english > maxScore) { maxScore = english; bestSubject = "English"; }
    if (science > maxScore) { maxScore = science; bestSubject = "Science"; }
    if (history > maxScore) { maxScore = history; bestSubject = "History"; }
    if (computerScience > maxScore) { maxScore = computerScience; bestSubject = "Computer Science"; }

    if (english < minScore) { minScore = english; worstSubject = "English"; }
    if (science < minScore) { minScore = science; worstSubject = "Science"; }
    if (history < minScore) { minScore = history; worstSubject = "History"; }
    if (computerScience < minScore) { minScore = computerScience; worstSubject = "Computer Science"; }

    cout << "Best Performance: " << bestSubject << " (" << maxScore << ")" << endl;
    cout << "Needs Improvement: " << worstSubject << " (" << minScore << ")" << endl;

    // Provide recommendations
    cout << "\n============================================" << endl;
    cout << "            RECOMMENDATIONS                 " << endl;
    cout << "============================================" << endl;

    if (percentage >= 90) {
        cout << "Excellent work! Keep up the outstanding performance!" << endl;
    } else if (percentage >= 80) {
        cout << "Very good performance! With more effort, you can achieve excellence." << endl;
    } else if (percentage >= 70) {
        cout << "Good work! Focus on weak areas to improve further." << endl;
    } else if (percentage >= 60) {
        cout << "Satisfactory performance. Consider studying more consistently." << endl;
    } else {
        cout << "Needs significant improvement. Seek help from teachers and study more." << endl;
    }

    // Subject-specific advice
    if (math < 70) {
        cout << "- Practice more mathematics problems daily" << endl;
    }
    if (english < 70) {
        cout << "- Read more books and practice writing" << endl;
    }
    if (science < 70) {
        cout << "- Focus on understanding scientific concepts" << endl;
    }
    if (history < 70) {
        cout << "- Review historical events and timelines" << endl;
    }
    if (computerScience < 70) {
        cout << "- Practice programming and logical thinking" << endl;
    }

    cout << "\n============================================" << endl;

    return 0;
}

/*
 * Sample Run:
 * ============================================
 *      STUDENT GRADE CALCULATOR SYSTEM
 * ============================================
 *
 * Enter student name: John Smith
 * Enter student ID: 12345
 *
 * Enter marks for each subject (0-100):
 * Mathematics: 85
 * English: 78
 * Science: 92
 * History: 76
 * Computer Science: 88
 *
 * ============================================
 *               GRADE REPORT
 * ============================================
 * Student Name: John Smith
 * Student ID: 12345
 * --------------------------------------------
 * Subject             Marks     Grade
 * ----------------------------------------
 * Mathematics         85.00     B
 * English             78.00     C
 * Science             92.00     A
 * History             76.00     C
 * Computer Science    88.00     B
 * ----------------------------------------
 * Total Marks         419.00 / 500
 * Average Marks       83.80 / 100
 * Percentage          83.80%
 * Overall Grade       B (Excellent)
 * GPA                 3.00 / 4.0
 *
 * ... (continues with status and recommendations)
 */
