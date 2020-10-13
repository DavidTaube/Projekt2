#include <map>
#include <math.h>
#include <iostream>
#include <numeric>
#include "Calculations.h"

// temporary 60° value for the cameras
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
	/*
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
	*/
	d1 = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));

	return d1;
}


// calculate the coordinates by looking at the corners and switching axies. It does so by adding up the distances from the markers until a corner is reached
void calcCoordinates(std::map<int, BoardProperties> &boards) {
	std::cout << "start calculating coordinates..." << std::endl;
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
	std::cout << "finished calculating coordinates" << std::endl;
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
			if (element.second.Y <= biggest.Y) {
				biggest = element.second;
			}
		}
		// push the found board into a vector
		boardVec.push_back(biggest);
		// delete the just found board from the original map
		boardMap.erase(biggest.Panel);
	}

	for (int i = 0; i < boardVec.size(); i++) {
		//std::cout << "Panel: " << boardVec[i].Panel << "\tY: " << boardVec[i].Y;
	}

	return boardVec;
}

// calculates the neighbours to the right for all boards.  
void calcRightNeighbour(std::map<int, BoardProperties> &coordinates, std::vector<LogBlock> &blocks) {
	std::cout << "start calculating neighbours..." << std::endl;
	// iterate over every block
	std::map<int, std::map<int,double>> tmpDistMap;

	std::map<int, std::map<int, std::vector<double>>>tmpDistVector;
	for (int blockNumber = 0; blockNumber < blocks.size(); blockNumber++) {
		
		// if the block contains just 1 panel, we ignore it because its useless
		if (blocks[blockNumber].PanelCount <= 1) {
			//std::cout << "Skipped Block becuase it has a size of 1" << std::endl; // DEBUG
			continue;
		}

		// translate into a map to eliminate double entries
		std::map<int,GeoBoard> tmpBoardMap;

		std::map<int, std::vector<GeoBoard>> tmpVectorBoardMap;
		// set every boardNumber as a key in the new map with the value of the board itself.
		//int camera;
		for (int boardNumber = 0; boardNumber < blocks[blockNumber].boards.size(); boardNumber++) {
			tmpVectorBoardMap[blocks[blockNumber].boards[boardNumber].Panel].push_back(blocks[blockNumber].boards[boardNumber]);
		}
		for (auto const& element : tmpVectorBoardMap) {
			double sumX = element.second[0].X;
			double sumY = element.second[0].Y;
			for (int i = 1; i < element.second.size(); i++) {
				sumX += element.second[i].X;
				sumY += element.second[i].Y;
			}
			tmpBoardMap[abs(element.first)].X = sumX / element.second.size();
			tmpBoardMap[abs(element.first)].Y = sumY / element.second.size();
			tmpBoardMap[abs(element.first)].Panel = abs(element.first);
			tmpBoardMap[abs(element.first)].Camera = 1;
		}

		// sort the boards to see their order from right to left
		//std::cout << "BlockNumber: " << blockNumber << std::endl;
		std::vector<GeoBoard> sortedBoards = sortBoards(tmpBoardMap);


		for (int i = 1; i < sortedBoards.size(); i++) {
			GeoBoard current = sortedBoards[i];
			GeoBoard neighbour = sortedBoards[i - 1];
			tmpDistVector[current.Panel][neighbour.Panel].push_back(calcDistanceBetweenBoards(current, neighbour)) ;
		}

		// DEBUG text to print all block with all boards after they have been sorted and combined
		//std::cout << "Block: " << blockNumber << std::endl;
		//for (int i = 0; i < sortedBoards.size(); i++) {
		//	std::cout << "Nr: " << sortedBoards[i].Panel << "\tY: " << sortedBoards[i].Y << std::endl;
		//}
	}
	std::cout << "all neighbours:" << std::endl;
	for (auto const& element : tmpDistVector) {
		for (auto const& innerelement : element.second) {
			tmpDistMap[element.first][innerelement.first] = std::accumulate(innerelement.second.begin(), innerelement.second.end(), 0.0) / innerelement.second.size();
		}
	}

	for (auto const& element : tmpDistMap) {
		BoardProperties smallest;
		bool first = true;
		for (auto const& innerelement : element.second) {
			if (first) {
				smallest.distNeighbour = innerelement.second;
				smallest.rightNeighbour = innerelement.first;
				first = false;
			}
			if (smallest.distNeighbour > innerelement.second) {
				smallest.distNeighbour = innerelement.second;
				smallest.rightNeighbour = innerelement.first;
			}
			std::cout << "Panel: " << element.first << "\tNeighbour: " << innerelement.first << "\tdistance: " << innerelement.second << std::endl;
		}
		coordinates[element.first] = smallest;
	}
	std::cout << "nearest Neighbour:" << std::endl;
	for (auto const& element : coordinates) {
		std::cout << "Panel: " << element.first << "\tNeighbour: " << element.second.rightNeighbour << std::endl;
	}

	std::cout << "finished calculating neighbours" << std::endl;
}

