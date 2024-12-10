#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <matplot/matplot.h>

using namespace std;
using namespace matplot;

//adapted from: https://www.geeksforgeeks.org/sorting-a-map-by-value-in-c-stl/
bool cmp(pair<string, int>& a, pair<string, int>& b) {
    return a.second > b.second; 
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

    map<string, map<string, int>> lightTypeOrdersCounts; 
    map<string, int> insectOrderCounts; 

    while (getline(file, line)) {
        stringstream ss(line);
        vector<string> row;
        string temp;

        while (getline(ss, temp, ',')) {
            row.push_back(temp);
        }

        string lightType = row[0]; 
        vector<string> orders = {"Diptera", "Coleoptera", "Hymenoptera", "Lepidoptera", "Hemiptera", "Psocoptera", "Collembola", "Arachnid", "Thysanura", "Isoptera", "Neuroptera", "Larvae", "Orthoptera", "Unident", "Other Orders"};

        for (int i = 0; i < orders.size(); ++i) {
            int count = stoi(row[i + 17]); 
            lightTypeOrdersCounts[lightType][orders[i]] += count;
            insectOrderCounts[orders[i]] += count;
        }
    }
    file.close();

    vector<pair<string, int>> sortedOrders; 
    for (const pair<string, int>& temp : insectOrderCounts) {
        sortedOrders.push_back(temp); 
    }
    sort(sortedOrders.begin(), sortedOrders.end(), cmp);


    vector<string> mostCommonOrders;
    for (int i = 0; i < 4; ++i) {
        mostCommonOrders.push_back(sortedOrders[i].first);
    }

    vector<string> lightTypes;
    vector<vector<double>> data(4, vector<double>()); 

    for (const auto& [lightType, orders] : lightTypeOrdersCounts) {
        lightTypes.push_back(lightType); 
        for (int i = 0; i < 4; ++i) {
            string order = mostCommonOrders[i];
            if (orders.count(order)) 
                data[i].push_back(orders.at(order)); 
        }
    }

    hold(on);
    auto fig = bar(data);
    xticklabels(lightTypes);
    xlabel("Light Types");
    ylabel("Insect Count");
    title("Comparison of the Insect Counts of the Top 4 Most Common Insect Orders Attracted by Different Light Types");
    matplot::legend(mostCommonOrders);      //NOT WORKING
    show();

    return 0;
}
