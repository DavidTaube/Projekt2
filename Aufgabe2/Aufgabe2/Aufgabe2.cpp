#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "Aufgabe2.h"

std::map<int, BoardProperties> Coordinates;
std::vector<LogBlock> Boards;
std::vector<std::string> LogFile;


void DEBUG_getBoardPositions() {
	for (auto const& element : Coordinates) {
		std::cout << "BoardPositions:\tright\tdist\tX\tY" << std::endl;
		std::cout << element.first << "\t\t" << element.second.rightNeighbour << "\t" << element.second.distNeighbour << "\t" << element.second.X << "\t" << element.second.Y << std::endl;
	}
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

	// use 0 for the DEBUG boardNumber because its not affected by any plotting or calculation functions
	Coordinates[0].distNeighbour = 456.4;
	Coordinates[0].rightNeighbour = 1;
	Coordinates[0].X = 1337;
	Coordinates[0].Y = 42;
	// DEBUG VALUES
	DEBUG_getBoardPositions();

	//calcCoordinates(Coordinates);

	return 0;
}