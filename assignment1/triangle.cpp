#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cmath>



using namespace std;

int main(int argc, char* argv[]) {
    bool valid = true;
    string input;
    vector<double> points;
    int count = 0;
    double tempPoint;

    while (valid) {
        cout << "please enter the points of a triangle with spaces separating the points itself. make sure to enter them in the order of x1, y1, x2, y2, x3, y3." << endl;
        cout << "to quit, please type in 'quit'" << endl;

        getline(cin, input);

        if(input == "quit") {
            cout << "program quit!" << endl;
            break;
        }

        stringstream line(input);
        
        while (line >> tempPoint) {
            count++;
            points.push_back(tempPoint);
        }

        if(count < 5) {

            count = 0;
            cout << "please try again. an error has occurred." << endl;
            points.clear();
            line.clear();
        } else {
            cout << "the area of the triangle of the points given is " << fabs(0.5*(points[0] * (points[3] - points[5]) + points[2] * (points[5] - points[1]) + points[4] * (points[1] - points[3]))) << endl;
            valid = false;   

        }

    }
    
    return 0;


}