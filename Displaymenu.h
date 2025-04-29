#pragma once
namespace Battleship
{
    void displayMenu() {
        int selectedOption = 0;
        const int totalOptions = 2;

        while (true) {
            system("cls");
            cout << "\033[34m"
                << " ______     ______     ______   ______   __         ______     ______     __  __     __     ______  \n"
                << "/\\  == \\   /\\  __ \\   /\\__  _\\ /\\__  _\\ /\\ \\       /\\  ___\\   /\\  ___\\   /\\ \\_\\ \\   /\\ \\   /\\  == \\ \n"
                << "\033[31m"
                << "\\ \\  __<   \\ \\  __ \\  \\/_/\\ \\/ \\/_/\\ \\/ \\ \\ \\____  \\ \\  __\\   \\ \\___  \\  \\ \\  __ \\  \\ \\ \\  \\ \\  _-/ \n"
                << "\033[32m"
                << " \\ \\_____\\  \\ \\_\\ \\_\\    \\ \\_\\    \\ \\_\\  \\ \\_____\\  \\ \\_____\\  \\/\\_____\\  \\ \\_\\ \\_\\  \\ \\_\\  \\ \\_\\   \n"
                << "  \\/_____/   \\/_/\\/_/     \\/_/     \\/_/   \\/_____/   \\/_____/   \\/_____/   \\/_/\\/_/   \\/_/   \\/_/   \n"
                << "\033[0m";


            cout << endl;

            cout << "\t\t\t\t\t" << (selectedOption == 0 ? " Start Game <-" : " Start Game   ") << "\n";
            cout << "\t\t\t\t\t" << (selectedOption == 1 ? " Exit <-" : " Exit   ") << "\n";


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
                        cout << " Player vs Player" << (gameMode == 0 ? " <-" : "   ") << "\n";
                        cout << " Computer vs Computer" << (gameMode == 1 ? " <-" : "   ") << "\n";
                        cout << " Player vs Computer" << (gameMode == 2 ? " <-" : "   ") << "\n";


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
                                srand(time(0));
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
                                        system("pause");
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
                                        system("pause");
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
                                    if (_kbhit() && _getch() == 27) {
                                        break;
                                    }
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
                    system("cls");
                    cout << "Exiting game. Goodbye!\n";
                    break;
                }
            }
        }
    }
}
