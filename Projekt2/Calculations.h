#pragma once
#include "GeoStruct.h"

// PI Constant withoud needing to include cmath.h and with much higher precision
double get_PI();

std::vector<GeoBoard> sortBoards(std::map<int, GeoBoard> boardMap);
void calcRightNeighbour(std::map<int, BoardProperties> &coordinates, std::vector<LogBlock> &blocks);
void calcCoordinates(std::map<int, BoardProperties> &boards);
double calcDistanceBetweenBoards(GeoBoard board1, GeoBoard board2);
double calcDistance(double X, double Y);
void normalizeCoordinates(std::vector<LogBlock>& blocks);
void calcDivergence(std::vector<LogBlock>& blocks);
void calcPosKornknecht(std::vector<FakeLogBlock>& FakeBoards, std::map<int, BoardProperties>& Coordinates);