/*
 * Program: Number Guessing Game
 * Description: Interactive game where player guesses a random number
 * Compilation: g++ 07_guessing_game.cpp -o 07_guessing_game
 * Execution: ./07_guessing_game
 */

#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int main() {
    // Seed random number generator
    srand(time(0));

    cout << "=== Number Guessing Game ===" << endl << endl;

    bool playAgain = true;
    char choice;

    while (playAgain) {
        // Generate random number between 1 and 100
        int secretNumber = rand() % 100 + 1;
        int guess;
        int attempts = 0;
        int maxAttempts = 10;

        cout << "I'm thinking of a number between 1 and 100..." << endl;
        cout << "You have " << maxAttempts << " attempts to guess it!" << endl << endl;

        bool guessedCorrectly = false;

        while (attempts < maxAttempts && !guessedCorrectly) {
            cout << "Attempt " << (attempts + 1) << "/" << maxAttempts << endl;
            cout << "Enter your guess: ";
            cin >> guess;

            attempts++;

            if (guess == secretNumber) {
                guessedCorrectly = true;
                cout << "\n*** CONGRATULATIONS! ***" << endl;
                cout << "You guessed the number in " << attempts << " attempts!" << endl;

                if (attempts <= 3) {
                    cout << "Excellent! You're a guessing master!" << endl;
                } else if (attempts <= 6) {
                    cout << "Good job! You're pretty good at this!" << endl;
                } else {
                    cout << "You made it! Better luck next time!" << endl;
                }
            }
            else if (guess < secretNumber) {
                cout << "Too low! Try a higher number." << endl;

                if (secretNumber - guess > 20) {
                    cout << "Hint: You're way too low!" << endl;
                }
            }
            else {
                cout << "Too high! Try a lower number." << endl;

                if (guess - secretNumber > 20) {
                    cout << "Hint: You're way too high!" << endl;
                }
            }

            // Show remaining attempts
            if (!guessedCorrectly && attempts < maxAttempts) {
                cout << "Remaining attempts: " << (maxAttempts - attempts) << endl;
            }

            cout << endl;
        }

        if (!guessedCorrectly) {
            cout << "Game Over! You've used all your attempts." << endl;
            cout << "The secret number was: " << secretNumber << endl << endl;
        }

        // Play again?
        cout << "Do you want to play again? (y/n): ";
        cin >> choice;
        cout << endl;

        if (choice != 'y' && choice != 'Y') {
            playAgain = false;
            cout << "Thanks for playing! Goodbye!" << endl;
        } else {
            cout << "Starting new game..." << endl << endl;
        }
    }

    return 0;
}
