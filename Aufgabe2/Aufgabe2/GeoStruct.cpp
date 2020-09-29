#include "GeoStruct.h"

using namespace std;

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
	vector<GeoBoard> boards;
	int TurnSignal;
	int Temp;
	int Moist;
};


struct markerDist
{
	int marker1;
	int marker2;
	double dist;
};