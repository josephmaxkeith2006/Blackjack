#include "main.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

void startGame(double &balance) {

    /* DECLARATIONS */
    int option;

    vector<vector<string>> deck = DECK;

    double bet = 0;

    int hand = 0;

    vector<vector<string>> dealer_hand;
    vector<vector<string>> player_hand;
    vector<vector<string>> pair_hand;

    sum sums = {0, 0, 0};

    bool isFirstDeal = true;
    bool showDealerCard = false;
    vector<bool> hasAce = {false, false, false}; // i=0 is for the Player, i=1 is for the dealer, i=2 is if there is a pair split
    bool doubleDown = false;
    bool splittingPair = false;

    string suit;

    /* START */
    player_hand.push_back(drawCard(deck)); // Game starts with the dealer dealing a card to the player and then the dealer
    dealer_hand.push_back(drawCard(deck)); // face up, and then dealing another card to the player and another card to 
    player_hand.push_back(drawCard(deck)); // the dealer face down.
    dealer_hand.push_back(drawCard(deck));

    if (dealer_hand[0][0] == "A") {
        sums.dealer = 11;
        hasAce[1] = true;
    } else sums.dealer = cardValue(dealer_hand[0][0]);

    if (player_hand[0][0] == "A") {
        sums.player = 11;
        hasAce[0] = true;
    } else sums.player = cardValue(player_hand[0][0]);

    // Showing the first state of the hands to the player before they bet (1 card shown per hand)
    cout << "\nStarting Deal\n"
         << "-------------\n" << endl;
    displayCards(bet, dealer_hand, player_hand, pair_hand, sums, hasAce, isFirstDeal, showDealerCard, splittingPair);
    isFirstDeal = false;

    while (true) {
        cout << "Enter your bet to continue: $";
        cin >> bet;
        if (balance-bet >= 0) {
            balance -= bet;
            break;
        }
        cout << "\nInsufficent funds.\n" << endl;
    }

    /* PLAYER HANDS */
    while (true) {

        // Calculating player's hand sum
        sums.player = 0;
        for (size_t i=0; i<player_hand.size(); i++)
            sums.player += cardValue(player_hand[i][0]); // sum excludes the value of ace
        for (size_t i=0; i<player_hand.size(); i++) {
            if (player_hand[i][0] == "A") {
                hasAce[0] = true;
                if (sums.player + 11 <= 21)
                    sums.player += 11;
                else
                    sums.player += 1;
            }
        }

        ++hand;
        cout << "\nHand #" << hand
             << "\n-------\n" << endl;
        displayCards(bet, dealer_hand, player_hand, pair_hand, sums, hasAce, isFirstDeal, showDealerCard, splittingPair);

        if (sums.player > 21) { // Bust condition
            cout << "BUST" << endl;
            cout << "You lost..." << endl;
            cout << "Your bet will be taken." << endl;
            if (splittingPair) { // If player split and lost their first hand
                sums.pair = split(balance, bet, dealer_hand, player_hand, pair_hand, sums, hasAce, deck);
                if (sums.pair > 21)
                    return; // If player loses both hands to a bust
                else
                    break;
            } else
                return;
        }

        if (doubleDown) { // Ends play after one drawing if the player doubled down
            cout << "You have double downed.\n"
                 << "The dealer will now start drawing.\n" << endl;
            cout << "Press any number to continue: ";
            cin >> option;
            break;
        }

        // for conditions that can only occur on the first hand
        if (hand == 1) {
            if (sums.player == 21) { // Natural blackjack condition
                cout << "NATURAL BLACKJACK" << endl;
                cout << "You Won!" << endl;
                cout << "2.5x return on your bet." << endl;
                balance += bet*2.5;
                return;
            }
            if (9 <= sums.player && sums.player <= 11) // Double down condition
                doubleDown = true;
            if (player_hand[0][0] == player_hand[1][0]) // Split pair condition
                splittingPair = true;
        }

        // Mini-menu
        while (true) {
            cout << "Actions\n"
                 << "-------\n"
                 << "1. Hit\n"
                 << "2. Stand\n"
                 << "3. Double Down" << (doubleDown&&(hand==1) ? "\n" : " (Unavailable)\n")
                 << "4. Split Pair" << (splittingPair&&(hand==1) ? "\n" : " (Unavailable)\n") << endl;
            cout << "Enter your option (1 - 4): ";
            cin >> option;

            if (option == 1) {
                if (hand == 1)
                    splittingPair = false;
                break;
            } else if (option == 2) {
                break;
            } else if (option == 3) {
                if (doubleDown && hand == 1) {
                    if (balance-bet >= 0) break; // Checking for funds
                    cout << "\nInsufficent funds.\n" << endl;
                } else
                    cout << "\nDoubling down is available only on your first hand\n"
                         << "if your first deal's total sum is 9, 10, or 11.\n" << endl;
            } else if (option == 4) {
                if (splittingPair && hand == 1) {
                    if (balance-bet >= 0) break; // Checking for funds
                    cout << "\nInsufficent funds.\n" << endl;
                } else
                    cout << "\nYou can only split a pair on your first hand if\n"
                         << "you are dealt two cards of the same rank.\n" << endl;
            } else
                cout << "\nInvalid choice!\n" << endl;
        }

        // Hit
        if (option == 1) {
            player_hand.push_back(drawCard(deck));
        }
        // Stand
        if (option == 2) {
            if (splittingPair)
                sums.pair = split(balance, bet, dealer_hand, player_hand, pair_hand, sums, hasAce, deck);
            else {
                cout << "\nStanding..." << endl;
                cout << "The dealer will now start drawing.\n" << endl;
            }
            cout << "Press any number to continue: ";
            cin >> option;
            break;
        }
        // Double Down
        if (option == 3) {
            balance -= bet;
            bet *= 2;
            cout << "\nDoubling down...\n"
                 << "Your bet is now double." << endl;
            player_hand.push_back(drawCard(deck));
            continue;
        }
        // Splitting Pair
        if (option == 4) {
            balance -= bet;
            cout << "\nSplitting pair...\n"
                 << "Your bet has now doubled and split." << endl;

            sums.pair = cardValue(player_hand[1][0]);
            if (player_hand[0][0] == "A") {
                hasAce[2] = true;
                sums.pair = 11;
            }

            pair_hand.push_back(player_hand[1]);
            vector<string> hold = player_hand[0];
            player_hand.clear();
            player_hand.push_back(hold);
        }

        // Reset
        doubleDown = false;
    }

    /* DEALER HANDS */
    showDealerCard = true;
    while (true) {
        // Calculating the dealer's card sum
        sums.dealer = 0;
        for (size_t i=0; i<dealer_hand.size(); i++)
            sums.dealer += cardValue(dealer_hand[i][0]); // sum excludes the value of ace
        for (size_t i=0; i<dealer_hand.size(); i++) {
            if (dealer_hand[i][0] == "A") {
                hasAce[1] = true;
                if (sums.dealer + 11 <= 21)
                    sums.dealer += 11;
                else
                    sums.dealer += 1;
            }
        }

        cout << "\nDealer's Draw\n"
             << "-------------\n" << endl;
        displayCards(bet, dealer_hand, player_hand, pair_hand, sums, hasAce, isFirstDeal, showDealerCard, splittingPair);

        /* FINAL OUTCOME LOGIC */
        // No split pair play or bust on split pair
        if (sums.pair == 0 || sums.pair > 21) {
            if (sums.dealer > 21) {
                if (sums.pair > 21) cout << "Normal Hand\n-----------" << endl;
                cout << "HOUSE BUST" << endl;
                cout << "You Won!" << endl;
                cout << "2x return on your bet." << endl;
                balance += bet*2;
                return;
            }
            if (sums.dealer > 16 || (sums.dealer <= 16 && sums.dealer >= sums.player)) {
                if (sums.pair > 21) cout << "Normal Hand\n-----------" << endl;
                cout << "HOUSE STAND" << endl;
                if (sums.dealer < sums.player) {
                    cout << "You Won!" << endl;
                    cout << "2x return on your bet." << endl;
                    balance += bet*2;
                    return;
                } else if (sums.dealer == sums.player) {
                    cout << "Draw." << endl;
                    cout << "Your bet will be returned." << endl;
                    balance += bet;
                    return;
                } else if (sums.dealer > sums.player) {
                    cout << "You lost..." << endl;
                    cout << "Your bet will be taken." << endl;
                    return;
                }
            }
        }
        // Stand on pair play and bust on first play
        if (0 < sums.pair && sums.pair <= 21 && sums.player > 21) {
            if (sums.dealer > 21) {
                cout << "Split Hand\n----------" << endl;
                cout << "HOUSE BUST" << endl;
                cout << "You Won!" << endl;
                cout << "2x return on your bet." << endl;
                balance += bet*2;
                return;
            }
            if (sums.dealer > 16 || (sums.dealer <= 16 && sums.dealer >= sums.pair)) {
                cout << "Split Hand\n----------" << endl;
                cout << "HOUSE STAND" << endl;
                if (sums.dealer < sums.pair) {
                    cout << "You Won!" << endl;
                    cout << "2x return on your bet." << endl;
                    balance += bet*2;
                    return;
                } else if (sums.dealer == sums.pair) {
                    cout << "Draw." << endl;
                    cout << "Your bet will be returned." << endl;
                    balance += bet;
                    return;
                } else if (sums.dealer > sums.pair) {
                    cout << "You lost..." << endl;
                    cout << "Your bet will be taken." << endl;
                    return;
                }
            }
        }
        // both pair hands and first hands stand
        if (0 < sums.pair && sums.pair <= 21 && sums.player <= 21) {
            if (sums.dealer > 21) {
                cout << "Both Hands" << endl;
                cout << "----------" << endl;
                cout << "HOUSE BUST" << endl;
                cout << "You won on both hands!" << endl;
                cout << "2x return on your bets." << endl;
                balance += bet*4;
                return;
            }
            if (sums.dealer > 16) {
                cout << "Normal Hand\n-----------" << endl;
                cout << "HOUSE STAND" << endl;
                if (sums.dealer < sums.player) {
                    cout << "You Won!" << endl;
                    cout << "2x return on your bet." << endl;
                    balance += bet*2;
                } else if (sums.dealer == sums.player) {
                    cout << "Draw." << endl;
                    cout << "Your bet will be returned." << endl;
                    balance += bet;
                } else if (sums.dealer > sums.player) {
                    cout << "You lost..." << endl;
                    cout << "Your bet will be taken." << endl;
                }
                cout << "\nSplit Hand\n----------" << endl;
                cout << "HOUSE STAND" << endl;
                if (sums.dealer < sums.pair) {
                    cout << "You Won!" << endl;
                    cout << "2x return on your bet." << endl;
                    balance += bet*2;
                    return;
                } else if (sums.dealer == sums.pair) {
                    cout << "Draw." << endl;
                    cout << "Your bet will be returned." << endl;
                    balance += bet;
                    return;
                } else if (sums.dealer > sums.pair) {
                    cout << "You lost..." << endl;
                    cout << "Your bet will be taken." << endl;
                    return;
                }
            }
        }

        cout << "Enter any number to continue: ";
        cin >> option;
        dealer_hand.push_back(drawCard(deck));
    }

    return;
}

