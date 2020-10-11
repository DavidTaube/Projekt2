#pragma once
#include <vector> 

struct GeoBoard {
	int Camera;
	int Panel;
	double X;
	double Y;
	double Rotation;
};

struct MeasurementError {
	int Panel;
	double MaxX;
	double MaxY;
	double MinX;
	double MinY;
	double AvgX;
	double AvgY;
	double Range;
};

struct LogBlock
{
	int PanelCount;
	std::vector<GeoBoard> boards;
	std::vector<MeasurementError> offset;
	int TurnSignal;
	int Temp;
	int Moist;
};

struct BoardProperties {
	int rightNeighbour;
	double distNeighbour;
	double X, Y;
};