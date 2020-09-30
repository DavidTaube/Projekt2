#include <map>
#include <math.h>
#include <iostream>
#include "Calculations.h"

// temporary 60° value for the cameras
// TODO: Needs to be overwritten at runtime ( prequisits: 1 board must be recognized from both cameras at the same time )
double divergence = 60.0;

// PI Constant withoud needing to include cmath.h and with much higher precision
double get_PI() { return atan(1) * 4; }
double PI = get_PI();


// calculates the distance of an object from the camera with its X & Y coordinates
double calcDistance(double X, double Y) {
	return sqrt(X*X + Y*Y);
}

// calculates the distance between two boards
double calcDistanceBetweenBoards(GeoBoard board1, GeoBoard board2) {

	static double x1, x2, y1, y2, d1, d2, angle1, angle2, sign;

	x1 = board1.X;
	y1 = board1.Y;

	x2 = board2.X;
	y2 = board2.Y;

	if (board1.Camera != board2.Camera) {
		sign = (board1.Camera < board2.Camera ? -1.0 : 1.0);
		d1 = sqrt(x1 * x1 + y1 * y1);
		angle1 = atan(y1 / x1) + sign * divergence / 2 * PI / 180.0;
		x1 = d1 * cos(angle1);
		y1 = d1 * sin(angle1);

		d2 = sqrt(x2 * x2 + y2 * y2);
		angle2 = atan(y2 / x2) - sign * divergence / 2 * PI / 180.0;
		x2 = d2 * cos(angle2);
		y2 = d2 * sin(angle2);
	}
	d1 = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));

	return d1;
}


// calculate the coordinates by looking at the corners and switching axies. It does so by adding up the distances from the markers until a corner is reached
void calcCoordinates(std::map<int, BoardProperties> boards) {

	// set initial numbers
	int boardNumber = 1;
	int directionX = 0;
	int directionY = 1;
	int corner = 0;

	// set coordinate origin
	boards[boardNumber].X = 0.0;
	boards[boardNumber].Y = 0.0;
	// lopp until break out
	while (true) {
		// if the number on the board is between 1 and 4 it must be a corner
		if (boardNumber <= 4) {
			switch (corner)
			{
			case 0:
				// from 0,0 to 0,Y
				directionX = 0;
				directionY = 1;
				break;
			case 1:
				// from 0,Y to X,Y
				directionX = 1;
				directionY = 0;
				break;
			case 2:
				// from X,Y to Y,0 (hopefully)
				directionX = 0;
				directionY = -1;
				break;
			case 3:
				// from Y,0 to 0,0 (hopefully)
				directionX = -1;
				directionY = 0;
				break;
			default:
				break;
			}
			corner++;
		}

		// set the new coordinates to the current boardProperties struct
		boards[boards[boardNumber].rightNeighbour].X = boards[boardNumber].X + directionX * boards[boardNumber].distNeighbour;
		boards[boards[boardNumber].rightNeighbour].Y = boards[boardNumber].Y + directionY * boards[boardNumber].distNeighbour;

		// set the new neighbour for the board
		boardNumber = boards[boardNumber].rightNeighbour;
		// if the boardnumber is now 1, the loop is ended
		if (boardNumber == 1) {
			break;
		}
	}
}


// sort the Boards by its y coordinates to get them ordered from right to left
std::vector<GeoBoard> sortBoards(std::map<int, GeoBoard> boardMap) {
	std::vector<GeoBoard> boardVec;
	// set the max iteration count to boardMap.size()
	int max = boardMap.size(); // same as boardMap.PanelCount
	// run until the maximum iteration count is reached
	for (int i = 0; i < max; i++) {
		bool first = true;
		GeoBoard biggest;
		// iterate over every board in the map
		for (auto const& element : boardMap) {
			// if its the first, set it to maximum
			if (first) {
				biggest = element.second;
				first = false;
			}
			// if the Y coordinate is higher than or equal to the current biggest, set the new board in biggest
			if (element.second.Y >= biggest.Y) {
				biggest = element.second;
			}
		}
		// push the found board into a vector
		boardVec.push_back(biggest);
		// delete the just found board from the original map
		boardMap.erase(biggest.Panel);
	}

	return boardVec;
}

// calculates the neighbours to the right for all boards.  
void calcRightNeighbour(std::map<int, BoardProperties> coordinates, std::vector<LogBlock> blocks) {

	// iterate over every block
	for (int blockNumber = 0; blockNumber < blocks.size(); blockNumber++) {
		
		// if the block contains just 1 panel, we ignore it because its useless
		if (blocks[blockNumber].PanelCount <= 1) {
			//std::cout << "Skipped Block becuase it has a size of 1" << std::endl; // DEBUG
			continue;
		}

		//TODO: This is a relict from testing - the map does not need to be created. the function sortBoards should also work with a plain vector
		std::map<int, GeoBoard> tmpBoardMap;
		// set evry boardNumber as a key in the new map with the value of the board itself.
		for (int boardNumber = 0; boardNumber < blocks[blockNumber].boards.size(); boardNumber++) {
			tmpBoardMap[blocks[blockNumber].boards[boardNumber].Panel] = blocks[blockNumber].boards[boardNumber];
		}
		
		// sort the boards to see their order from right to left
		std::vector<GeoBoard> vector = sortBoards(tmpBoardMap);


		/*
		.
		.
		.
		TODO: calculate neighbour
		
		*/



		// DEBUG text to print all block with all boards after they have been sorted and combined
		//std::cout << "Block: " << blockNumber << std::endl;
		//for (int i = 0; i < vector.size(); i++) {
		//	std::cout << "Nr: " << vector[i].Panel << "\tY: " << vector[i].Y << std::endl;
		//}
	}
}

