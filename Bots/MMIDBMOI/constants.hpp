#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <cstdint>
#include <limits.h>


constexpr int MAX_WIDTH = 8;
constexpr int MAX_HEIGHT = 8;

const uint8_t PLAYERS_NB = 2;
const uint8_t SQUARE_NB = MAX_WIDTH*MAX_HEIGHT;

const uint32_t DEPTH_LIMIT = SQUARE_NB+2;
const uint32_t INITIAL_DEPTH = 1;
const uint32_t TRANSPOSITION_TABLE_SIZE = (1ULL<<16);
const uint32_t KILLER_MOVES = 2;


constexpr int INF = INT_MAX;
constexpr int LOSS = -1000000;
constexpr int WIN = -LOSS;

#endif