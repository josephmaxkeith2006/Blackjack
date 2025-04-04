// Programmer: Joseph Keith
// Date: 3/10/2025
// Last Update: 3/13/2025
// Files: main.cpp, functions.cpp, main.h
// Assignment: Homework 4
// Purpose: Create a terminal-level version of Blackjack in C++.

#include "main.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

int main() {

    srand(time(0)); // Randomizing seed according to time

    /* DECLARATIONS */
    int option;
    double balance = 100.00;

    /* MENU */
    while (true) {
        cout << "\nBlackjack!\n"
                << "----------\n"
                << "1. Play Game\n"
                << "2. View Balance\n"
                << "3. Exit\n" << endl;
        cout << "Enter your option (1-3): ";
        cin >> option;

        switch (option) {
            case 1:
                if (balance < 0) { // Kicking player out if they run out of money or are in debt.
                    cout << "\nYou have run out of funds.\n"
                         << "You have been cut off.\n"
                         << "Bye!\n" <<endl;
                    return 2;
                }
                startGame(balance);
                break;
            case 2:
                cout << "\nBalance: $" << fixed << setprecision(2) << balance << endl;
                break;
            case 3:
                cout << "\nBye!\n" << endl;
                return 1;
            default:
                cout << "\nInvalid choice!" << endl;
        }
    }

    return 0;
}
