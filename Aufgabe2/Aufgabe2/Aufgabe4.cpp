#include <iostream>
#include <vector>
#include "GeoStruct.cpp"

std::vector<int> Duplicates;

void GetDuplicatePanels(std::vector<GeoBoard> Boards) {
	// ToDo 
	// Find Duplicate Panels in Frame
	int duplicate = 1; //Whatever we find
	Duplicates.push_back(duplicate);
}

double GetMaximumPoint(std::vector<GeoBoard> Boards, int panelNr) {

}

double GetMinimumPoint(std::vector<GeoBoard> Boards, int panelNr) {

}

double GetAveragePoint(std::vector<GeoBoard> Boards, int panelNr) {

}

double GetErrorRange(std::vector<GeoBoard> Boards, int panelNr) {
	// ToDo
	// Need functions from other group
}

double GetMeasurementsErrors(std::vector<LogBlock> GeoStruct) {
	std::cout << "INIT:\tMeasuring Errors" << std::endl;

	for (size_t i = 0; i < GeoStruct.size(); i++)
	{
		std::vector<GeoBoard> Frame = GeoStruct[i].boards;
		GetDuplicatePanels(Frame);
		
		for (size_t i = 0; i < Duplicates.size(); i++)
		{
			MeasurementError offset;

			offset.Panel = Duplicates[i];
			// ToDo Differentiate between X and Y with parameters?
			offset.MaxX = GetMaximumPoint(Frame, offset.Panel);
			offset.MaxY = GetMaximumPoint(Frame, offset.Panel);
			offset.MinX = GetMinimumPoint(Frame, offset.Panel);
			offset.MinY = GetMinimumPoint(Frame, offset.Panel);
			offset.AvgX = GetAveragePoint(Frame, offset.Panel); // Need Min/Max Value?
			offset.AvgY = GetAveragePoint(Frame, offset.Panel); // Need Min/Max Value?
			offset.RangeX = GetErrorRange(Frame, offset.Panel);
			offset.RangeY = GetErrorRange(Frame, offset.Panel);

			GeoStruct[i].offset.push_back(offset);
		}

	}

	
	
}