#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip> 


using namespace std;


int main() {
    string fileName = "bug-attraction.csv";
    //map<order, map<lighttype, totalCount>> orderLightType
    map<string, map<string, int>> orderLightType;
    vector<int> numbers(6, 0);

    ifstream file(fileName);
    if (!file.is_open()) {
        cerr << "error! can't open file " << "\n";
        return 1;
    }

    
    string line;

    getline(file, line);
    vector<string> orders = {"Diptera", "Hymenoptera", "Hemiptera", "Psocoptera", "Coleoptera", "Collembola", "Arachnid", "Thysanura", "Isoptera", "Lepidoptera", "Neuroptera", "Larave", "Orthoptera", "Unident", "Total"};

    while (getline(file, line)) {
        stringstream ss(line);
        string lightType, temp;
        vector <int> values;
        int index = 0;

        while(getline(ss, temp, ',')) {
            if(index == 0) {
                lightType = temp;
            } else if (index >= 17) {
                values.push_back(stoi(temp)); // Attempt to convert
            }
            index++;
        }

        for (int i = 0; i < orders.size(); i++) 
            orderLightType[orders[i]][lightType] += values[i];

    }


 

    file.close();

    cout << setw(20) << "Order" << setw(40) << "Most Attractive Light Type" << endl;
    cout << string(70, '-') << endl;

    for (map<string, map<string, int>>::iterator it = orderLightType.begin(); it != orderLightType.end(); ++it) {
        string order = it->first;                   
        map<string, int>& lightTypes = it->second; 

        string mostAttractiveLight = "";
        int maxCount = 0;

        for (map<string, int>::iterator innerIt = lightTypes.begin(); innerIt != lightTypes.end(); ++innerIt) {
            string lightType = innerIt->first; 
            int count = innerIt->second;      

            if (count > maxCount) {
                maxCount = count;
                mostAttractiveLight = lightType;
            }
        }

        cout << setw(20) << order << setw(30) << mostAttractiveLight << endl;;
    }


    return 0;
}







