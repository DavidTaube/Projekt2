#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "main.h"

std::map<int, BoardProperties> Coordinates;
std::vector<LogBlock> Boards;
std::vector<std::string> LogFile;

std::map<int, fakeData> FakeData;

// Debug Function to print all positions saved in the final Coordinates map
void DEBUG_getBoardPositions() {
	std::cout << "\nNr\tneighbour\tdist\t\tX\tY" << std::endl;
	for (auto const& element : Coordinates) {
		std::cout << element.first << "\t" << element.second.rightNeighbour << "\t\t" << element.second.distNeighbour << "\t\t" << element.second.X << "\t" << element.second.Y << std::endl;
	}
}

//Debug Function to print out all coordinates of every panel of every frame from the logfile 
void DEBUG_getBlocks() {
	for (int i = 0; i < Boards.size(); i++) {
		std::cout << "Block: " << i << "\tSize: " << Boards[i].boards.size() << "/" << Boards[i].PanelCount << std::endl;
		for (int j = 0; j < Boards[i].boards.size(); j++) {
			std::cout << "Nr: " << Boards[i].boards[j].Panel << "\tX: " << Boards[i].boards[j].X << "\tY: " << Boards[i].boards[j].Y << std::endl;
		}
	}
}

// inserts fake data into the coordinates
// this is only for development. Because the original Logfiles are crappy, this allows the other groups to insert fakeData and work with it as they normaly would
void insertFakeData(std::map<int,fakeData> & fakedata) {
	std::cout << "start inserting fake Data" << std::endl;
	// remove old data from the map
	for (auto const& element : Coordinates) {
		Coordinates.erase(element.first);
	}
	//std::map<int, BoardProperties>::iterator itC;
	//for (itC = Coordinates.begin(); itC != Coordinates.end(); ++itC) {
	//	Coordinates.erase(itC->first);
	//}


	// iterate over FakeData map and insert values to Coordinates map
	for (auto const& element : fakedata) {
		Coordinates[element.first].rightNeighbour = element.second.rightNeighbour;
		Coordinates[element.first].distNeighbour = element.second.dist;
	}
	//std::map<int, fakeData>::iterator itF;
	//for (itF = FakeData.begin(); itF != FakeData.end(); ++itF) {
	//	Coordinates[itF->first].rightNeighbour = itF->second.rightNeighbour;
	//	Coordinates[itF->first].distNeighbour = itF->second.dist;
	//}
	std::cout << "finished inserting fake Data" << std::endl;
}


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

	std::cout << "initializing..." << std::endl;
	// initializing data from logfile before proceeding with the programm
	if (!init()) {
		return 1;
	}
	
	//DEBUG_getBlocks();


	/*REMEMBER: 
	 * 1-4 are ALLWAYS CORNERS 
	 * dist is a DOUBLE 
	 * rightNeighbour is an INT
	 */
	//EXAMPLE FAKE DATA:
	FakeData[1].rightNeighbour = 5;
	FakeData[1].dist = 10.0;
	FakeData[5].rightNeighbour = 2;
	FakeData[5].dist = 20.0;
	FakeData[2].rightNeighbour = 12;
	FakeData[2].dist = 15.0;
	FakeData[12].rightNeighbour = 3;
	FakeData[12].dist = 15.0;
	FakeData[3].rightNeighbour = 7;
	FakeData[3].dist = 20.0;
	FakeData[7].rightNeighbour = 4;
	FakeData[7].dist = 10.0;
	FakeData[4].rightNeighbour = 6;
	FakeData[4].dist = 17.0;
	FakeData[6].rightNeighbour = 1;
	FakeData[6].dist = 13.0;

	calcDivergence(Boards);
	normalizeCoordinates(Boards);
	calcRightNeighbour(Coordinates, Boards);
	// insert Fake data to allow other groups continue working
	insertFakeData(FakeData);
	calcCoordinates(Coordinates);
	DEBUG_getBoardPositions();
	
	// pause the console application so that the output can be read
	system("pause");
	return 0;
}