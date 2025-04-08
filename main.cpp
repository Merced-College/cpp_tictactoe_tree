#include <iostream>
#include <vector>
#include <limits>
using namespace std;

const char HUMAN = 'X';
const char COMPUTER = 'O';
const char EMPTY = ' ';

class GameState {
private:
    vector<char> board; // 9 cells for Tic-Tac-Toe

public:
    GameState() : board(9, EMPTY) {}

    GameState(const vector<char>& b) : board(b) {}

    void printBoard() const {
        for (int i = 0; i < 9; ++i) {
            cout << (board[i] == EMPTY ? '-' : board[i]) << " ";
            if ((i + 1) % 3 == 0) cout << endl;
        }
    }

    bool isFull() const {
        for (char c : board)
            if (c == EMPTY) return false;
        return true;
    }

    bool isGameOver() const {
        return checkWinner() != EMPTY || isFull();
    }

    char checkWinner() const {
        const int winPatterns[8][3] = {
            {0,1,2}, {3,4,5}, {6,7,8},
            {0,3,6}, {1,4,7}, {2,5,8},
            {0,4,8}, {2,4,6}
        };

        for (auto& pattern : winPatterns) {
            if (board[pattern[0]] != EMPTY &&
                board[pattern[0]] == board[pattern[1]] &&
                board[pattern[1]] == board[pattern[2]]) {
                return board[pattern[0]];
            }
        }
        return EMPTY;
    }

    vector<int> getAvailableMoves() const {
        vector<int> moves;
        for (int i = 0; i < 9; ++i)
            if (board[i] == EMPTY) moves.push_back(i);
        return moves;
    }

    GameState makeMove(int index, char player) const {
        vector<char> newBoard = board;
        newBoard[index] = player;
        return GameState(newBoard);
    }

    const vector<char>& getBoard() const {
        return board;
    }
};

class TicTacToeTree {
public:
    int minimax(const GameState& state, bool isMaximizing) {
        char winner = state.checkWinner();
        if (winner == COMPUTER) return 1;
        if (winner == HUMAN) return -1;
        if (state.isFull()) return 0;

        if (isMaximizing) {
            int bestScore = numeric_limits<int>::min();
            for (int move : state.getAvailableMoves()) {
                GameState newState = state.makeMove(move, COMPUTER);
                int score = minimax(newState, false);
                bestScore = max(bestScore, score);
            }
            return bestScore;
        } else {
            int bestScore = numeric_limits<int>::max();
            for (int move : state.getAvailableMoves()) {
                GameState newState = state.makeMove(move, HUMAN);
                int score = minimax(newState, true);
                bestScore = min(bestScore, score);
            }
            return bestScore;
        }
    }

    int findBestMove(const GameState& state) {
        int bestScore = numeric_limits<int>::min();
        int bestMove = -1;

        for (int move : state.getAvailableMoves()) {
            GameState newState = state.makeMove(move, COMPUTER);
            int score = minimax(newState, false);
            if (score > bestScore) {
                bestScore = score;
                bestMove = move;
            }
        }
        return bestMove;
    }
};

void playGame() {
    GameState state;
    TicTacToeTree ai;
    char currentPlayer = HUMAN;

    while (!state.isGameOver()) {
        state.printBoard();
        if (currentPlayer == HUMAN) {
            int move;
            cout << "Enter your move (0-8): ";
            cin >> move;
            if (move >= 0 && move < 9 && state.getBoard()[move] == EMPTY) {
                state = state.makeMove(move, HUMAN);
                currentPlayer = COMPUTER;
            } else {
                cout << "Invalid move. Try again.\n";
            }
        } else {
            int move = ai.findBestMove(state);
            state = state.makeMove(move, COMPUTER);
            cout << "Computer plays at position " << move << endl;
            currentPlayer = HUMAN;
        }
    }

    state.printBoard();
    char winner = state.checkWinner();
    if (winner == COMPUTER) cout << "Computer wins!\n";
    else if (winner == HUMAN) cout << "You win!\n";
    else cout << "It's a draw!\n";
}

int main() {
    playGame();
    return 0;
}
