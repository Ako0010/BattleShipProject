#pragma once

const int BOARD_SIZE = 10;
const char EMPTY_CELL = '#';
const char SHIP_CELL = 'S';
const char HIT_CELL = 'X';
const char MISS_CELL = 'M';


enum ShipType
{
    SINGLE = 1,
    DOUBLE = 2,
    TRIPLE = 3,
	QUAD = 4,
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
                    char cell = grid[i][j];
                    if (hideShips && cell == SHIP_CELL) {
                        cell = EMPTY_CELL;
                    }
                    if (cell == SHIP_CELL) {
                        cout << "\033[34m " << cell << " \033[0m";
                    }
                    else if (cell == 'X') {
                        cout << "\033[31m " << cell << " \033[0m";
                    }
                    else if (cell == 'M') {
                        cout << "\033[33m " << cell << " \033[0m";
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


    void toggleOrientation() {
        orientation = (orientation == HORIZONTAL) ? VERTICAL : HORIZONTAL;
    }

    void placeShips(bool autoPlace = false) {

        if (autoPlace) {
            srand(time(0));
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

                if (board.placeShip(row, col, type, orientation)) {
                    shipsToPlace[type - 1]--;
                    currentShipIndex++;
                }
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
            else {
                continue;
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
        this_thread::sleep_for(chrono::milliseconds(2000));
        return hit;
    }


};

void displayMenu() {
    int selectedOption = 0;
    const int totalOptions = 2;

    while (true) {
        system("cls");
        cout << "=== Battleship Menu ===\n";
        cout << (selectedOption == 0 ? "1. Start Game <-" : "1. Start Game   ") << "\n";
        cout << (selectedOption == 1 ? "2. Exit <-" : "2. Exit   ") << "\n";

        char key = _getch();
        if (key == 'W' || key == 'w') {
            selectedOption = (selectedOption - 1 + totalOptions) % totalOptions;
        }
        else if (key == 'S' || key == 's') {
            selectedOption = (selectedOption + 1) % totalOptions;
        }

        else if (key == '\r') {
            if (selectedOption == 0) {
                int gameMode = 0;
                while (true) {
                    system("cls");
                    cout << "Select Game Mode:\n";
                    cout << "1. Player vs Player" << (gameMode == 0 ? " <-" : "   ") << "\n";
                    cout << "2. Computer vs Computer" << (gameMode == 1 ? " <-" : "   ") << "\n";
                    cout << "3. Player vs Computer" << (gameMode == 2 ? " <-" : "   ") << "\n";


                    key = _getch();
                    if (key == 27) {
                        break;
                    }
                    if (key == 'W' || key == 'w') {
                        gameMode = (gameMode - 1 + 3) % 3;
                    }
                    else if (key == 'S' || key == 's') {
                        gameMode = (gameMode + 1) % 3;
                    }


                    else if (key == '\r') {
                        if (gameMode == 0) {
                            string player1Name = "Player 1";
                            string player2Name = "Player 2";

                            cout << "Player 1, do you want to set a custom name? (y/n): ";
                            char setName1;
                            cin >> setName1;
                            if (setName1 == 'y' || setName1 == 'Y') {
                                cout << "Enter name for Player 1: ";
                                cin >> player1Name;
                            }

                            cout << "Player 2, do you want to set a custom name? (y/n): ";
                            char setName2;
                            cin >> setName2;
                            if (setName2 == 'y' || setName2 == 'Y') {
                                cout << "Enter name for Player 2: ";
                                cin >> player2Name;
                            }

                            cout << "Does Player 1 want to auto-place ships? (y/n): ";
                            char autoPlace1;
                            cin >> autoPlace1;

                            cout << "Does Player 2 want to auto-place ships? (y/n): ";
                            char autoPlace2;
                            cin >> autoPlace2;

                            Player player1(player1Name);
                            Player player2(player2Name);
                            player1.placeShips(autoPlace1 == 'y' || autoPlace1 == 'Y');
                            player2.placeShips(autoPlace2 == 'y' || autoPlace2 == 'Y');

                            while (true) {
                                system("cls");
                                cout << player1.getName() << "'s turn:" << endl;
                                if (player1.attackEnemy(player2.getBoard())) {
                                    cout << "Hit!" << endl;
                                }
                                else {
                                    cout << "Miss!" << endl;
                                }
                                if (player2.getBoard().allShipsSunk()) {
                                    cout << player1.getName() << " wins!" << endl;
                                    break;
                                }

                                system("cls");
                                cout << player2.getName() << "'s turn:" << endl;
                                if (player2.attackEnemy(player1.getBoard())) {
                                    cout << "Hit!" << endl;
                                }
                                else {
                                    cout << "Miss!" << endl;
                                }
                                if (player1.getBoard().allShipsSunk()) {
                                    cout << player2.getName() << " wins!" << endl;
                                    break;
                                }
                            }
                        }
                        else if (gameMode == 1) {
                            ComputerPlayer computer1("Computer 1");
                            ComputerPlayer computer2("Computer 2");
                            computer1.placeShips(false);
                            computer2.placeShips(false);

                            while (true) {
                                system("cls");
                                cout << "Computer 1's Board:\n";
                                computer1.getBoard().display(true);
                                cout << "\nComputer 2's Board:\n";
                                computer2.getBoard().display(true);
                                computer1.attackEnemy(computer2.getBoard());
                                if (computer2.getBoard().allShipsSunk()) {
                                    cout << "Computer 1 wins!\n";
                                    system("pause");
                                    break;
                                }
                                system("cls");
                                cout << "Computer 1's Board:\n";
                                computer1.getBoard().display(true);
                                cout << "\nComputer 2's Board:\n";
                                computer2.getBoard().display(true);
                                computer2.attackEnemy(computer1.getBoard());
                                if (computer1.getBoard().allShipsSunk()) {
                                    cout << "Computer 2 wins!\n";
                                    system("pause");
                                    break;
                                }
                            }
                        }
                        else if (gameMode == 2) {

                            Player player("Player");
                            ComputerPlayer computer("Computer");
                            player.placeShips();
                            computer.placeShips(gameMode);

                            while (true) {
                                computer.getBoard().display(true);
                                player.attackEnemy(computer.getBoard());
                                if (computer.getBoard().allShipsSunk()) {
                                    system("cls");
                                    cout << "Player wins!\n";
                                    system("pause");
                                    break;
                                }
                                computer.attackEnemy(player.getBoard());
                                if (player.getBoard().allShipsSunk()) {
                                    system("cls");
                                    cout << "Computer wins!\n";
                                    system("pause");
                                    break;
                                }
                            }
                        }
                        break;
                    }
                }
            }
            else if (selectedOption == 1) {
                cout << "Exiting game. Goodbye!\n";
                break;
            }
        }
    }
}
