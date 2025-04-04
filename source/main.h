#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <vector>

using namespace std;

/* CONSTANT VARIABLES */
// const DECK holds the 52 card standard deck in {"rank", "suit in their UTF-8 value"}
const vector<vector<string>> DECK = {
    {"A", u8"\u2660"}, {"A", u8"\u2663"}, {"A", u8"\u2665"}, {"A", u8"\u2666"},
    {"2", u8"\u2660"}, {"2", u8"\u2663"}, {"2", u8"\u2665"}, {"2", u8"\u2666"},
    {"3", u8"\u2660"}, {"3", u8"\u2663"}, {"3", u8"\u2665"}, {"3", u8"\u2666"},
    {"4", u8"\u2660"}, {"4", u8"\u2663"}, {"4", u8"\u2665"}, {"4", u8"\u2666"},
    {"5", u8"\u2660"}, {"5", u8"\u2663"}, {"5", u8"\u2665"}, {"5", u8"\u2666"},
    {"6", u8"\u2660"}, {"6", u8"\u2663"}, {"6", u8"\u2665"}, {"6", u8"\u2666"},
    {"7", u8"\u2660"}, {"7", u8"\u2663"}, {"7", u8"\u2665"}, {"7", u8"\u2666"},
    {"8", u8"\u2660"}, {"8", u8"\u2663"}, {"8", u8"\u2665"}, {"8", u8"\u2666"},
    {"9", u8"\u2660"}, {"9", u8"\u2663"}, {"9", u8"\u2665"}, {"9", u8"\u2666"},
    {"10", u8"\u2660"}, {"10", u8"\u2663"}, {"10", u8"\u2665"}, {"10", u8"\u2666"},
    {"J", u8"\u2660"}, {"J", u8"\u2663"}, {"J", u8"\u2665"}, {"J", u8"\u2666"},
    {"Q", u8"\u2660"}, {"Q", u8"\u2663"}, {"Q", u8"\u2665"}, {"Q", u8"\u2666"},
    {"K", u8"\u2660"}, {"K", u8"\u2663"}, {"K", u8"\u2665"}, {"K", u8"\u2666"}
}; // {u8"\u2605", "J"}, {u8"\u2605", "J"}

/* STRUCTURES */
// sum structure for easy access of sums of card hands
struct sum {
    int player;
    int dealer;
    int pair;
};

/* FUNCTION PROTOTYPES */

// Function Description: The main complete logic of blackjack.
// Parameters:
//      double & balance: The total balance of the user
// Post: n/a
void startGame(double &balance);

// Function Description: If the user chooses to split their cards when they draw a pair inicially,
//                       this function holds the logic for the second hand they the player plays on a split.
// Parameters:
//      double & balance: The total balance of the user
//      double bet: The user's bet for the game
//      vector<vector<string>> dealer_hand: Vector that holds the dealer's cards
//      vector<vector<string>> player_hand: Vector that holds the player's first hand of cards
//      vector<vector<string>> & pair_hand: Vector that holds the player's second hand of cards
//      sum sums: Custom struct type that contains all of the sums of the threee different hands
//      vector<bool> hasAce: Vector of Booleans that tell the display function if there is an ace in play. {player 1st, dealer, player 2nd}
//      vector<vector<string>> & deck: Vector that contains the current state of the deck
// Post: (int) the sum of the second play.
int split(double &balance, double bet, vector<vector<string>> dealer_hand, vector<vector<string>> player_hand, vector<vector<string>> &pair_hand, sum sums, vector<bool> hasAce, vector<vector<string>> &deck);

// Function Description: This function is responsible for the displaying logic of the mini-card icons, sums, and bets.
// Parameters:
//      double bet: The user's bet for the game
//      vector<vector<string>> dealer_hand: Vector that holds the dealer's cards
//      vector<vector<string>> player_hand: Vector that holds the player's first hand of cards
//      vector<vector<string>> pair_hand: Vector that holds the player's second hand of cards
//      sum sums: Custom struct type that contains all of the sums of the threee different hands
//      vector<bool> hasAce: Vector of Booleans that tell the display function if there is an ace in play. {player 1st, dealer, player 2nd}
//      bool isFirstHand: Boolean that is true when displaying the first hand of the game before the inicial bet
//      bool showDealerCard: Boolean that is true when the dealer reveals their flipped card and so on
//      bool splittingPair: Boolean that is true when the player chooses to split their play when they get a pair on the first hand
// Post: n/a
void displayCards(double bet, vector<vector<string>> dealer_hand, vector<vector<string>> player_hand, vector<vector<string>> pair_hand, sum sums, vector<bool> hasAce, bool isFirstHand, bool showDealerCard, bool splittingPair);

// Function Description: This function takes the rank of a card and gives back it's card value.
// Parameters:
//      string rank: The rank icon of the card as a string ("A", "8", "K", ect.)
// Post: (int) the value of the card in blackjack (0 if ace)
int cardValue(string rank);

// Function Description: This function takes in a deck, draws a card, and removes that card from the deck.
// Parameters:
//      vector<vector<string>> & deck: Vector that contains the current state of the deck
// Post: (vector<string>) the card that was drawn from the deck
vector<string> drawCard(vector<vector<string>> &deck);


#endif