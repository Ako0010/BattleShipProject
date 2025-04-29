#include <iostream>
#include <conio.h> // getch funskiyasina gore istifade
#include <thread>  // Thread, bir proqramda eyni vaxtda bir nece tapsiriqin paralel icra olunmasini temin eden mexanizmdir.  vaxta gore istifade
#include <chrono>  // vaxta gore istifade
#include <iomanip> // setw() gore istifade
#include <vector> 
using namespace std;

#include "Database.h"
#include "Board.h"
#include "Player.h"
#include "Computerplayer.h"
#include "Displaymenu.h"

using namespace Battleship;

int main()
{
	srand(static_cast<unsigned int>(time(0)));
	displayMenu();
	return 0;
}
