#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <numeric>
#include <algorithm>


using namespace std;

double standardDeviation(double mean, vector<double> data);

double standardDeviation(double mean, vector<double> data) {
    double sumSquares = 0.0;
    for(int i = 0; i < data.size(); i++) 
        sumSquares += (data[i] - mean) * (data[i] - mean);
    
    return sqrt(sumSquares/data.size());

}

int main(int argc, char* argv[]) {
    vector<vector<double>> iris(12);

    string input;
    int count = 0;
    double sepalLength, sepalWidth, petalLength, petalWidth;
    string tempSpecies;

    ifstream ifile("iris.txt");

    if(ifile.fail()) 
        "error: unable to open file";
   
    getline(ifile, input);
    replace(input.begin(), input.end(), ',', ' ');
    stringstream line(input);    

    while(count < 150) {
        stringstream line(input);    
    
        line >> sepalLength >> sepalWidth >> petalLength >> petalWidth >> tempSpecies;

        int numVector = (count/50) * 4;

        iris[numVector].push_back(sepalLength);
        iris[numVector + 1].push_back(sepalWidth);
        iris[numVector + 2].push_back(petalLength);
        iris[numVector + 3].push_back(petalWidth);
        
        count++;

        getline(ifile, input);
        replace(input.begin(), input.end(), ',', ' ');
 
    }


    cout << setw(75) << "Iris Data" << setw(75) << endl;
    cout << string(150, '-') << endl;
    cout << setw(15) << " " << setw(30) << "sepal length" << setw(30) << "sepal width" << setw(30) << "petal length" << setw(30) << "petal width" << endl;
    cout << string(150, '-') << endl;

    string species[] = {"iris-setosa", "iris-versicolor", "iris-virginica"};

    for(int i = 0; i < 3; i++) {
        cout << left << setw(30) << species[i];
        for(int j = 0; j < 4; j++) {
            int numVector = (i * 4) + j;

            double min = *min_element(iris[numVector].begin(), iris[numVector].end());
            double max = *max_element(iris[numVector].begin(), iris[numVector].end());
            double mean = (accumulate(iris[numVector].begin(), iris[numVector].end(), 0.0))/50;
            double stdev = standardDeviation(mean, iris[numVector]);

            cout << fixed << setprecision(2) << setw(6) << min << setw(6) << max << setw(6) << mean << setw(6) << stdev << "  |  ";

        }

        cout << endl;
    }
    return 0;
}
