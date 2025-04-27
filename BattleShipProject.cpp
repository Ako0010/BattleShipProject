#include <iostream>
#include <conio.h> // getch funskiyasina gore istifade
#include <thread>  // vaxta gore istifade
#include <chrono>  // vaxta gore istifade
#include <iomanip> // setw() gore istifade
using namespace std;
#include "Battleshipclassandfunctions.h"

int main()
{
	srand(static_cast<unsigned int>(time(0)));
	displayMenu();
	return 0;
}
