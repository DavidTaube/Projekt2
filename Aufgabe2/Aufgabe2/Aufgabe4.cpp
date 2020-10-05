#include <iostream>
#include <vector>
#include "GeoStruct.cpp"
#include <map>

std::vector<int> Duplicates;

// find duplicate panels in a frame
void GetDuplicatePanels(std::vector<GeoBoard> Boards) {
	// Create a map to store the frequency of each element in vector
	std::map<int, int> countMap;
	// Iterate over the vector and store the frequency of each element in map
	for (auto& elem : Boards)
	{
		auto result = countMap.insert(std::pair<int, int>(elem.Panel, 1));
		if (result.second == false)
			result.first->second++;
	}

	// Iterate over the map
	for (auto& elem : countMap)
	{
		// If frequency count is greater than 1 then its a duplicate element
		if (elem.second > 1)
		{
			Duplicates.push_back(elem.first);
		}
	}
}

// Get max point of the same panel in a frame
double GetMaximumPoint(std::vector<GeoBoard> Boards, int panelNr, bool isX) {
	double max = NULL;
	
	for (size_t i = 0; i < Boards.size(); i++)
	{
		if (Boards[i].Panel == panelNr) {
			if (isX) {
				max = (max == NULL) ? Boards[i].X : (max < Boards[i].X) ? Boards[i].X : max;
			}
			else {
				max = (max == NULL) ? Boards[i].Y : (max < Boards[i].Y) ? Boards[i].Y : max;
			}			
		}
	}
	
	return max;
}

// Get min point of the same panel in a frame
double GetMinimumPoint(std::vector<GeoBoard> Boards, int panelNr, bool isX) {
	double min = NULL;

	for (size_t i = 0; i < Boards.size(); i++)
	{
		if (Boards[i].Panel == panelNr) {
			if (isX) {
				min = (min == NULL) ? Boards[i].X : (min > Boards[i].X) ? Boards[i].X : min;
			}
			else {
				min = (min == NULL) ? Boards[i].Y : (min > Boards[i].Y) ? Boards[i].Y : min;
			}
		}
	}
	
	return min;
}

// Get average of all points of the same panel in a frame
double GetAveragePoint(std::vector<GeoBoard> Boards, int panelNr, bool isX) {
	double avg = 0.0;
	int divisionCount = 0;
	
	for (size_t i = 0; i < Boards.size(); i++)
	{
		if (Boards[i].Panel == panelNr) {
			if (isX) {
				avg += Boards[i].X;
				divisionCount++;
			}
			else {
				avg += Boards[i].Y;
				divisionCount++;
			}
		}
	}
	
	avg = avg / divisionCount;
	
	return avg;
}

// calculate the range of possible error occurencies
double GetErrorRange(std::vector<GeoBoard> Boards, int panelNr) {
	// ToDo
	// Need functions from other group
	return 0.0; // Percentage
}

// starting point for the error measurement
void GetMeasurementsErrors(std::vector<LogBlock> GeoStruct) {
	std::cout << "INIT:\tMeasuring Errors" << std::endl;

	for (size_t i = 0; i < GeoStruct.size(); i++)
	{
		// Get relevant frame
		std::vector<GeoBoard> Frame = GeoStruct[i].boards;
		GetDuplicatePanels(Frame);
		
		// fill error struct
		for (size_t j = 0; j < Duplicates.size(); j++)
		{
			MeasurementError offset;

			offset.Panel = Duplicates[j];

			offset.MaxX = GetMaximumPoint(Frame, offset.Panel, true);
			offset.MaxY = GetMaximumPoint(Frame, offset.Panel, false);
			offset.MinX = GetMinimumPoint(Frame, offset.Panel, true);
			offset.MinY = GetMinimumPoint(Frame, offset.Panel, false);
			offset.AvgX = GetAveragePoint(Frame, offset.Panel, true);
			offset.AvgY = GetAveragePoint(Frame, offset.Panel, false);
			offset.Range = GetErrorRange(Frame, offset.Panel);

			GeoStruct[i].offset.push_back(offset);
		}
	}	
}