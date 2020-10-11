#pragma once
#include "GeoStruct.h"

void GetDuplicatePanels(std::vector<GeoBoard> &Boards);
double GetMaximumPoint(std::vector<GeoBoard> &Boards, int panelNr, bool isX);
double GetMinimumPoint(std::vector<GeoBoard> &Boards, int panelNr, bool isX);
double GetAveragePoint(std::vector<GeoBoard> &Boards, int panelNr, bool isX);
double GetErrorRange(std::vector<GeoBoard> &Boards, int panelNr);
void GetMeasurementsErrors(std::vector<LogBlock> &GeoStruct);
void PrintMeasurementsErrors(std::vector<LogBlock>& GeoStruct);