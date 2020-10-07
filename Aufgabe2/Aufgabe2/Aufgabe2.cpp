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

/*Interface to Drawing Project from 'Aufgabe1'
	Creates dummy GeoBoards to draw them in Aufgabe 1
*/


int main(int argc, char* argv[]) {

	std::cout << "initializing..." << std::endl;
	// initializing data from logfile before proceeding with the programm
	if (!init()) {
		return 1;
	}

	// use 0 for the DEBUG boardNumber because its not affected by any plotting or calculation functions // it is safe to leave to test if the programm works
	//Coordinates[0].distNeighbour = 456.4;
	//Coordinates[0].rightNeighbour = 0;
	//Coordinates[0].X = 1337;
	//Coordinates[0].Y = 42;

	
	//DEBUG_getBlocks();

	/*
	TODO:
	NORMALIZE SLOWS DOWN THE PROGRAMM
	DOES NOT WORK
	*/
	calcDivergence(Boards);
	normalizeCoordinates(Boards);
	calcRightNeighbour(Coordinates, Boards);
	calcCoordinates(Coordinates);
	DEBUG_getBoardPositions();
	
	// pause the console application so that the output can be read
	system("pause");
	return 0;
}