void normalizeCoordinates(std::vector<LogBlock>& blocks) {
	std::cout << "start normalizing coordinates..." << std::endl;
	for (int i = 0; i < blocks.size(); i++) {
		//std::cout << "Block: " << i << std::endl;
		for (int j = 0; j < blocks[i].boards.size(); j++) {
			double X = blocks[i].boards[j].X;
			double Y = blocks[i].boards[j].Y;
			if (blocks[i].boards[j].Camera == 0) {
				blocks[i].boards[j].X = X * cos(divergence * PI / 180.0) + Y * sin(divergence * PI / 180.0);
				blocks[i].boards[j].Y = -X * sin(divergence * PI / 180.0) + Y * cos(divergence * PI / 180.0);
				blocks[i].boards[j].Camera = 1;
			}
			//std::cout << "Nr: " << blocks[i].boards[j].Panel << "\t" << "\tX: " << X << "\tY: " << Y << "\t\t\tX: " << blocks[i].boards[j].X << "\tY: " << blocks[i].boards[j].Y << std::endl;

		}
	}
	std::cout << "finished normalizing coordinates" << std::endl;
}


// calculates the divergence (angle) between the two cameras
void calcDivergence(std::vector<LogBlock>& blocks) {
	std::cout << "calculating divergence..." << std::endl;
	std::vector<double> divergenceVector;
	for (int i = 0; i < blocks.size(); i++) {
		//std::cout << "Block: " << i << std::endl;
		if (blocks[i].PanelCount < 2) {
			continue;
		}
		std::map<int, GeoBoard> tmpMap;
		int camera;
		for (int j = 0; j < blocks[i].boards.size(); j++) {
			camera = (blocks[i].boards[j].Camera) ? -1 : 1;
			tmpMap[blocks[i].boards[j].Panel * camera] = blocks[i].boards[j];
		}
		for (auto const& element : tmpMap) {
			if (element.first >= 0) {
				break;
			}
			if (element.second.Panel == tmpMap[element.first * -1].Panel) {
				//std::cout << "Panel: " << element.second.Panel << "\tfound element match" << std::endl;
				double angle = 360 - (atan2(element.second.Y, element.second.X) - atan2(tmpMap[element.first * -1].Y, tmpMap[element.first * -1].X)) / PI * 180;
				//std::cout << "Block: " << i << "\tAngle: " << angle << std::endl;
				divergenceVector.push_back(angle);

			}
		}
	}
	divergence = std::accumulate(divergenceVector.begin(), divergenceVector.end(), 0.0) / divergenceVector.size();
	std::cout << "divergence: " << divergence << std::endl;
}

