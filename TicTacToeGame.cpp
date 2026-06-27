#include <iostream>
#include <vector>
using namespace std;
void displayMatchBoard(const vector<vector<char>>& grid);
bool verifyVictory(const vector<vector<char>>& grid, char token);
bool verifyTieMatch(const vector<vector<char>>& grid);
void executeGameSession();

int main() {
    char replayDecision;
    do {
        executeGameSession();
        cout << "\nWould you like to play another match? (y/n): ";
        cin >> replayDecision;
    } while (replayDecision == 'y' || replayDecision == 'Y');

    cout << "\nThank you for playing! Exiting the game module.\n";
    return 0;
}

void displayMatchBoard(const vector<vector<char>>& grid) {
    cout << "\n  Current Board State:\n";
    cout << "     1     2     3  \n";
    cout << "  +-----+-----+-----+\n";
    for (int r = 0; r < 3; ++r) {
        cout << r + 1 << " |  " << grid[r][0] << "  |  " << grid[r][1] << "  |  " << grid[r][2] << "  |\n";
        cout << "  +-----+-----+-----+\n";
    }
}

bool verifyVictory(const vector<vector<char>>& grid, char token) {
    for (int i = 0; i < 3; ++i) {
        if (grid[i][0] == token && grid[i][1] == token && grid[i][2] == token) return true;
        if (grid[0][i] == token && grid[1][i] == token && grid[2][i] == token) return true;
    }

    if (grid[0][0] == token && grid[1][1] == token && grid[2][2] == token) return true;
    if (grid[0][2] == token && grid[1][1] == token && grid[2][0] == token) return true;

    return false;
}

bool verifyTieMatch(const vector<vector<char>>& grid) {
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            if (grid[r][c] == ' ') return false;
        }
    }
    return true;
}

void executeGameSession() {
    vector<vector<char>> arenaGrid(3, vector<char>(3, ' '));
    char currentActiveToken = 'X';
    bool sessionRunning = true;

    cout << "\n=========================================\n";
    cout << "         TIC TAC TOE MINI GAME           \n";
    cout << "=========================================\n";
    cout << "Player 1: [X]  |  Player 2: [O]\n";

    while (sessionRunning) {
        displayMatchBoard(arenaGrid);
        int targetedRow, targetedCol;

        cout << "\nActive Turn [" << currentActiveToken << "]\n";
        cout << "Enter Row (1-3) and Column (1-3) separated by space: ";

        if (!(cin >> targetedRow >> targetedCol)) {
            cout << "Invalid input! Please enter numbers only.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        targetedRow--;
        targetedCol--;

        if (targetedRow < 0 || targetedRow > 2 || targetedCol < 0 || targetedCol > 2) {
            cout << "Out of bounds! Choose coordinates between 1 and 3.\n";
            continue;
        }

        if (arenaGrid[targetedRow][targetedCol] != ' ') {
            cout << "That slot is already occupied! Try a different move.\n";
            continue;
        }

        arenaGrid[targetedRow][targetedCol] = currentActiveToken;

        if (verifyVictory(arenaGrid, currentActiveToken)) {
            displayMatchBoard(arenaGrid);
            cout << "\n🎉 Congratulations! Player [" << currentActiveToken << "] wins the match! 🎉\n";
            sessionRunning = false;
        }
        else if (verifyTieMatch(arenaGrid)) {
            displayMatchBoard(arenaGrid);
            cout << "\n🤝 Match ended in a draw! Exceptional defense from both sides.\n";
            sessionRunning = false;
        }
        else {
            currentActiveToken = (currentActiveToken == 'X') ? 'O' : 'X';
        }
    }
}
