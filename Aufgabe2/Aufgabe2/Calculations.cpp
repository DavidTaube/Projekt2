#include <map>
#include <math.h>
#include <iostream>
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

void calcCoordinates(std::map<int, BoardProperties> boards) {

	int boardNumber = 1;
	int directionX = 0;
	int directionY = 1;
	int corner = 0;
	boards[boardNumber].X = 0.0;
	boards[boardNumber].Y = 0.0;
	while (true) {
		if (boardNumber <= 4) {
			switch (corner)
			{
			case 0:

				directionX = 0;
				directionY = 1;
				break;
			case 1:
				directionX = 1;
				directionY = 0;
				break;
			case 2:
				directionX = 0;
				directionY = -1;
				break;
			case 3:
				directionX = -1;
				directionY = 0;
				break;
			default:
				break;
			}
			corner++;
		}

		boards[boards[boardNumber].rightNeighbour].X = boards[boardNumber].X + directionX * boards[boardNumber].distNeighbour;
		boards[boards[boardNumber].rightNeighbour].Y = boards[boardNumber].Y + directionY * boards[boardNumber].distNeighbour;

		boardNumber = boards[boardNumber].rightNeighbour;
		if (boardNumber == 1) {
			break;
		}
	}
}