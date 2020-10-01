#include "GeoStruct.h"

using namespace std;

struct GeoBoard {
	int Camera;
	int Panel;
	double X;
	double Y;
	double Rotation;
};

struct MeasurementError
{
	int Panel;
	double MaxX;
	double MaxY;
	double MinX;
	double MinY;
	double AvgX;
	double AvgY;
	double RangeX;
	double RangeY;
};

struct LogBlock
{
	int PanelCount;
	vector<GeoBoard> boards;
	vector<MeasurementsError> offset;
	int TurnSignal;
	int Temp;
	int Moist;
};