int split(double &balance, double bet, vector<vector<string>> dealer_hand, vector<vector<string>> player_hand, vector<vector<string>> &pair_hand, sum sums, vector<bool> hasAce, vector<vector<string>> &deck) {

    /* DECLARATIONS */
    int option;

    int hand = 0;

    bool showDealerCard = false;

    /* PAIR HANDS */
    cout << "\nYou have split your pair.\n"
         << "Play on the split will now begin.\n" << endl;
    cout << "Enter any number to continue: ";
    cin >> option;

    while (true) {
        // Calculating player's hand sum
        sums.pair = 0;
        for (size_t i=0; i<pair_hand.size(); i++)
            sums.pair += cardValue(pair_hand[i][0]); // sum excludes the value of ace
        for (size_t i=0; i<pair_hand.size(); i++) {
            if (pair_hand[i][0] == "A") {
                hasAce[2] = true;
                if (sums.pair + 11 <= 21)
                    sums.pair += 11;
                else
                    sums.pair += 1;
            }
        }

        ++hand;
        cout << "\nHand #" << hand
             << "\n-------\n" << endl;
        displayCards(bet, dealer_hand, player_hand, pair_hand, sums, hasAce, false, showDealerCard, true);

        if (sums.pair > 21){
            cout << "BUST" << endl;
            cout << "You lost..." << endl;
            cout << "Your bet will be taken." << endl;
            balance -= bet;
            return sums.pair;
        }

        while (true) {
            cout << "Actions\n"
                 << "-------\n"
                 << "1. Hit\n"
                 << "2. Stand\n" << endl;
            cout << "Enter your option (1 - 2): ";
            cin >> option;

            if (option == 1)
                break;
            else if (option == 2)
                break;
            else
                cout << "\nInvalid choice!\n" << endl;
        }

        // Hit
        if (option == 1) {
            pair_hand.push_back(drawCard(deck));
        }
        // Stand
        if (option == 2) {
            cout << "\nStanding..." << endl;
            cout << "The dealer will now start drawing.\n" << endl;
            return sums.pair;
        }
    }

    return 0;
}

