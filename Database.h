#pragma once

namespace Battleship
{
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
}