// calculates the positions of Kornknecht
void calcPosKornknecht(std::vector<FakeLogBlock>& FakeBoards, std::map<int, BoardProperties>& Coordinates) {
	std::cout << "calculating positions of Kornknecht..." << std::endl;
	std::vector<FakeKornknecht> KornknechtPos;
	FakeKornknecht kornknecht;
	for (int i = 0; i < FakeBoards.size(); i++) {

		//cheack if 2 boards were found
		if (FakeBoards[i].boards.size() < 2) {
			continue;
		}

		double a, b, c, alpha, beta, gamma;

		// get length of dist to create triangle
		a = FakeBoards[i].boards[0].distToKornknecht;
		b = FakeBoards[i].boards[1].distToKornknecht;
		c = Coordinates[FakeBoards[i].boards[0].Panel].distNeighbour;
		
		// check if sides creat triangle
		if (a + b <= c || a + c <= b || b + c <= a) {
			std::cout << "Position couldnt be calculated(cant create triangle)" << std::endl;
			continue;
		}
		
		//calc angle
		alpha = acos((((a * a) - (b * b) - (c * c)) / (-2 * b * c)));
		double alphagrad = alpha * (180/PI);

		beta = asin((b / a) * (sin(alpha)));
		double betagrad = beta * (180/PI);

		gamma = 180 - alphagrad - betagrad;

		//kornknecht.X = (c*(tan(beta)))/(1+tan(beta));
		//kornknecht.Y = kornknecht.X * tan(alpha);

		//calc X/Y position
		kornknecht.X = b*(cos(alpha));
		kornknecht.Y = b*(sin(alpha));

		//kornknecht.X = ((b * b) + (c * c) - (a * a)) / (2 * c);
		//kornknecht.Y = sqrt((b * b) - (kornknecht.X * kornknecht.X));

		/*
		double ax0, ax1, ax2, ax3, ay0, ay1, ay2, ay3;
		double bx0, bx1, bx2, bx3, by0, by1, by2, by3;

		//rechte Seite
		double ar = FakeBoards[i].boards[0].distToKornknecht * FakeBoards[i].boards[0].distToKornknecht;
		double br = FakeBoards[i].boards[1].distToKornknecht * FakeBoards[i].boards[1].distToKornknecht;
		double y,yx0, yx1;

		//ohne x/y
		ax0 = FakeBoards[i].boards[0].X * FakeBoards[i].boards[0].X;
		ay0 = FakeBoards[i].boards[0].Y * FakeBoards[i].boards[0].Y;

		// mit x/y
		ax1 = FakeBoards[i].boards[0].X * 2;
		ay1 = FakeBoards[i].boards[0].Y * 2;

		//ohne x/y
		bx0 = FakeBoards[i].boards[1].X * FakeBoards[i].boards[1].X;
		by0 = FakeBoards[i].boards[1].Y * FakeBoards[i].boards[1].Y;

		//mit x/y
		bx1 = FakeBoards[i].boards[1].X * 2;
		by1 = FakeBoards[i].boards[1].Y * 2;

		//y
		y = (-ay1) - (-by1);

		if (y == 0) {
			//x0
			yx0 = (ar - br - (ax0 - bx0) - (ay0 - by0));
			//x1
			yx1 = (0 - ((-ax1) - (-bx1)));
		}
		else {
			//x0
			yx0 = (ar - br - (ax0 - bx0) - (ay0 - by0)) / y;
			//x1
			yx1 = (0 - ((-ax1) - (-bx1)) / y);
		}



		//qudratische Gleichung
		double cx2 = 1 + (yx1*yx1);
		double cx1, cx0;
		if (yx0 < 0) {
			cx1 = (-ax1) - (2 * (yx1 * yx0)) + (ay1 * yx0);
		}
		else {
			cx1 = (-ax1) + (2 * (yx1 * yx0)) + (ay1 * yx0);
		}
		cx0 = ax0 + ay0 + (yx0 * yx0) + yx0 - ar;

		cx1 = cx1 / cx2;
		cx0 = cx0 / cx2;

		double p = cx1;
		double q = cx0;

		double L1X, L1Y, L2X, L2Y;

		L1X = ((p / 2) * (-1)) + (sqrt(((p / 2) * (p / 2) - q)));
		L2X = ((p / 2) * (-1)) - (sqrt(((p / 2) * (p / 2) - q)));

		L1Y = (L1X * L1X) + (cx1 * L1X) + cx0;
		L2Y = (L2X * L2X) + (cx1 * L2X) + cx0;

		if (L1X < 0 || L1Y < 0) {
			kornknecht.X = L2X;
			kornknecht.Y = L2Y;
		}
		else if (L2X < 0 || L2Y < 0) {
			kornknecht.X = L1X;
			kornknecht.Y = L1Y;
		}*/

		KornknechtPos.push_back(kornknecht);
		
		std::cout << "Position Kornknecht:  X: " << KornknechtPos[i].X << "   Y: " << KornknechtPos[i].Y << std::endl;
	}
	std::cout << "finished calculating positions of Kornknecht..." << std::endl << std::endl;
}
