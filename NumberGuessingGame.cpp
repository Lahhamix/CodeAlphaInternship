#include <iostream>
#include <cstdlib>  
#include <ctime>     
#include <limits>    
#include <vector>
#include <string>

using namespace std;

int main() {
    
    srand(static_cast<unsigned int>(time(nullptr)));

    
    const int MIN_NUMBER = 1;
    const int MAX_NUMBER = 100;

    
    int targetNumber = rand() % MAX_NUMBER + MIN_NUMBER;

    int guess = 0;          
    int attempts = 0;       

    
    vector<std::string> lowHints = {
        "Too low! Try again.",
        "Low! Keep going.",
        "Not quite there, it's higher.",
        "Increase your guess!",
        "That's too low."
    };

    vector<string> highHints = {
        "Too high! Try again.",
        "High! Keep going.",
        "Not quite there, it's lower.",
        "Decrease your guess!",
        "That's too high."
    };

    cout << "========================================================================================================================\n";
    cout << "                                             Welcome to the Number Guessing Game!\n";
    cout << "========================================================================================================================\n";
    cout << "I have selected a number between " << MIN_NUMBER
        << " and " << MAX_NUMBER << ".\n";
    cout << "Can you guess what it is?\n\n";

    
    while (true) {
        cout << "Enter your guess: ";
        cin >> guess;

        
        if (cin.fail()) {
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            cout << "Invalid input! Please enter an integer.\n\n";
            continue; 
        }

        attempts++; 

        if (guess < targetNumber) {
            
            int index = rand() % lowHints.size();
            cout << lowHints[index] << "\n\n";
        }
        else if (guess > targetNumber) {
            
            int index = rand() % highHints.size();
            cout << highHints[index] << "\n\n";
        }
        else {
            cout << "Congratulations! You guessed the number in "
                << attempts << " attempts.\n";
            break; 
        }
    }

    cout << "Thank you for playing!\n";
    return 0;
}
