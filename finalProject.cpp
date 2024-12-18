#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <matplot/matplot.h>

using namespace std;
using namespace matplot;

struct healthData {
    string country, diseaseCategory;
    double mortalityRate, healthcareAccess;
};

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
    string fileName = "Global Health Statistics.csv";

    ifstream file(fileName);
    if (!file.is_open()) {
        cerr << "error! can't open file " << "\n";
        return 1;
    }
    
    vector<healthData> data;
    string line;

    getline(file, line);
    while (getline(file, line)) {
        stringstream ss(line);
    
        string temp, country, disease, treatment;
        double year, mortality, healthcare, recoveryRate;

        getline(ss, country, ',');
        getline(ss, temp, ',');

        year = stoi(temp);

        int index = 2;

        while(getline(ss, temp, ',')) {
            if(index == 3) {
                if(temp == "Respiratory" || temp == "Parasitic" || temp == "Bacterial" || temp == "Viral" || temp == "Infectious")
                    disease = "Infectious";
                else   
                    disease = "Non-Communicable";

            } else if(index == 6) {
                mortality = stod(temp);
            } else if(index == 10) {
                healthcare = stod(temp);
            } else if (index == 15) {
                treatment = temp;
            } else if (index == 16) {
                recoveryRate = stod(temp);
            }   

                index++;
            }
            if(year == 2024 && treatment == "Yes" && recoveryRate >= 86.8) 
                data.push_back({country, disease, mortality, healthcare});
        
    }

    vector<double> healthcareInfectious, mortalityInfectious;
    vector<double> healthcareNCD, mortalityNCD;

    map<string, pair<double, int>> countryMortalityInfectious;
    map<string, pair<double, int>> countryMortalityNCD;


    for (const auto& row : data) {
        if (row.diseaseCategory == "Infectious") {
            healthcareInfectious.push_back(row.healthcareAccess);
            mortalityInfectious.push_back(row.mortalityRate);

            countryMortalityInfectious[row.country].first += row.mortalityRate;
            countryMortalityInfectious[row.country].second++;               

        } else {
            healthcareNCD.push_back(row.healthcareAccess);
            mortalityNCD.push_back(row.mortalityRate);

            countryMortalityNCD[row.country].first += row.mortalityRate;
            countryMortalityNCD[row.country].second++; 
        }
    }

    vector<double> healthcare(healthcareInfectious);
    healthcare.insert(healthcare.end(), healthcareNCD.begin(), healthcareNCD.end());

    vector<double> mortality(mortalityInfectious);
    mortality.insert(mortality.end(), mortalityNCD.begin(), mortalityNCD.end());

    figure();
    hist(healthcare, 20); 
    title("Distribution of Healthcare Access");
    xlabel("Healthcare Access (%)");
    ylabel("Frequency");
    show();

    figure();
    hist(mortality, 20); 
    title("Distribution of Mortality Rate");
    xlabel("Mortality Rate (%)");
    ylabel("Frequency");
    show();


    vector<string> countries;
    vector<vector<double>> graphData(2, vector<double>()); 
    vector<double> x;

    for (const auto& data : countryMortalityInfectious) {
        string country = data.first;
        countries.push_back(country);

        graphData[0].push_back(countryMortalityInfectious[country].first / countryMortalityInfectious[country].second);
        graphData[1].push_back(countryMortalityNCD[country].first / countryMortalityNCD[country].second);
    }

    for (int i = 1; i <= countries.size();i++) 
        x.push_back(i); 
    
    figure();
    bar(graphData);
    xticks(x);
    xticklabels(countries);
    xtickangle(60);
    xlabel("Countries");
    ylabel("Average Mortality Rate");
    title("Average Mortality Rates by Country");
    matplot::legend({"Infectious", "Non-Communicable Diseases"});    //NOT WORKING- same issue as exam 
    show();

    auto [slopeInterceptInfectious, rSquaredInfectious] = linearRegression(healthcareInfectious, mortalityInfectious);
    double slopeInfectious = slopeInterceptInfectious.first;
    double interceptInfectious = slopeInterceptInfectious.second;
    vector<double> trendInfectiousX = linspace(*min_element(healthcareInfectious.begin(), healthcareInfectious.end()), *max_element(healthcareInfectious.begin(), healthcareInfectious.end()), 25);
    vector<double> trendInfectiousY(trendInfectiousX.size());
    transform(trendInfectiousX.begin(), trendInfectiousX.end(), trendInfectiousY.begin(), [slopeInfectious, interceptInfectious](double x) {return slopeInfectious * x + interceptInfectious;});
    string str =  "y = " + to_string(slopeInfectious) + "x + " + to_string(interceptInfectious) + "  (R-squared: " + to_string(rSquaredInfectious) + ")";

    figure();
    hold(on);
    scatter(healthcareInfectious, mortalityInfectious, 2.0);
    plot(trendInfectiousX, trendInfectiousY);
    title("Healthcare Access (%) vs Mortality Rate (%) for Infectious Diseases");
    xlabel("Healthcare Access (%)");
    ylabel("Mortality Rate (%)");
    matplot::legend({"Data from Kaggle", str});
    show();

    auto [slopeInterceptNCD, rSquaredNCD] = linearRegression(healthcareNCD, mortalityNCD);
    double slopeNCD = slopeInterceptNCD.first;
    double interceptNCD = slopeInterceptNCD.second;
    vector<double> trendNCDX = linspace(*min_element(healthcareNCD.begin(), healthcareNCD.end()), *max_element(healthcareNCD.begin(), healthcareNCD.end()), 25);
    vector<double> trendNCDY(trendNCDX.size());
    transform(trendNCDX.begin(), trendNCDX.end(), trendNCDY.begin(), [slopeNCD, interceptNCD](double x) {return slopeNCD * x + interceptNCD;});
    str =  "y = " + to_string(slopeNCD) + "x + " + to_string(interceptNCD) + "  (R-squared: " + to_string(rSquaredNCD) + ")";

    figure();
    hold(on);
    scatter(healthcareNCD, mortalityNCD, 2.0);
    plot(trendNCDX, trendNCDY);
    title("Healthcare Access (%) vs Mortality Rate (%) for Non-Communicable Diseases");
    xlabel("Healthcare Access (%)");
    ylabel("Mortality Rate (%)");
    matplot::legend({"Data from Kaggle", str});
    show();

    return 0;
}