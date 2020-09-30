#pragma once
#include <vector> 

struct GeoBoard {
	int Camera;
	int Panel;
	double X;
	double Y;
	double Rotation;
};

struct LogBlock
{
	int PanelCount;
	std::vector<GeoBoard> boards;
	int TurnSignal;
	int Temp;
	int Moist;
};

struct BoardProperties {
	int rightNeighbour;
	double distNeighbour;
	double X, Y;
};