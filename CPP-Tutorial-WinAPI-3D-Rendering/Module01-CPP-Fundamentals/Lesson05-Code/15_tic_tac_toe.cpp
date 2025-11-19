/*
 * Program: Tic-Tac-Toe Game
 * Description: Complete tic-tac-toe game using 2D array
 * Compilation: g++ 15_tic_tac_toe.cpp -o 15_tic_tac_toe
 * Execution: ./15_tic_tac_toe
 */

#include <iostream>
using namespace std;

void initializeBoard(char board[3][3]) {
    int num = 1;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = '0' + num++;  // '1' to '9'
        }
    }
}

void displayBoard(char board[3][3]) {
    cout << "\n";
    for (int i = 0; i < 3; i++) {
        cout << " ";
        for (int j = 0; j < 3; j++) {
            cout << board[i][j];
            if (j < 2) cout << " | ";
        }
        cout << endl;
        if (i < 2) cout << "---|---|---" << endl;
    }
    cout << endl;
}

bool isValidMove(char board[3][3], int position) {
    if (position < 1 || position > 9) {
        return false;
    }

    int row = (position - 1) / 3;
    int col = (position - 1) % 3;

    return (board[row][col] != 'X' && board[row][col] != 'O');
}

void makeMove(char board[3][3], int position, char player) {
    int row = (position - 1) / 3;
    int col = (position - 1) % 3;
    board[row][col] = player;
}

bool checkWin(char board[3][3], char player) {
    // Check rows
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player) {
            return true;
        }
    }

    // Check columns
    for (int j = 0; j < 3; j++) {
        if (board[0][j] == player && board[1][j] == player && board[2][j] == player) {
            return true;
        }
    }

    // Check diagonals
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player) {
        return true;
    }

    if (board[0][2] == player && board[1][1] == player && board[2][0] == player) {
        return true;
    }

    return false;
}

bool isBoardFull(char board[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] != 'X' && board[i][j] != 'O') {
                return false;
            }
        }
    }
    return true;
}

int main() {
    cout << "=== TIC-TAC-TOE GAME ===" << endl;
    cout << "\nHow to play:" << endl;
    cout << "- Enter a number (1-9) to place your mark" << endl;
    cout << "- Player 1 is X, Player 2 is O" << endl;

    char board[3][3];
    initializeBoard(board);

    char currentPlayer = 'X';
    int moveCount = 0;
    bool gameWon = false;

    while (!gameWon && !isBoardFull(board)) {
        displayBoard(board);

        cout << "Player " << currentPlayer << "'s turn" << endl;
        cout << "Enter position (1-9): ";

        int position;
        cin >> position;

        if (!isValidMove(board, position)) {
            cout << "Invalid move! Try again." << endl;
            continue;
        }

        makeMove(board, position, currentPlayer);
        moveCount++;

        if (checkWin(board, currentPlayer)) {
            gameWon = true;
            displayBoard(board);
            cout << "*** Player " << currentPlayer << " WINS! ***" << endl;
        } else if (isBoardFull(board)) {
            displayBoard(board);
            cout << "*** DRAW! ***" << endl;
        } else {
            // Switch player
            currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
        }
    }

    cout << "\nTotal moves: " << moveCount << endl;
    cout << "Thanks for playing!" << endl;

    return 0;
}
