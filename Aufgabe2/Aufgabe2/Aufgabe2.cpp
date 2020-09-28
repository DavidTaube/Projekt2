#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "GeoStruct.cpp"
#include <sstream>

using namespace std;

vector<LogBlock> Boards;
vector<string> LogFile;


bool getFileContent(std::string fileName, std::vector<std::string>& vecOfStrs)
{
	std::ifstream in(fileName.c_str());
	if (!in)
	{
		std::cerr << "Cannot open the File : " << fileName << std::endl;
		return false;
	}
	std::string str;
	while (std::getline(in, str))
	{
		if (str.size() > 0)
			vecOfStrs.push_back(str);
	}
	in.close();
	return true;
}




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
			board.X = stoi(LogFile.at(line++));
			board.Y = stoi(LogFile.at(line++));
			board.Rotation = stoi(LogFile.at(line++));
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


int main()
{
	bool result = getFileContent("log191223.txt", LogFile);
	cout << result;
	bool ConvertLogToStructs = GetBlocks();
	cout << ConvertLogToStructs;

	return 0;
}