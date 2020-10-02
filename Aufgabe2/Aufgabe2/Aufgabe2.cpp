#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "GeoStruct.cpp"
#include <sstream>
#include <math.h>

#define PI 3.14159265358979323846

std::vector<LogBlock> Boards;
std::vector<std::string> LogFile;
std::vector<markerDist> Dist;
std::vector<panelNeighbours> NearestNeighbours;
std::vector<int> PanelNrs;


// read in the LogFile and push all lines into a
bool getFileContent(std::string fileName, std::vector<std::string>& vecOfStrs) {
	// read in file
	std::ifstream in(fileName.c_str());
	// throw error if file does not exist
	if (!in) {
		std::cerr << "Cannot open the File : " << fileName << std::endl;
		return false;
	}
	std::string str;
	//read in new line from file
	while (std::getline(in, str)) {
		//ignore newlines
		if (str.size() > 0)
			// push string into vector
			vecOfStrs.push_back(str);
	}
	// close file to prevent memory leaks
	in.close();
	return true;
}

// convert the strings in the vector LogFile to structs
bool GetBlocks() {
	int line = 0;
	while (true) {
		LogBlock block;
		block.PanelCount = stoi(LogFile.at(line));
		
		line++;
		for (int x = 0; x < block.PanelCount; x++) {
			GeoBoard board;
			board.Camera = stoi(LogFile.at(line++));
			board.Panel = stoi(LogFile.at(line++));
			board.X = stod(LogFile.at(line++));
			board.Y = stod(LogFile.at(line++));
			board.Rotation = stod(LogFile.at(line++));
			block.boards.push_back(board);
		}	
		block.TurnSignal = stoi(LogFile.at(line++));

		if (line + 1 >= LogFile.size()) {
			return true;
		}
		if (LogFile.at(line) == "-1000") {
			line++;
			block.Temp = stoi(LogFile.at(line++));
			block.Moist = stoi(LogFile.at(line++));
		}

		Boards.push_back(block);

		if (line + 5 >= LogFile.size()) {
			return true;
		}
	}
}

// check, if the combination of two markers is already in Dist-vector
// if so, return index of position in Dist
int checkPreviousOccurence(int marker1, int marker2) {
	// iterate through Dist-vector
	for (int i = 1; i <= Dist.size(); i++) {
		int dpanel1 = Dist[i-1].Panel1;
		int dpanel2 = Dist[i-1].Panel2;
		
		//cout << "m1 " << marker1 << " m2 " << marker2 << "\n";
		//cout << "d1 " << dpanel1 << " d2 " << dpanel2 << "\n";

		// if combination of markers is found, return index of Dist
		if ((marker1 == dpanel1) && (marker2 == dpanel2)) return i;
		else if ((marker2 == dpanel1) && (marker1 == dpanel2)) return i;
	}
	return 0;
}

// calculate angle of two markers to y-axis
// if geo1 is left of geo2, return 0
bool calcAngles(GeoBoard geo1, GeoBoard geo2) {
	double angle[2];
	GeoBoard geo[2]{ geo1, geo2 };
	for (int i = 0; i < 2; i++) {
		// calculate angle if camera1
		if (geo[i].Camera) {
			angle[i] = atan(geo[i].X / geo[i].Y);
		} // calculate angle if camera0
		else {
			angle[i] = 120 - atan(geo[i].X / geo[i].Y);
		}
	}

	// return 0 if geo1 is left of geo2, 1 vice versa
	if (angle[0] < angle[1]) return 0;
}

// calculate average marker distance and angle between two markers
// save into Dist-vector
bool calculateMarkerDist(GeoBoard geo1, GeoBoard geo2) {

	double x1, x2, y1, y2, d1, d2, winkel1, winkel2, vorzeichen;

	x1 = geo1.X;
	y1 = geo1.Y;

	x2 = geo2.X;
	y2 = geo2.Y;

	// if markerposition is from different cameras, take different angle into account
	if (geo1.Camera != geo2.Camera) {

		vorzeichen = (float)(geo1.Camera < geo2.Camera ? -1 : 1);

		d1 = sqrt(x1 * x1 + y1 * y1);
		winkel1 = atan(y1 / x1) + vorzeichen * 60 / 2 * PI / 180.0; // PI festlegen
		x1 = d1 * cos(winkel1);
		y1 = d1 * sin(winkel1);

		d2 = sqrt(x2 * x2 + y2 * y2);
		winkel2 = atan(y2 / x2) - vorzeichen * 60 / 2 * PI / 180.0; // PI festlegen
		x2 = d2 * cos(winkel2);
		y2 = d2 * sin(winkel2);
	}

	// get distance between markers
	d1 = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));

	// call function to calculate angles
	int dir = calcAngles(geo1, geo2);

	// call function to check, if marker combination has already been calculated
	int index = checkPreviousOccurence(geo1.Panel, geo2.Panel);
	// cout << "index: " << index << "\n";

 	// calculate average distance if distance has already been calculated
	if ( index ) {
		Dist[index-1].Dist = (Dist[index-1].Counter * Dist[index-1].Dist + d1) / (Dist[index-1].Counter + 1);
		Dist[index-1].Counter = Dist[index-1].Counter + 1;
		// cout << "I have seen this.\n";
	}
	// if distance has not been calculated, push new object into Dist
	else {
		markerDist markerDistance;
		markerDistance.Panel1 = geo1.Panel;
		markerDistance.Panel2 = geo2.Panel;
		markerDistance.Dist = d1;
		markerDistance.Counter = 1;
		markerDistance.dir = dir;

		Dist.push_back(markerDistance);
	}
	
	// cout << "Distance between " << geo1.Panel << " and " << geo2.Panel << " is " << d1 << "\n";

	return 0;

}

