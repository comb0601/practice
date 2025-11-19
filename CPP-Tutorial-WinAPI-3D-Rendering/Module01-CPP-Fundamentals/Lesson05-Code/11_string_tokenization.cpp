/*
 * Program: String Tokenization
 * Description: Splitting strings into tokens using strtok and manual parsing
 * Compilation: g++ 11_string_tokenization.cpp -o 11_string_tokenization
 * Execution: ./11_string_tokenization
 */

#include <iostream>
#include <cstring>
using namespace std;

void manualTokenize(char str[], char delimiter) {
    cout << "Tokens: ";
    int start = 0;
    int len = strlen(str);

    for (int i = 0; i <= len; i++) {
        if (str[i] == delimiter || str[i] == '\0') {
            // Print token from start to i-1
            if (i > start) {
                for (int j = start; j < i; j++) {
                    cout << str[j];
                }
                cout << " | ";
            }
            start = i + 1;
        }
    }
    cout << endl;
}

int countTokens(const char str[], char delimiter) {
    int count = 0;
    bool inToken = false;

    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] != delimiter) {
            if (!inToken) {
                count++;
                inToken = true;
            }
        } else {
            inToken = false;
        }
    }

    return count;
}

void splitCSV(char str[]) {
    cout << "CSV Fields:" << endl;
    int fieldNum = 1;
    int start = 0;
    int len = strlen(str);

    for (int i = 0; i <= len; i++) {
        if (str[i] == ',' || str[i] == '\0') {
            cout << "Field " << fieldNum << ": ";
            for (int j = start; j < i; j++) {
                cout << str[j];
            }
            cout << endl;
            fieldNum++;
            start = i + 1;
        }
    }
}

int main() {
    cout << "=== String Tokenization ===" << endl << endl;

    // Using strtok
    cout << "1. Using strtok (space delimiter):" << endl;
    char str1[] = "The quick brown fox jumps";
    cout << "Original: " << str1 << endl;

    char* token = strtok(str1, " ");
    cout << "Tokens: ";
    while (token != nullptr) {
        cout << token << " | ";
        token = strtok(nullptr, " ");
    }
    cout << endl;
    // Note: str1 is now modified by strtok!
    cout << endl;

    // Multiple delimiters with strtok
    cout << "2. Multiple Delimiters (space, comma, period):" << endl;
    char str2[] = "Hello, world. How are you?";
    cout << "Original: " << str2 << endl;

    token = strtok(str2, " ,.");
    cout << "Tokens: ";
    while (token != nullptr) {
        cout << token << " | ";
        token = strtok(nullptr, " ,.");
    }
    cout << endl << endl;

    // Manual tokenization
    cout << "3. Manual Tokenization (hyphen delimiter):" << endl;
    char str3[] = "2024-11-19";
    cout << "Original: " << str3 << endl;
    manualTokenize(str3, '-');
    cout << endl;

    // Count tokens
    cout << "4. Count Tokens:" << endl;
    char str4[] = "apple,banana,cherry,date";
    cout << "String: " << str4 << endl;
    cout << "Number of tokens: " << countTokens(str4, ',') << endl << endl;

    // CSV parsing
    cout << "5. CSV Parsing:" << endl;
    char csv[] = "John,Doe,30,Engineer";
    cout << "CSV String: " << csv << endl;
    splitCSV(csv);
    cout << endl;

    // Extract words from sentence
    cout << "6. Extract Words:" << endl;
    char sentence[] = "C++ is a powerful language";
    cout << "Sentence: " << sentence << endl;

    char tempSentence[100];
    strcpy(tempSentence, sentence);  // Make a copy

    token = strtok(tempSentence, " ");
    int wordCount = 0;
    cout << "Words:" << endl;
    while (token != nullptr) {
        wordCount++;
        cout << "  " << wordCount << ". " << token << endl;
        token = strtok(nullptr, " ");
    }
    cout << "Total words: " << wordCount << endl << endl;

    // Path tokenization
    cout << "7. Path Tokenization:" << endl;
    char path[] = "/home/user/documents/file.txt";
    cout << "Path: " << path << endl;

    char tempPath[100];
    strcpy(tempPath, path);

    token = strtok(tempPath, "/");
    cout << "Path components:" << endl;
    int level = 1;
    while (token != nullptr) {
        cout << "  Level " << level << ": " << token << endl;
        token = strtok(nullptr, "/");
        level++;
    }

    return 0;
}
