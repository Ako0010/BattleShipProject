#pragma once

namespace Battleship
{
   class Player {
    private:
        Board board;
        string name;
        ShipType currentShipType;
        int cursorRow, cursorCol;
        char orientation;
        vector<vector<bool>> attackedPositions;
        vector<bool>* attackedPositionsPointer;

    public:
        Player(string playerName) 
            : name(playerName), currentShipType(SINGLE), cursorRow(0), cursorCol(0), orientation(HORIZONTAL), attackedPositions(BOARD_SIZE, vector<bool>(BOARD_SIZE, false)) 
        {
            try {
                attackedPositionsPointer = &attackedPositions[0];
            }
            catch (const bad_alloc& e) {
                cerr << "Memory allocation failed for attackedPositions: " << e.what() << endl;
                throw;
            }
        }

        Board& getBoard() {
            return board;
        }

        string getName() const {
            return name;
        }

        void toggleOrientation() {
            orientation = (orientation == HORIZONTAL) ? VERTICAL : HORIZONTAL;
        }

        void placeShips(bool autoPlace = false) {
            try {
                if (autoPlace) {
                    int shipsToPlace[4] = { 4, 3, 2, 1 };
                    int currentShipIndex = 0;

                    while (currentShipIndex < 10) {
                        ShipType type = (currentShipIndex < 4) ? SINGLE : (currentShipIndex < 7) ? DOUBLE : (currentShipIndex < 9) ? TRIPLE : QUAD;
                        if (shipsToPlace[type - 1] == 0) {
                            currentShipIndex++;
                            continue;
                        }

                        int row = rand() % BOARD_SIZE;
                        int col = rand() % BOARD_SIZE;
                        char orientation = (rand() % 2 == 0) ? 'H' : 'V';

                        if (!board.placeShip(row, col, type, orientation)) {
                            continue;
                        }
                        shipsToPlace[type - 1]--;
                        currentShipIndex++;
                    }
                }
                else {
                    int shipsToPlace[4] = { 4, 3, 2, 1 };
                    int currentShipIndex = 0;

                    while (currentShipIndex < 10) {
                        currentShipType = (currentShipIndex < 4) ? SINGLE : (currentShipIndex < 7) ? DOUBLE : (currentShipIndex < 9) ? TRIPLE : QUAD;
                        if (shipsToPlace[currentShipType - 1] == 0) {
                            currentShipIndex++;
                            continue;
                        }

                        while (true) {
                            system("cls");
                            cout << name << ", place your ships (" << (10 - currentShipIndex) << " left)." << endl;
                            cout << " | Orientation: " << (orientation == HORIZONTAL ? "Horizontal" : "Vertical") << endl;

                            board.display(false, cursorRow, cursorCol, 'S');

                            char key = _getch();
                            if (key == 'W' || key == 'w') {    
                                if (cursorRow > 0) cursorRow--;
                            }
                            else if (key == 'S' || key == 's') {
                                if (cursorRow < BOARD_SIZE - 1) cursorRow++;
                            }
                            else if (key == 'A' || key == 'a') {
                                if (cursorCol > 0) cursorCol--;
                            }
                            else if (key == 'D' || key == 'd') {
                                if (cursorCol < BOARD_SIZE - 1) cursorCol++;
                            }
                            else if (key == ' ') {
                                toggleOrientation();
                            }
                            else if (key == '\r') {
                                if (board.placeShip(cursorRow, cursorCol, currentShipType, orientation)) {
                                    shipsToPlace[currentShipType - 1]--;
                                    currentShipIndex++;
                                    break;
                                }
                                else {
                                    cout << "Invalid placement (ships cannot be adjacent). Try again." << endl;
                                    system("pause");
                                }
                            }
                        }
                    }
                }
            }
            catch (const exception& e) {
                cerr << "Error placing ships: " << e.what() << endl;
            }
        }

        virtual bool attackEnemy(Board& enemyBoard) {
            try {
                while (true) {
                    system("cls");
                    cout << name << "'s Board:\n";
                    board.display(true);
                    cout << "\n" << name << ", attack " << "Opponent's Board:\n";
                    enemyBoard.display(true, cursorRow, cursorCol, 'A');

                    char key = _getch();
                    if (key == 'W' || key == 'w') {
                        if (cursorRow > 0) cursorRow--;
                    }
                    else if (key == 'S' || key == 's') {
                        if (cursorRow < BOARD_SIZE - 1) cursorRow++;
                    }
                    else if (key == 'A' || key == 'a') {
                        if (cursorCol > 0) cursorCol--;
                    }
                    else if (key == 'D' || key == 'd') {
                        if (cursorCol < BOARD_SIZE - 1) cursorCol++;
                    }
                    else if (key == ' ') {
                        if ((attackedPositionsPointer)[cursorRow][cursorCol]) {
                            cout << "You've already attacked this position. Please choose a different one.\n";
                            system("pause");
                            continue;
                        }

                        bool hit = enemyBoard.attack(cursorRow, cursorCol);
                        cout << (hit ? "Hit!" : "Miss!") << endl;
                        (attackedPositionsPointer)[cursorRow][cursorCol] = true;

                        return hit;
                    }
                }
            }
            catch (const exception& e) {
                cerr << "Error attacking the enemy: " << e.what() << endl;
                return false;
            }
        }
    };
}
