#pragma once
#include "Calculations.h"
#include "GeoStruct.h"

void DEBUG_getBlocks();
void DEBUG_getBoardPositions();
bool getFileContent(std::string fileName, std::vector<std::string>& vecOfStrs);
bool GetBlocks();
bool init();
GeoBoard getArtificialGeoBoard();
