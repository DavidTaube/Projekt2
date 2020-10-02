#include "GeoStruct.h"

using namespace std;

struct GeoBoard 
{
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
	int dir; // 0 if panel1 is left of panel2, 1 vice versa
};


struct panelNeighbours
{
	int Panel;
	int NeighbourL = 0;
	int NeighbourR = 0;
	double DistanceL = 1000.0;
	double DistanceR = 1000.0;
	int AngleL = 3; // 0 if panel is left of n1, 1 vice versa
	int AngleR = 3; // 0 if panel is left of n2, 1 vice versa
};