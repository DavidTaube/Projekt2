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
	int Panel1;
	int Panel2;
	double Dist;
	int Counter;
};

struct panelNeighbours 
{
	int Panel;
	int Neighbor1;
	int Neighbor2;
	double Distance1;
	double Distance2;
};