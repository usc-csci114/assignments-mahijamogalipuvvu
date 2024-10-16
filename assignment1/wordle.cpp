#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>


using namespace std;

int main(int argc, char* argv[]) {
	string green = "\033[92m";
	string yellow = "\033[93m";
	string coloroff = "\033[0m";

    int numGuess = 0;
    bool valid = true;
    string tempWord;
    vector<string> possibleWordles;
    vector<string> board;

    ifstream ifile("wordlist.txt");

    if(ifile.fail()) 
        "error: unable to open file";

    while(ifile >> tempWord)
        possibleWordles.push_back(tempWord);

    srand((unsigned) time(NULL));
	int random = rand() % possibleWordles.size();
    string answer = possibleWordles[random];

    cout << "hello! welcome to wordle!" << endl;
    bool isYellow = false;
    bool isGreen = false;;

    while(valid && numGuess < 7) {
        for(int i = 0; i < numGuess; i++) {
            for(int j = 0; j < 5; j++) {
                if(board[i][j] == answer[j]) {
                    cout << green << board[i][j];
                    isGreen = true;
                } else {
                    for(int k = 0; k < 5; k++) {
                        if(board[i][j] == answer[k]) {
                            cout << yellow << board[i][j];
                            isYellow = true;
                            break;
                        }
                    }

                    if(!isGreen && !isYellow) {
                        cout << coloroff << board[i][j];
                    }
                }
                isYellow = false;
                isGreen = false;
            } 
            cout << endl << coloroff;
        }

         for(int i = numGuess; i < 6; i++) {
            cout << "*****" << endl;
        }
        if(numGuess != 0 && board[numGuess - 1] == answer) {
            cout << "congrats! you did the wordle in " << (numGuess) << endl;
            valid = false;
            cout << "enter new for new game or quit to quit the program: ";
        } else {
            cout << "enter your guess: ";
        }

        cin >> tempWord;

        if(tempWord == "quit") {
            cout << "program quit!";
            valid = false;

        } else if(tempWord == "new") {
            cout << "restarting wordle..." << endl;
	        random = rand() % possibleWordles.size();
            answer = possibleWordles[random];
            board.clear();
            numGuess = 0;
            valid = true;
        } else if (tempWord.size() < 5 || tempWord.size() > 5) {
            cout << "unable to parse input. please try again!" << endl;
        } else if (find(possibleWordles.begin(), possibleWordles.end(), tempWord) == possibleWordles.end()) {
            cout << "invalid word. please try again!" << endl;
        } else {
            board.push_back(tempWord);
            numGuess++;
        }
    
    }


}







/*
#### Your Task

We have provided a list of 5 letter words to use for your game. When you program starts you should read this list of words into a `vector` of strings. The user will be able to play multiple times, so we'll need this list to pick the words.

Use a random number to pick one of the 5 letter words (for debugging at first you can just pick the first word). After  your program should automatically start a round by printing 6 lines of the '*' character 5 times and then prompting "Your guess?"

```
Welcome to Wordle!
*****
*****
*****
*****
*****
*****
Enter your guess:
```

At the prompt users can enter one of the following:

* "quit" to quit the program
* "new" to start a new round. Pick a new word and start over.
* or a 5 letter word as a guess

If the user enters anything other than the options above, ignore the input after printing a message (e.g "Unable to parse input, please try again.")

If the user enters a 5 letter "word", but it is not in the word list then print out a message (e.g. "Invalid word, please try again"). Can you write a function that finds a word in the word list in O(log n)?

##### Handling Guesses

To handle a guess you need to determine which letters in the guess are in the target word and if they are in the correct location as you print out the guess. One suggestion is to model the "board" as 6 strings that you modify after each guess. Each guess then only requires you to modify one string. Think about how to use string concatenation or a `stringstream` to do this. After each guess print all six strings, including the '*' placeholder strings for their upcoming guesses.

When building the string to print for a guess, letters that are in the correct location should be printed in green, letters that appear in the word but are in the incorrect place should be printed in yellow, otherwise the letter should be normal. See the `color.cpp` example program for help with printing green and yellow.

If the user gets the word correct, print the entire word in green and give the user the option for a new game or quit.

If the user guesses 6 times without getting the word correct, print the correct word after printing their last guess and then give the user the option for a new game or to quit.

#### Testing/Debugging

When testing/debugging feel free to hardcode the word. This will make it eaiser to figure out if your code is correct. Also, we've included a second file with only 10 words that will also make it eaiser to test/debug your code. Another idea is that once you switch to the big word list you print out the target word once before the game begins so you know what the answer is.


*/