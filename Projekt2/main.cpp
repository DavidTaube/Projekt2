#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "main.h"
#include "SDL.h"
std::map<int, BoardProperties> Coordinates;
std::vector<LogBlock> Boards;
std::vector<std::string> LogFile;

std::map<int, fakeData> FakeData;

// Debug Function to print all positions saved in the final Coordinates map
void DEBUG_getBoardPositions() {
	std::cout << "\n\nCOORDINATES:" << std::endl;
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

/*void GetXYFromCoordinates() {
	double x, y;
	x = Coordinates[1].X;
	y = Coordinates[1].Y;
	std::cout << "-------- Drawing Debugger --------" <<std::endl;
	std::cout << "X Coordinate: "<< x <<std::endl;
	std::cout << "Y Coordinate: "<< y <<std::endl;
	std::cout << "-------- --------"<<std::endl;

}*/

// Draw the SDL Window
void InitWindow() {
	//Window Config
	SDL_Renderer* renderer;
	SDL_Window* window;
	SDL_Point points[8];
	SDL_Point  startingPoint;
	startingPoint.x = 0;
	startingPoint.y = 0;
	float scale = 8.0;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		std::cout << "Failed to init SDL : " << SDL_GetError();

	window = SDL_CreateWindow("Visualize Coordinates", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 500, 0);

	if (window == nullptr)
		std::cout << "Failed creating window : " << SDL_GetError();

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (renderer == nullptr)
		std::cout << "Could not create renderer!";

	//SDL_RenderSetLogicalSize(renderer, 1000, 1000);

	// Clear background
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	//Boards = 1, 5, 2, 12, 3, 7, 4, 6
	// for scaling purpose adding *10 on each value


	points[0].x = Coordinates[1].X*10;
	points[0].y = Coordinates[1].Y*10;

	points[1].x = Coordinates[2].X *10;
	points[1].y = Coordinates[2].Y *10;

	points[2].x = Coordinates[3].X*10;
	points[2].y = Coordinates[3].Y*10;

	points[3].x = Coordinates[4].X*10;
	points[3].y = Coordinates[4].Y*10;

	points[4].x = Coordinates[5].X*10;
	points[4].y = Coordinates[5].Y*10;

	points[5].x = Coordinates[6].X*10;
	points[5].y = Coordinates[6].Y*10;

	points[6].x = Coordinates[7].X*10;
	points[6].y = Coordinates[7].Y*10;

	points[7].x = Coordinates[12].X*10;
	points[7].y = Coordinates[12].Y*10;
	// Apply scale
	for (int i = 0; i < 7; ++i)
	{
		points[i].x /= scale;
		points[i].y /= scale;
	}

	SDL_RenderSetScale(renderer, scale, scale);
	SDL_RenderDrawPoints(renderer, points, 7);

	SDL_RenderPresent(renderer);


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
	//GetMeasurementsErrors(Boards);				eure methode dauert scheiße lange ;)
	// Prints the calculated Measurement Errors
	//PrintMeasurementsErrors(Boards);
	normalizeCoordinates(Boards);
	
	calcRightNeighbour(Coordinates, Boards);
	// insert Fake data to allow other groups continue working
	insertFakeData(FakeData);
	calcCoordinates(Coordinates);
	DEBUG_getBoardPositions();
	

	//draw Window

	InitWindow();
	std::cout << "---------- Debug Visualization ----------"<< std::endl;
	std::cout << "for scaling purpose adding *10 on each value" << std::endl;
	std::cout << "---------- ----------" << std::endl;

	// pause the console application so that the output can be read
	system("pause");
	return 0;
}