// delete redundant markers and call calculateMarkerDist for possible combinations
bool checkMarkerCount() {
	// iterate through Boards-vector
	for (size_t i = 0; i < Boards.size(); i++) {
		if (Boards[i].PanelCount > 2) {

			// cout << "\nI have seen " << Boards[i].PanelCount << " Panels:\n";
			LogBlock Board = Boards[i];
			vector<GeoBoard> myGeoBoards = Board.boards;
			vector<int> markerNrs;

			for (int j = 0; j < myGeoBoards.size(); j++) {
				// erase markers, that are redundant
				if (find(markerNrs.begin(), markerNrs.end(), myGeoBoards[j].Panel) != markerNrs.end()) {
					// cout << "I found " << myGeoBoards[j].Panel << "to be double.\n";
					myGeoBoards.erase(myGeoBoards.begin() + j);
					j = j - 1;
				}
				else {
					markerNrs.push_back(myGeoBoards[j].Panel);
					// cout << "Pushed back " << myGeoBoards[j].Panel << "\n";
				}
			}
			// if there are two markers, calculate distance between those 
			if (markerNrs.size() == 2)
				calculateMarkerDist(myGeoBoards[0], myGeoBoards[1]);
			// if there are more than two markers, calculate distance between every combination
			else if (markerNrs.size() > 2) {
				int markerSize = markerNrs.size();
				for (int i = 0; i < markerSize - 1; i++) {
					for (int j = i + 1; j < markerSize; j++) {
						calculateMarkerDist(myGeoBoards[i], myGeoBoards[j]);
					}
				}
			}
		}
	}

	return 0;

}

// get two nearest markers for every panel 
// TO DO one for left one for right #############################################
bool  getNearestNeighbors() {

	// go through Dist-Vector
	// TO DO unnessecary, create struct and safe every new markernr in the beginning ###########
	for (int i = 0; i < Dist.size(); i++) {
		int markersi[2]{Dist[i].Panel1, Dist[i].Panel2};
		
		for (int m = 0; m < 2; m++) {

			// check, if panel is already in NearestNeighbours
			int occurenceCheck = 1;
			for (int n = 0; n < NearestNeighbours.size(); n++) {
				if (NearestNeighbours[n].Panel == markersi[m]) occurenceCheck = 0;
			}
			// if panel is not already in NearestNeighbours
			if (occurenceCheck) {
				panelNeighbours PN;
				PN.Panel = markersi[m];
				for (int j = 0; j < Dist.size(); j++) {
					if (markersi[m] == Dist[j].Panel1) {
						// if panel is right
						if (Dist[j].dir) {
							if (Dist[j].Dist < PN.DistanceR) {
								PN.DistanceR = Dist[j].Dist;
								PN.NeighbourR = Dist[j].Panel2;
								PN.AngleR = 1;
							}
						}
						else {
							if (Dist[j].Dist < PN.DistanceL) {
								PN.DistanceL = Dist[j].Dist;
								PN.NeighbourL = Dist[j].Panel2;
								PN.AngleL = 0;
							}
						}
					}
					else if (markersi[m] == Dist[j].Panel2) {
						if (Dist[j].dir) {
							if (Dist[j].Dist < PN.DistanceL) {
								PN.DistanceL = Dist[j].Dist;
								PN.NeighbourL = Dist[j].Panel1;
								PN.AngleL = 0;
							}
						}
						else {
							if (Dist[j].Dist < PN.DistanceR) {
								PN.DistanceR = Dist[j].Dist;
								PN.NeighbourR = Dist[j].Panel1;
								PN.AngleR = 1;
							}
						}
					}					
				}
				NearestNeighbours.push_back(PN);
			}
		}
	}
	return 0;
}


// init function to collect data from logfiles & save them into structs
bool init() {
	// check if file is loaded correctly
	if (!getFileContent("log191223.txt", LogFile)) { 
		std::cerr << "INIT:\tERROR LOADING FILE" << std::endl;
		return false; 
	}
	std::cout << "INIT:\tFile loaded successfuly" << std::endl;
	
	// check if data is converted to structs
	if (!GetBlocks()) { 
		std::cerr << "INIT:\tERROR CONVERTING DATA" << std::endl;
		return false; 
	}
	std::cout << "INIT:\tData successfuly converted to structs" << std::endl;
	
	return true;

}


int main(int argc, char* argv[]) {
	// initializing data from logfile before proceeding with the programm
	if (!init()) {
		return 1;
	}
	checkMarkerCount();
	getNearestNeighbors();
	
	for (int i = 0; i < NearestNeighbours.size(); i++) {
		cout << "panel " << NearestNeighbours[i].Panel << " x1 " << NearestNeighbours[i].NeighbourL << " " << NearestNeighbours[i].AngleL << " x2 " << NearestNeighbours[i].NeighbourR << " " << NearestNeighbours[i].AngleR << "\n";
	}
	
	return 0;
}
