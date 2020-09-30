#pragma once
#include "GeoStruct.h"

// PI Constant withoud needing to include cmath.h and with much higher precision
double get_PI();

void calcCoordinates(std::map<int, BoardProperties> boards);
double calcDistanceBetweenBoards(GeoBoard board1, GeoBoard board2);
double calcDistance(double X, double Y);