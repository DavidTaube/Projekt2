#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "main.h"

std::map<int, BoardProperties> Coordinates;
std::vector<LogBlock> Boards;
std::vector<FakeLogBlock> FakeBoards;
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
void insertFakeData(std::map<int, fakeData> &fakedata) {
	std::cout << "start inserting fake Data..." << std::endl;
	
	// remove old data from the map
	std::map<int, BoardProperties> itCoordinates = Coordinates;
	for (auto const& element : itCoordinates) {
		Coordinates.erase(element.first);
	}

	// iterate over FakeData map and insert values to Coordinates map
	for (auto const& element : fakedata) {
		Coordinates[element.first].rightNeighbour = element.second.rightNeighbour;
		Coordinates[element.first].distNeighbour = element.second.dist;
	}
	
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

// add blocks for fakeboards
void createFakeScans() {
	std::cout << std::endl  << "start creating fake Scans..." << std::endl;
	std::cout << "\nScanNr\tBoardNr1\tX\tY\tdist\tBoardNr2\tX\tY\tdist" << std::endl;

	//create pos and dist of first board
	int posOfFirstBlock[8][3];
	posOfFirstBlock[0][0] = 0;
	posOfFirstBlock[0][1] = 0;
	posOfFirstBlock[0][2] = 4;
	posOfFirstBlock[1][0] = 0;
	posOfFirstBlock[1][1] = 30;
	posOfFirstBlock[1][2] = 9;
	posOfFirstBlock[2][0] = 30;
	posOfFirstBlock[2][1] = 30;
	posOfFirstBlock[2][2] = 12;
	posOfFirstBlock[3][0] = 30;
	posOfFirstBlock[3][1] = 0;
	posOfFirstBlock[3][2] = 10;
	posOfFirstBlock[4][0] = 0;
	posOfFirstBlock[4][1] = 10;
	posOfFirstBlock[4][2] = 8;
	posOfFirstBlock[5][0] = 13;
	posOfFirstBlock[5][1] = 0;
	posOfFirstBlock[5][2] = 8;
	posOfFirstBlock[6][0] = 30;
	posOfFirstBlock[6][1] = 10;
	posOfFirstBlock[6][2] = 17;
	posOfFirstBlock[7][0] = 15;
	posOfFirstBlock[7][1] = 30;
	posOfFirstBlock[7][2] = 7;

	for (int x = 1; x < 5; x++) {
		FakeLogBlock block;

		//insert data in first board
		FakeGeoboard board1;
		board1.Panel = x;
		board1.X = posOfFirstBlock[x-1][0];
		board1.Y = posOfFirstBlock[x-1][1];
		board1.distToKornknecht = posOfFirstBlock[x-1][2];
		block.boards.push_back(board1);

		//insert datain second board
		FakeGeoboard board2;
		int neighbour = Coordinates[x].rightNeighbour;
		board2.Panel = neighbour;
		board2.X = Coordinates[neighbour].X;
		board2.Y = Coordinates[neighbour].Y;

		//cheack if boardpanel is 12
		if (board2.Panel > 7) {
			board2.distToKornknecht = posOfFirstBlock[7][2];
		}
		else{
			board2.distToKornknecht = posOfFirstBlock[neighbour - 1][2];
		}
		block.boards.push_back(board2);
		
		FakeBoards.push_back(block);

		std::cout << x << "\t" << board1.Panel << "\t\t" << board1.X << "\t" << board1.Y << "\t" << board1.distToKornknecht << "\t" << board2.Panel << "\t\t" << board2.X << "\t" << board2.Y << "\t" << board2.distToKornknecht << std::endl;
	}

	std::cout << "finished creating fake Scans..." << std::endl;
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
	
	//task3 create fakescan and calc position of Kornknecht
	createFakeScans();
	calcPosKornknecht(FakeBoards, Coordinates);

	// pause the console application so that the output can be read
	system("pause");
	return 0;
}