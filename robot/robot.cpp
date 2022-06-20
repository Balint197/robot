#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

bool map[100][100];

int setmapsize() {
    int mapsize;
    cout << "Please enter size of map\n";
    cin >> mapsize;
    return mapsize;
}
/*
double** read_csv() {
    vector<vector<string>> content;
    vector<string> row;
    string line, word;
    double** data_in = 0;
    data_in = new double* [2201];

    fstream file("robot.csv", ios::in);
    if (file.is_open())
    {
        while (getline(file, line))
        {
            row.clear();

            stringstream str(line);

            while (getline(str, word, ','))
                row.push_back(word);
            content.push_back(row);
        }
    }
    else
        cout << "Could not open the file\n";


    for (int i = 1; i < content.size(); i++)
    {
        data_in[i] = new double[i];
        for (int j = 0; j < content[i].size(); j++)
        {
            data_in[i][j] = stod(content[i][j]);
        }
    }

    return data_in;
}
*/

vector<vector<string>> read_csv() {
    vector<vector<string>> content;
    vector<string> row;
    string line, word;

    fstream file("robot.csv", ios::in);
    if (file.is_open())
    {
        while (getline(file, line))
        {
            row.clear();

            stringstream str(line);

            while (getline(str, word, ','))
                row.push_back(word);
            content.push_back(row);
        }
    }
    else
        cout << "Could not open the file\n";

    return content;
}

int main()
{
    cout << "Reading file\n";
    
    //double** data = read_csv();
    vector<vector<string>> data = read_csv();
    cout << data[10][2];
    
    //int mapsize = setmapsize();
}

