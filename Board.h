#pragma once

namespace Battleship
{
    class Board {
    private:
        vector<vector<char>>* grid;

    public:
        Board() {
            try {
                grid = new vector<vector<char>>(BOARD_SIZE, vector<char>(BOARD_SIZE, EMPTY_CELL));
            }
            catch (const bad_alloc& e) {
                cerr << "Memory allocation failed: " << e.what() << endl;
                throw;
            }
        }

        ~Board() {
            delete grid;
        }

        void display(bool hideShips = true, int cursorRow = -1, int cursorCol = -1, char currentShip = ' ') const {
            cout << "   ";
            for (int i = 0; i < BOARD_SIZE; i++) {
                cout << "  " << char('A' + i) << "";
            }
            cout << endl;

            cout << "   " << char(254) << string(BOARD_SIZE * 3, char(254)) << char(254) << endl;

            for (int i = 0; i < BOARD_SIZE; i++) {
                cout << setw(2) << i + 1 << " " << char(221);
                for (int j = 0; j < BOARD_SIZE; j++) {
                    if (i == cursorRow && j == cursorCol) {
                        cout << "[" << currentShip << "]";
                    }
                    else {
                        char cell = (*grid)[i][j];
                        if (hideShips && cell == SHIP_CELL) {
                            cell = EMPTY_CELL;
                        }
                        if (cell == SHIP_CELL) {
                            cout << "\033[34m " << cell << " \033[0m";
                        }
                        else if (cell == HIT_CELL) {
                            cout << "\033[32m " << cell << " \033[0m";
                        }
                        else if (cell == MISS_CELL) {
                            cout << "\033[31m " << cell << " \033[0m";
                        }
                        else {
                            cout << "\033[47m " << cell << " \033[0m";
                        }
                    }
                }
                cout << char(221) << endl;
            }

            cout << "   " << char(254) << string(BOARD_SIZE * 3, char(254)) << char(254) << endl;
        }

        bool placeShip(int row, int col, ShipType type, char orientation) {
            int length = static_cast<int>(type);

            try {
                if (orientation == HORIZONTAL) {
                    if (col + length > BOARD_SIZE) return false;
                }
                else if (orientation == VERTICAL) {
                    if (row + length > BOARD_SIZE) return false;
                }
                else {
                    throw invalid_argument("Invalid orientation. Must be 'H' or 'V'.");
                }

                for (int i = 0; i < length; i++) {
                    int r = (orientation == HORIZONTAL) ? row : row + i;
                    int c = (orientation == HORIZONTAL) ? col + i : col;

                    if ((*grid)[r][c] != EMPTY_CELL) return false;

                    for (int dr = -1; dr <= 1; dr++) {
                        for (int dc = -1; dc <= 1; dc++) {
                            int nr = r + dr;
                            int nc = c + dc;
                            if (nr >= 0 && nr < BOARD_SIZE && nc >= 0 && nc < BOARD_SIZE) {
                                if ((*grid)[nr][nc] == SHIP_CELL) {
                                    return false;
                                }
                            }
                        }
                    }
                }

                for (int i = 0; i < length; i++) {
                    if (orientation == HORIZONTAL) {
                        (*grid)[row][col + i] = SHIP_CELL;
                    }
                    else {
                        (*grid)[row + i][col] = SHIP_CELL;
                    }
                }

            }
            catch (const invalid_argument& e) {
                cerr << "Error placing ship: " << e.what() << endl;
                return false;
            }
            catch (const exception& e) {
                cerr << "Unexpected error: " << e.what() << endl;
                return false;
            }

            return true;
        }

        bool allShipsSunk() const {
            for (int i = 0; i < BOARD_SIZE; i++) {
                for (int j = 0; j < BOARD_SIZE; j++) {
                    if ((*grid)[i][j] == SHIP_CELL) {
                        return false;
                    }
                }
            }
            return true;
        }

        bool attack(int row, int col) {
            try {
                if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
                    throw out_of_range("Attack coordinates are out of bounds.");
                }

                if ((*grid)[row][col] == SHIP_CELL) {
                    (*grid)[row][col] = HIT_CELL;
                    return true;
                }
                else if ((*grid)[row][col] == EMPTY_CELL) {
                    (*grid)[row][col] = MISS_CELL;
                    return false;
                }
            }
            catch (const out_of_range& e) {
                cerr << "Error attacking: " << e.what() << endl;
            }
            catch (const exception& e) {
                cerr << "Unexpected error: " << e.what() << endl;
            }

            return false;
        }

        bool isAlreadyAttacked(int row, int col) const {
            return (*grid)[row][col] == HIT_CELL || (*grid)[row][col] == MISS_CELL;
        }
    };
}