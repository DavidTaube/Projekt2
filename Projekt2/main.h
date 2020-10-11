#pragma once
#include "Calculations.h"
#include "MeasurementsErrors.h"

struct fakeData {
	int rightNeighbour;
	double dist;
};

void insertFakeData(std::map<int, fakeData>& fakedata);
void DEBUG_getBlocks();
void DEBUG_getBoardPositions();
bool getFileContent(std::string fileName, std::vector<std::string>& vecOfStrs);
bool GetBlocks();
bool init();