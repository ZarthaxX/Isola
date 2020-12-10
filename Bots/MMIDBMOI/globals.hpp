#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <random>

#include "types.hpp"
#include "constants.hpp"

int boardW;
int boardH;
int timeLimit;

Moves movesArray[MAX_HEIGHT*MAX_WIDTH];
Removes removesArray[MAX_HEIGHT*MAX_WIDTH];

namespace Zobrist{
    Key playerKey[PLAYERS_NB][MAX_WIDTH*MAX_HEIGHT];
}

void initPlayerKey(){

    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<uint64_t> distribution(0x0ULL,0xFFFFFFFFFFFFFFFFULL); // distribution in range [1, 6]

    for(uint8_t p = 0;p < PLAYERS_NB;p++)
        for(uint8_t s = 0;s < SQUARE_NB;s++)
            Zobrist::playerKey[p][s] = distribution(rng);
}

#endif