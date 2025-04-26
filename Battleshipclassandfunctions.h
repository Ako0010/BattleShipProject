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


class Player {
private:
    Board board;
    string name;
    ShipType currentShipType;
    int cursorRow, cursorCol;
    char orientation;
    bool attackedPositions[BOARD_SIZE][BOARD_SIZE];

public:
    Player(string playerName) : name(playerName), currentShipType(SINGLE), cursorRow(0), cursorCol(0), orientation(HORIZONTAL) {
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                attackedPositions[i][j] = false;
            }
        }
    }

    Board& getBoard() {
        return board;
    }

    string getName() const {
        return name;
    }

    void changeShipType(char key) {
        if (key == 'Q') currentShipType = SINGLE;
        else if (key == 'E') currentShipType = DOUBLE;
        else if (key == 'R') currentShipType = TRIPLE;
    }

    void toggleOrientation() {
        orientation = (orientation == HORIZONTAL) ? VERTICAL : HORIZONTAL;
    }

    void placeShips() {
        int shipsToPlace[3] = { 2, 2, 2 };
        int currentShipIndex = 0;

        while (currentShipIndex < 6) {
            currentShipType = (currentShipIndex < 2) ? SINGLE : (currentShipIndex < 4) ? DOUBLE : TRIPLE;
            if (shipsToPlace[currentShipType - 1] == 0) {
                currentShipIndex++;
                continue;
            }

            while (true) {
                system("cls");
                cout << name << ", place your ships (" << (6 - currentShipIndex) << " left)." << endl;
                cout << "Arrow keys: Move | Space: Toggle orientation | Enter: Place ship" << endl;
                cout << "Q: Single | E: Double | R: Triple" << endl;
                cout << "Current ship: ";
                if (currentShipType == SINGLE) cout << "SINGLE (1-cell)";
                else if (currentShipType == DOUBLE) cout << "DOUBLE (2-cells)";
                else cout << "TRIPLE (3-cells)";
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
                else if (key == 'Q' || key == 'E' || key == 'R') {
                    changeShipType(key);
                }
            }
        }
    }

    virtual bool attackEnemy(Board& enemyBoard) {
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
                if (attackedPositions[cursorRow][cursorCol]) {
                    cout << "You've already attacked this position. Please choose a different one.\n";
                    system("pause");
                    continue;
                }

                bool hit = enemyBoard.attack(cursorRow, cursorCol);
                cout << (hit ? "Hit!" : "Miss!") << endl;
                attackedPositions[cursorRow][cursorCol] = true;

                system("pause");
                return hit;
            }
        }
    }
};


class ComputerPlayer : public Player {
public:
    ComputerPlayer(string name) : Player(name) {}

    void placeShips(int gameMode, bool silent = false) {
        srand(time(0));
        int shipsToPlace[3] = { 2, 2, 2 };
        int currentShipIndex = 0;

        while (currentShipIndex < 6) {
            ShipType type = (currentShipIndex < 2) ? SINGLE : (currentShipIndex < 4) ? DOUBLE : TRIPLE;
            if (shipsToPlace[type - 1] == 0) {
                currentShipIndex++;
                continue;
            }

            int row = rand() % BOARD_SIZE;
            int col = rand() % BOARD_SIZE;
            char orientation = (rand() % 2 == 0) ? 'H' : 'V';

            if (!silent) {
                if (gameMode == 1) {
                    cout << getName() << " (Computer) is trying to place a ";
                }
                else {
                    cout << getName() << " is trying to place a ";
                }
                cout << getName() << " is trying to place a ";
                if (type == SINGLE) cout << "SINGLE";
                else if (type == DOUBLE) cout << "DOUBLE";
                else cout << "TRIPLE";
                cout << " ship at (" << row + 1 << ", " << char('A' + col) << ") " << orientation << endl;
            }

            if (getBoard().placeShip(row, col, type, orientation)) {
                shipsToPlace[type - 1]--;
                currentShipIndex++;
                if (!silent) {
                    if (gameMode == 1) {
                        cout << "Computer successfully placed ship at ";
                    }
                    else {
                        cout << "Ship placed successfully at ";
                    }
                    cout << "Ship placed successfully at ";
                    int length = static_cast<int>(type);
                    for (int i = 0; i < length; i++) {
                        if (orientation == 'H') {
                            cout << "(" << row + 1 << ", " << char('A' + col + i) << ") ";
                        }
                        else {
                            cout << "(" << row + 1 + i << ", " << char('A' + col) << ") ";
                        }
                    }
                }

                if (gameMode == 2) {
                    if (!silent) {
                        cout << "\nCurrent board (computer's ships hidden):\n";
                        getBoard().display(true);
                        cout << endl;
                    }
                }
                else {
                    if (!silent) {
                        cout << "\nCurrent board (ships visible):\n";
                        getBoard().display(false);
                        cout << endl;
                    }
                }

                this_thread::sleep_for(chrono::milliseconds(1500));
                system("cls");
            }
            else if (!silent) {
                cout << "Failed to place the ship. Current board:\n";
                getBoard().display(true);
                cout << "Trying again...\n\n";
                this_thread::sleep_for(chrono::milliseconds(1000));
            }
        }

        if (!silent) {
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

    }

    bool attackEnemy(Board& enemyBoard) override {
        int row, col;
        do {
            row = rand() % BOARD_SIZE;
            col = rand() % BOARD_SIZE;
        } while (enemyBoard.isAlreadyAttacked(row, col));

        cout << getName() << " attacks (" << row + 1 << ", " << char('A' + col) << ") ";
        bool hit = enemyBoard.attack(row, col);
        cout << (hit ? "Hit!" : "Miss!") << endl;
        this_thread::sleep_for(chrono::milliseconds(1000));
        return hit;
    }


};
