#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>

#include <stdio.h>
#include <stdlib.h>


using namespace std;

bool map[100][100];

int setmapsize() {
    int mapsize;
    cout << "Please enter size of map\n";
    cin >> mapsize;
    return mapsize;
}


vector<vector<long double>> read_csv() {
    vector<vector<long double>> content;
    vector<long double> row;
    string line, word;

    fstream file("robot.csv", ios::in);
    if (file.is_open())
    {
        while (getline(file, line))
        {
            row.clear();

            stringstream str(line);

            while (getline(str, word, ','))
                row.push_back(stold(word));
            content.push_back(row);
        }
    }
    else
        cout << "Could not open the file\n";

    return content;
}

void drawImage(vector<vector<int>> img) {
    FILE* imageFile;

    #pragma warning(suppress : 4996)
    imageFile = fopen("map_image.ppm", "wb");
    if (imageFile == NULL) {
        perror("ERROR: Cannot open output file");
        exit(EXIT_FAILURE);
    }

    fprintf(imageFile, "P1\n");               // filetype - PBM
    fprintf(imageFile, "%d %d\n", int(img[0].size()), int(img.size()));   // dimensions - w h

    for (int i = 0; i < img.size(); i++)
    {    
        for (int j = 0; j < img[0].size(); j++)
        {
            fprintf(imageFile, "%d ", img[i][j]);
        }
        fprintf(imageFile, "\n");
    }
    fclose(imageFile);
}


int main()
{
    cout << setprecision(17);
    cout << "Reading file...\n";
    vector<vector<long double>> data = read_csv();

    //int mapsize = setmapsize();

    cout << "Drawing map image...\n";
    vector<vector<int>> img{ { 1, 0, 1 },
                         { 0, 1, 1 },
                         { 1, 0, 0 },
                        { 0, 1, 0 } };
    drawImage(img);

}

