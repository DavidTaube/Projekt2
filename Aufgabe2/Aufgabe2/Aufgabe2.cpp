#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "GeoStruct.cpp"
#include <sstream>

#define PI 3.14159265358979323846

std::vector<LogBlock> Boards;
std::vector<std::string> LogFile;
std::vector<markerDist> Dist;


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

	markerDist markerDistance;
	markerDistance.marker1 = geo1.Panel;
	markerDistance.marker2 = geo2.Panel;
	markerDistance.dist = d1;
	
	cout << "Distance between " << geo1.Panel << " and " << geo2.Panel << " is " << d1 << "\n";

	Dist.push_back(markerDistance);

	return 0;

}

bool checkMarkerCount() {

	for (size_t i = 0; i < Boards.size(); i++) {
		if (Boards[i].PanelCount > 2) {
			//cout << "\nI have seen " << Boards[i].PanelCount << " Panels:\n";
			LogBlock Board = Boards[i];
			vector<GeoBoard> myGeoBoards = Board.boards;
			vector<int> markerNrs;
			for (int j = 0; j < myGeoBoards.size(); j++) {
				if (find(markerNrs.begin(), markerNrs.end(), myGeoBoards[j].Panel) != markerNrs.end()) {
					//cout << "I found " << myGeoBoards[j].Panel << "to be double.\n";
					myGeoBoards.erase(myGeoBoards.begin() + j);
					j = j - 1;
				}
				else {
					markerNrs.push_back(myGeoBoards[j].Panel);
					//cout << "Pushed back " << myGeoBoards[j].Panel << "\n";
				}
			}
			if (markerNrs.size() > 1)
				bool whatever = calculateMarkerDist(myGeoBoards[0], myGeoBoards[1]);
			// to do: get all combinations and calculate distance
		}
	}

	return 0;

}


// init function to collect data from logfiles & save them into structs
bool init() {
	// check if file is loaded correctly
	if (!getFileContent("log200914.txt", LogFile)) { 
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
	return 0;
}