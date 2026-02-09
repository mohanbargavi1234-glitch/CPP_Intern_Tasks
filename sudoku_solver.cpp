#include <iostream>
using namespace std;

// Check if num is safe in given row
bool isSafeRow(int board[9][9], int row, int num) {
    for (int col = 0; col < 9; col++) {
        if (board[row][col] == num)
            return false;
    }
    return true;
}

// Check if num is safe in given column
bool isSafeCol(int board[9][9], int col, int num) {
    for (int row = 0; row < 9; row++) {
        if (board[row][col] == num)
            return false;
    }
    return true;
}

// Check if num is safe in 3x3 subgrid
bool isSafeSubgrid(int board[9][9], int row, int col, int num) {
    int startRow = row - row % 3;
    int startCol = col - col % 3;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[startRow + i][startCol + j] == num)
                return false;
        }
    }
    return true;
}

// Combined safety check
bool isSafe(int board[9][9], int row, int col, int num) {
    return isSafeRow(board, row, num) &&
           isSafeCol(board, col, num) &&
           isSafeSubgrid(board, row, col, num);
}

// Find next empty cell
bool findEmpty(int board[9][9], int &row, int &col) {
    for (row = 0; row < 9; row++) {
        for (col = 0; col < 9; col++) {
            if (board[row][col] == 0)
                return true;
        }
    }
    return false; // Solved
}

// Recursive backtracking solver
bool solveSudoku(int board[9][9]) {
    int row, col;

    if (!findEmpty(board, row, col))
        return true;

    for (int num = 1; num <= 9; num++) {
        if (isSafe(board, row, col, num)) {
            board[row][col] = num;

            if (solveSudoku(board))
                return true;

            board[row][col] = 0; // Backtrack
        }
    }
    return false;
}

// Print board
void printBoard(int board[9][9]) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

// Main function
int main() {
    int board[9][9] = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };

    cout << "Original Puzzle:" << endl;
    printBoard(board);

    if (solveSudoku(board)) {
        cout << "\nSolution:" << endl;
        printBoard(board);
    } else {
        cout << "No solution!" << endl;
    }

    return 0;
}
