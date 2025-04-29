#pragma once

namespace Battleship
{
    class ComputerPlayer : public Player {
    public:
        ComputerPlayer(string name) : Player(name) {}

        void placeShips(int gameMode, bool silent = false) {
            srand(time(0));
            vector<int> shipsToPlace = { 4, 3, 2, 1 };
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

                try {
                    if (getBoard().placeShip(row, col, type, orientation)) {
                        shipsToPlace[type - 1]--;
                        currentShipIndex++;

                        if (!silent) {
                            if (gameMode == 1) {
                                cout << getName() << " (Computer) successfully placed a ";
                            }
                            else {
                                cout << getName() << " successfully placed a ";
                            }

                            if (type == SINGLE) cout << "SINGLE";
                            else if (type == DOUBLE) cout << "DOUBLE";
                            else if (type == TRIPLE) cout << "TRIPLE";
                            else cout << "QUAD";

                            if (gameMode == 2) {
                                cout << "\nCurrent board (computer's ships hidden):\n";
                                getBoard().display(true);
                                cout << endl;
                            }
                            else {
                                cout << "\nCurrent board (ships visible):\n";
                                getBoard().display(false);
                                cout << endl;
                            }
                        }

                        this_thread::sleep_for(chrono::milliseconds(1500));
                        system("cls");
                    }
                }
                catch (const exception& e) {
                    cerr << "Error placing ship: " << e.what() << endl;
                    continue;
                }
            }

            if (!silent) {
                try {
                    if (gameMode == 2) {
                        cout << getName() << " has finished placing all ships.\nFinal board (computer's ships hidden):\n";
                        getBoard().display(true);
                        cout << endl;
                    }
                    else {
                        cout << getName() << " has finished placing all ships.\nFinal board (ships visible):\n";
                        getBoard().display(false);
                        cout << endl;
                    }
                    this_thread::sleep_for(chrono::milliseconds(2000));
                }
                catch (const exception& e) {
                    cerr << "Error displaying board: " << e.what() << endl;
                }
            }
        }

        bool attackEnemy(Board& enemyBoard) override {
            int row, col;

            try {
                do {
                    row = rand() % BOARD_SIZE;
                    col = rand() % BOARD_SIZE;
                } while (enemyBoard.isAlreadyAttacked(row, col));

                cout << getName() << " attacks (" << row + 1 << ", " << char('A' + col) << ") ";
                bool hit = enemyBoard.attack(row, col);
                cout << (hit ? "Hit!" : "Miss!") << endl;
                this_thread::sleep_for(chrono::milliseconds(1500));

                return hit;
            }
            catch (const exception& e) {
                cerr << "Error during attack: " << e.what() << endl;
                return false;
            }
        }
    };
}