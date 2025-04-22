#pragma once

const int BOARD_SIZE = 10;
const char EMPTY_CELL = '.';
const char SHIP_CELL = 'S';
const char HIT_CELL = 'X';
const char MISS_CELL = 'M';


enum ShipType
{
    SINGLE = 1,
    DOUBLE = 2,
    TRIPLE = 3
};

enum Orientation
{
    HORIZONTAL = 'H',
    VERTICAL = 'V'
};


class Board {
private:
    char grid[BOARD_SIZE][BOARD_SIZE];

public:
    Board() {
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                grid[i][j] = EMPTY_CELL;
            }
        }
    }

    void display(bool hideShips = true, int cursorRow = -1, int cursorCol = -1, char currentShip = ' ') const {
        cout << "    ";
        for (int i = 0; i < BOARD_SIZE; i++) {
            cout << char('A' + i) << " ";
        }
        cout << endl;

        for (int i = 0; i < BOARD_SIZE; i++) {
            cout << (i + 1 < 10 ? " " : "") << i + 1 << " ";
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (i == cursorRow && j == cursorCol) {
                    cout << "[" << currentShip << "]";
                }
                else {
                    if (hideShips && grid[i][j] == SHIP_CELL) {
                        cout << EMPTY_CELL << " ";
                    }
                    else {
                        cout << grid[i][j] << " ";
                    }
                }
            }
            cout << endl;
        }
    }

    bool placeShip(int row, int col, ShipType type, char orientation) {
        int length = static_cast<int>(type);


        if (orientation == HORIZONTAL) {
            if (col + length > BOARD_SIZE) return false;
        }
        else if (orientation == VERTICAL) {
            if (row + length > BOARD_SIZE) return false;
        }
        else {
            return false;
        }


        for (int i = 0; i < length; i++) {
            int r = (orientation == HORIZONTAL) ? row : row + i;
            int c = (orientation == HORIZONTAL) ? col + i : col;


            if (grid[r][c] != EMPTY_CELL) return false;

            for (int dr = -1; dr <= 1; dr++) {
                for (int dc = -1; dc <= 1; dc++) {
                    int nr = r + dr;
                    int nc = c + dc;
                    if (nr >= 0 && nr < BOARD_SIZE && nc >= 0 && nc < BOARD_SIZE) {
                        if (grid[nr][nc] == SHIP_CELL) {
                            return false;
                        }
                    }
                }
            }
        }


        for (int i = 0; i < length; i++) {
            if (orientation == HORIZONTAL) {
                grid[row][col + i] = SHIP_CELL;
            }
            else {
                grid[row + i][col] = SHIP_CELL;
            }
        }
        return true;
    }


    bool allShipsSunk() const {
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (grid[i][j] == SHIP_CELL) {
                    return false;
                }
            }
        }
        return true;
    }


    bool attack(int row, int col) {
        if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) return false;
        if (grid[row][col] == SHIP_CELL) {
            grid[row][col] = HIT_CELL;
            return true;
        }
        else if (grid[row][col] == EMPTY_CELL) {
            grid[row][col] = MISS_CELL;
            return false;
        }
        return false;
    }

    bool isAlreadyAttacked(int row, int col) const {
        return grid[row][col] == 'X' || grid[row][col] == 'M';
    }

};