void displayCards(double bet, vector<vector<string>> dealer_hand, vector<vector<string>> player_hand, vector<vector<string>> pair_hand, sum sums, vector<bool> hasAce, bool isFirstHand, bool showDealerCard, bool splittingPair) {

    /* DEALER'S HANDS */
    // for dealer play when the hidden card is revealed
    if (showDealerCard) {
        cout << "Dealer's Hand:" << endl;
        for (size_t i=0; i<dealer_hand.size(); i++) cout << " ___  ";
        cout << endl;
        for (size_t i=0; i<dealer_hand.size(); i++) cout << "|" << dealer_hand[i][0] << (dealer_hand[i][0] == "10" ? " | " : "  | ");
        cout << endl;
        for (size_t i=0; i<dealer_hand.size(); i++) cout << "| " << dealer_hand[i][1] << " | ";
        cout << endl;
        for (size_t i=0; i<dealer_hand.size(); i++) cout << (dealer_hand[i][0] == "10" ? "|_" : "|__") << dealer_hand[i][0] << "| ";
        cout << endl;
        cout << "\nSum: " << sums.dealer << (hasAce[1] ? "*" : "")  << "\n" << endl;
    }
    // for from the first draw up to the dealer's play
    else {
        cout << "Dealer's Hand:" << endl;
        cout << " ___   ___ " << endl;
        cout << "|" << dealer_hand[0][0] << (dealer_hand[0][0]=="10"?" |":"  |") << " | X |" << endl;
        cout << "| " << dealer_hand[0][1] << " | |X X|" << endl;
        cout << (dealer_hand[0][0]=="10"?"|_":"|__") << dealer_hand[0][0] << "| |_X_|\n" << endl;
        cout << "Sum: " << sums.dealer << (hasAce[1] ? "*" : "") << " + ???\n" << endl;
    }

    /* PLAYER'S HANDS */
    // For the first draw
    if (isFirstHand) {
        cout << "Your Hand:" << endl;
        cout << " ___   ___ " << endl;
        cout << "|" << player_hand[0][0] << (player_hand[0][0] == "10" ? " |" : "  |") << " | X |" << endl;
        cout << "| " << player_hand[0][1] << " | |X X|" << endl;
        cout << (player_hand[0][0] == "10" ? "|_" : "|__") << player_hand[0][0] << "| |_X_|\n" << endl;
        cout << "Sum: " << sums.player << (hasAce[0] ? "*" : "") << " + ???\n" << endl;
    }
    // For any draw after the first draw
    else {
        cout << "Your Hand:" << endl;
        cout << "Bet = $" << fixed << setprecision(2) << bet << endl;
        for (size_t i=0; i<player_hand.size(); i++) cout << " ___  ";
        cout << endl;
        for (size_t i=0; i<player_hand.size(); i++) cout << "|" << player_hand[i][0] << (player_hand[i][0] == "10" ? " | " : "  | ");
        cout << endl;
        for (size_t i=0; i<player_hand.size(); i++) cout << "| " << player_hand[i][1] << " | ";
        cout << endl;
        for (size_t i=0; i<player_hand.size(); i++) cout << (player_hand[i][0] == "10" ? "|_" : "|__") << player_hand[i][0] << "| ";
        cout << endl;
        cout << "\nSum: " << sums.player << (hasAce[0] ? "*" : "") << "\n" << endl;
    }

    /* PLAYER'S SECOND HAND */
    if (splittingPair) {
        cout << "Split Hand:" << endl;
        cout << "Bet = $" << fixed << setprecision(2) << bet << endl;
        for (size_t i=0; i<pair_hand.size(); i++) cout << " ___  ";
        cout << endl;
        for (size_t i=0; i<pair_hand.size(); i++) cout << "|" << pair_hand[i][0] << (pair_hand[i][0] == "10" ? " | " : "  | ");
        cout << endl;
        for (size_t i=0; i<pair_hand.size(); i++) cout << "| " << pair_hand[i][1] << " | ";
        cout << endl;
        for (size_t i=0; i<pair_hand.size(); i++) cout << (pair_hand[i][0] == "10" ? "|_" : "|__") << pair_hand[i][0] << "| ";
        cout << endl;
        cout << "\nSum: " << sums.pair << (hasAce[2] ? "*" : "") << "\n" << endl;
    }

    return;
}

int cardValue(string rank) {
    int value;

    if (rank == "A") value = 0; // ace value calculated outside of this function
    else if (rank == "2") value = 2;
    else if (rank == "3") value = 3;
    else if (rank == "4") value = 4;
    else if (rank == "5") value = 5;
    else if (rank == "6") value = 6;
    else if (rank == "7") value = 7;
    else if (rank == "8") value = 8;
    else if (rank == "9") value = 9;
    else if (rank == "10") value = 10;
    else if (rank == "J") value = 10;
    else if (rank == "Q") value = 10;
    else if (rank == "K") value = 10;

    return value;
}

vector<string> drawCard(vector<vector<string>> &deck) {

    int card_index = rand()%(deck.size());
    vector<string> card = deck[card_index];

    deck.erase(deck.begin()+card_index); // removes card from var deck until re-shuffled

    return card;
}
