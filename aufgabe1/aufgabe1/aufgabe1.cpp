#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main() {
    string line;
    int firstCamera = 0;
    int secCamera = 1;

    ifstream myfile("log200914.txt");
    if (myfile.is_open())
    {
        while (getline(myfile, line)) {
              cout << line << endl;
        }
        
        myfile.close();
    }

    else cout << "Unable to open file";

    return 0;
}