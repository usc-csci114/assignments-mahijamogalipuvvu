#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <matplot/matplot.h>
#include <algorithm> 

using namespace std;
using namespace matplot;

pair<pair<double, double>, double> linearRegression (const vector<double>& x, const vector<double>& y) {
    double numerator = 0.0;
    double denominator = 0.0;
    double totSS = 0.0;
    double resSS = 0.0;
    vector<double> predicted; 


    double xMean = accumulate(x.begin(), x.end(), 0.0) / x.size();
    double yMean = accumulate(y.begin(), y.end(), 0.0) / y.size();

    for (int i = 0; i < x.size(); i++) {
        numerator += (x[i] - xMean) * (y[i] - yMean);
        denominator += pow(x[i] - xMean, 2);
    }

    double beta = numerator / denominator;
    double alpha = yMean - (beta * xMean);

    for (int i = 0; i < x.size(); i++) {
        double predictedValue = alpha + (beta * x[i]);
        predicted.push_back(predictedValue);

        resSS += pow((y[i] - predictedValue), 2);
        totSS += pow((y[i] - yMean), 2);
    }

    double rSquared = 1 - (resSS / totSS);

    return {{beta, alpha}, rSquared};


}

int main() {
    string fileName = "bug-attraction.csv";

    ifstream file(fileName);
    if (!file.is_open()) {
        cerr << "error! can't open file " << "\n";
        return 1;
    }
    
    string line;

    getline(file, line);
    vector<double> BGx, BGy, SLx, SLy;

    while (getline(file, line)) {
        stringstream ss(line);
        string location, temp;
        
        int option = 0;
        int index = 0;

        getline(ss, temp, ',');
        getline(ss, temp, ',');
        getline(ss, location, ',');
       
        if(location == "BG")
            option = 1;
        else if (location == "Stunt" || location == "LK" )
            option = 2;

        while(getline(ss, temp, ',')) {
            if(index == 1) {
                if(option == 1) {
                    BGx.push_back(stod(temp)); 
            }else if(option == 2)
                    SLx.push_back(stod(temp));
            } else if (index >= 28) {
                if(option == 1)
                    BGy.push_back(stoi(temp));
                else if(option == 2)
                    SLy.push_back(stoi(temp));
            }
            index++;
        }

        }

    file.close();


    auto [slopeInterceptBG, rSquaredBG] = linearRegression(BGx, BGy);
    double slopeBG = slopeInterceptBG.first;
    double interceptBG = slopeInterceptBG.second;
    vector<double> trendBGx = linspace(*min_element(BGx.begin(), BGx.end()), *max_element(BGx.begin(), BGx.end()), 25);
    vector<double> trendBGy(trendBGx.size());
    transform(trendBGx.begin(), trendBGx.end(), trendBGy.begin(), [slopeBG, interceptBG](double x) {return slopeBG * x + interceptBG;});
    cout << "equation of line: y = " << slopeBG << "x + " << interceptBG << endl;
   
    double minX = *min_element(BGx.begin(), BGx.end());
    double maxX = *max_element(BGx.begin(), BGx.end());
    double minY = *min_element(BGy.begin(), BGy.end());
    double maxY = *max_element(BGy.begin(), BGy.end());

    auto plot1 = figure();
    hold(on);
    scatter(BGx, BGy);
    plot(trendBGx, trendBGy);
    xlim({minX, maxX}); 
    ylim({minY, maxY}); 
    xlabel("Standardized Moon");
    ylabel("Total Bug Counts for Each Trap for a Night");
    title("Standardized Moon Phase vs. Total Bug Counts in Botanical Garden");
    show();

    auto [slopeInterceptSL, rSquaredSL] = linearRegression(SLx, SLy);
    double slopeSL = slopeInterceptSL.first;
    double interceptSL = slopeInterceptSL.second;
    vector<double> trendSLx = linspace(*min_element(SLx.begin(), SLx.end()), *max_element(SLx.begin(), SLx.end()), 25);
    vector<double> trendSLy(trendSLx.size());
    transform(trendSLx.begin(), trendSLx.end(), trendSLy.begin(), [slopeSL, interceptSL](double x) {return slopeSL * x + interceptSL;});
    cout << "equation of line: y = " << slopeSL << "x + " << interceptSL << endl;

    minX = *min_element(SLx.begin(), SLx.end());
    maxX = *max_element(SLx.begin(), SLx.end());
    minY = *min_element(SLy.begin(), SLy.end());
    maxY = *max_element(SLy.begin(), SLy.end());

    auto plot2 = figure();
    hold(on);
    scatter(SLx, SLy);
    plot(trendSLx, trendSLy);
    xlim({minX, maxX}); 
    ylim({minY, maxY}); 
    xlabel("Standardized Moon");
    ylabel("Total Bug Counts for Each Trap for a Night");
    title("Standardized Moon Phase vs. Total Bug Counts in either La Kretz Field Station and Stunt Ranch");
    show();

    return 0;
}
