#include <iostream>
#include <vector>
#include "system.h"

using namespace std;

int main(int argc, char* argv[]) {
    int n, m;
	cout << "enter n and m with a space separating the numbers:";
    cin >> n >> m;

    vector<vector<double>> matrix;

    for(int i = 0; i < n; i++) {
        vector<double> temp;
        double entry;

        cout << "please enter the values for row " << i + 1 << " of the the system matrix" << endl;
        
        for(int i = 0; i < m + 1; i++) {
            cin >> entry;
            temp.push_back(entry);
        }

        if (temp.size() != m + 1)
            cout << "please try again." << endl;
        else 
            matrix.push_back(temp);
    }

    System one = System(n, m, matrix);
    one.solve();

    vector<double> sol = one.getSolution();

    if(one.getNumSolutions() == 0)
        cout << "no solutions" << endl;
    else {
        cout << one.getNumSolutions() << " solutions of ";
        for(int i = 0; i < sol.size(); i++) {
            cout << sol[i];
            if (i != sol.size() - 1)
                 cout << ", ";   
        }
        cout << endl;    
    }
}