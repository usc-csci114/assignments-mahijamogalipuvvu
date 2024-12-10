#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <utility>
#include <cctype>

using namespace std;

struct Element {
	Element() :
	symbol(),name(),number(0),atomic_mass(0.0),type() {};
	
	string symbol; //e.g "H", "He"
	string name; //e.g "Hydrogen", "Helium"
	int number; //e.g 1, 2
	double atomic_mass; //e.g. 1.008, 4.0026
	string phase; //phase at room temp e.g gas, liquid, solid
	string type; //e.g. "metalliod, "transitional metals"
};

struct compclass {
	bool operator() (pair<string, int> a, pair<string, int> b) { 
		return (a.first < b.first);
	}
} comp;

size_t index_find(vector<pair<string, int>> index, string key);
void printElementData(Element temp);

size_t index_find(vector<pair<string, int>> index, string key) {
	int low = 0;
	int high = index.size() - 1;

	while(low <= high) {
		int middle = (low + high)/2;

		if(index[middle].first == key) 
			return index[middle].second;
		else if (index[middle].first < key) {
			low = middle + 1;
		} else if (index[middle].first > key) {
			high = middle - 1;

		}
	}

	return -1;
}

void printElementData(Element temp) {
    cout << "name: " << temp.name << " (" << temp.symbol << ")" << endl;
	cout << "atomic number: " << temp.number << endl;
	cout << "atomic mass: " << temp.atomic_mass << endl;
	cout << "element type: " << temp.type << endl;
    cout << "phase @ room temp: " << temp.phase << endl;
    cout << "-----------------------------" << endl;

}

int main(int argc, char* argv[]) {
	vector<Element> elements(119);
	vector< pair<string, int> > name_index;
	vector< pair<string, int> > symbol_index;

	string input;

	ifstream ifile("elements.csv");

	if(ifile.fail()) {
		cout << "error: unable to open file";
		return 1;
	}

	getline(ifile, input);

	while(getline(ifile, input)) {
		replace(input.begin(), input.end(), ',', ' ');
		stringstream line(input);

		Element temp; 

		string num1, num2;

		line >> num1 >> temp.name >> temp.symbol >> num2 >> temp.phase >> temp.type;
		temp.number = stoi(num1);
		temp.atomic_mass = stod(num2);
		elements[temp.number] = temp;

		name_index.push_back({temp.name, temp.number});
		symbol_index.push_back({temp.symbol, temp.number});

	}

	sort(name_index.begin(), name_index.end(), comp);
	sort(symbol_index.begin(), symbol_index.end(), comp);

	cout << "to search for an element, either enter its number, symbol or name:" << endl;
	cin >> input;

	if(input == "quit") 
            cout << "program quit!";
	else {
		bool number = true;

		for(int i = 0; i < input.size(); i++) 
			if(!isdigit(input[i]))
				number = false;
		
		if(number) {
			printElementData(elements[stoi(input)]);
		} else if (input.size() < 3) {
			int index = index_find(symbol_index, input);

			if(index == -1) 
				cout << "not found";
			else
				printElementData(elements[index]);

		} else {
			int index = index_find(name_index, input);

			if(index == -1) 
				cout << "not found";
			else
				printElementData(elements[index]);
		}

	}

}
	


