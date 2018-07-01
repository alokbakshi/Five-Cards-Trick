#include <iostream>
#include <iomanip>
#include <utility>
#include <array>
#include <algorithm>
#include <cmath>
 
constexpr int CARDS_PER_SUIT = 13;
constexpr int CARDS_IN_HANDS = 5;
 
constexpr int SUITS = 4;
constexpr int CARDS = SUITS * CARDS_PER_SUIT;
 
enum Suit {
    CLUB = 0, DIAMOND, HEART, SPADE
};
 
enum Symbol {
    ACE = 0, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING
};
 
struct Card {
    Suit suit;
    Symbol symbol;
 
    friend std::ostream& operator <<(std::ostream& stream, const Card& card);
 
};
 
std::ostream& operator <<(std::ostream& stream, const Card& card) {
    const char* suit_text = "";
    const char* symbol_text = "";
 
    Symbol symbol = card.symbol;
    Suit suit = card.suit;
 
    switch (suit) {
    case Suit::CLUB: suit_text = "Club"; break;
    case Suit::DIAMOND: suit_text = "Diamond"; break;
    case Suit::HEART: suit_text = "Heart"; break;
    case Suit::SPADE: suit_text = "Spade"; break;
    }
 
    switch (symbol) {
    case Symbol::ACE: symbol_text = "Ace"; break;
    case Symbol::TWO: symbol_text = "Two"; break;
    case Symbol::THREE: symbol_text = "Three"; break;
    case Symbol::FOUR: symbol_text = "Four"; break;
    case Symbol::FIVE: symbol_text = "Five"; break;
    case Symbol::SIX: symbol_text = "Six"; break;
    case Symbol::SEVEN: symbol_text = "Seven"; break;
    case Symbol::EIGHT: symbol_text = "Eight"; break;
    case Symbol::NINE: symbol_text = "Nine"; break;
    case Symbol::TEN: symbol_text = "Ten"; break;
    case Symbol::JACK: symbol_text = "Jack"; break;
    case Symbol::QUEEN: symbol_text = "Queen"; break;
    case Symbol::KING: symbol_text = "King"; break;
    }
 
    stream << "Suit: " << std::setw(7) << suit_text << ",\tSymbol: " << std::setw(4) << symbol_text << "\n";
    return stream;
}
 
int get_code(const Card& card) {
    return (CARDS_PER_SUIT * card.suit) + card.symbol;
}
 
Card get_card(int code) {
    if ((code < 0) || (code >= 52))
        throw std::invalid_argument("Code must be an integer between 0 and 51 (inclusive)\n");
 
    Card result;
 
    result.suit = static_cast<Suit> (code / 13);
    result.symbol = static_cast<Symbol> (code % 13);
 
    return result;
}
 
bool compare_inc(const Card& card1, const Card& card2) {
    return (get_code(card1) < get_code(card2));
}
 
bool compare_dec(const Card& card1, const Card& card2) {
    return !compare_inc(card1, card2);
}
 
/* Sequence code is a number between 1 and 6 (inclusive) representing the
permutation of three input cards */
 
int seq_code(const Card& card1, const Card& card2, const Card& card3) {
 
    bool card1_less_card2 = compare_inc(card1, card2);
    bool card2_less_card3 = compare_inc(card2, card3);
    bool card1_less_card3 = compare_inc(card1, card3);
 
    int result = 0;
 
    if (card1_less_card2 && card2_less_card3 && card1_less_card3)
        result = 1;
    else if (card1_less_card2 && !card2_less_card3 && card1_less_card3)
        result = 2;
    else if (!card1_less_card2 && card2_less_card3 && card1_less_card3)
        result = 3;
    else if (card1_less_card2 && !card2_less_card3 && !card1_less_card3)
        result = 4;
    else if (!card1_less_card2 && card2_less_card3 && !card1_less_card3)
        result = 5;
    else if (!card1_less_card2 && !card2_less_card3 && !card1_less_card3)
        result = 6;
    else
        result = 0; // should not really happen!
 
    return result;
 
}
 
Card guess_card(const Card& card1, const Card& card2, const Card& card3, const Card& card4) {
 
    Card guess; 
 
    //Suit of unknown card is same as the suit of topmost card
    guess.suit = card1.suit;
 
    //Read the offset from permutation of remaining three cards
    int offset = seq_code(card2, card3, card4);
    guess.symbol = static_cast<Symbol>((card1.symbol - offset + CARDS_PER_SUIT) % CARDS_PER_SUIT);
 
    return guess;
 
}
 
/* Place the "card to be hidden" on top so that it is within six
units away from the second card on the pile */
 
void unknown_card_on_top(std::array<Card, CARDS_IN_HANDS>& cards) {
 
     std::sort(cards.begin(), cards.end(), [](const Card& card1, const Card&
          card2) {return card1.suit < card2.suit;});
 
     bool finished = false;
     for (int i = 0; (i < CARDS_IN_HANDS) && (!finished); ++i){
          for (int j = i + 1; (j < CARDS_IN_HANDS) && (!finished); ++j) {        
 
               if (cards[i].suit == cards[j].suit) {
                    std::swap(cards[0], cards[i]);
                    std::swap(cards[1], cards[j]);
                    finished = true;
               }
          }
     }  
 
     int diff = (cards[1].symbol - cards[0].symbol + CARDS_PER_SUIT) %
          CARDS_PER_SUIT;   
 
     if (diff > CARDS_PER_SUIT / 2) {
          std::swap(cards[0], cards[1]);
     }
 
}
 
/* Shuffle the card so that topmost card is the card to be hidden while
rest of the cards (in the same order) are to be given to the other magician. */
 
void shuffle_cards(std::array<Card, CARDS_IN_HANDS>& cards) {
 
    unknown_card_on_top(cards);
 
    int code = (cards[1].symbol - cards[0].symbol + CARDS_PER_SUIT) % CARDS_PER_SUIT;
 
    if(code <= 3)
        std::sort(cards.begin() + 2, cards.end(), compare_inc);
    else
        std::sort(cards.begin() + 2, cards.end(), compare_dec);
 
    switch (code) {
    case 1: break;
    case 2: std::swap(cards[CARDS_IN_HANDS - 2], cards[CARDS_IN_HANDS - 1]); break;
    case 3: std::swap(cards[CARDS_IN_HANDS - 3], cards[CARDS_IN_HANDS - 2]); break;
    case 4: std::swap(cards[CARDS_IN_HANDS - 3], cards[CARDS_IN_HANDS - 2]); break;
    case 5: std::swap(cards[CARDS_IN_HANDS - 2], cards[CARDS_IN_HANDS - 1]); break;
    case 6: break;
    }
 
}
 
int main() {
 
    std::cout << "\n\nFive Card Puzzle!\n\n";
 
    std::array<Card, CARDS_IN_HANDS> cards;
 
    /* Sample Assignment of Cards to Alice*/
 
    cards[0].suit = Suit::CLUB;
    cards[1].suit = Suit::HEART;
    cards[2].suit = Suit::DIAMOND;
    cards[3].suit = Suit::CLUB;
    cards[4].suit = Suit::SPADE;
 
    cards[0].symbol = Symbol::ACE;
    cards[1].symbol = Symbol::FIVE;
    cards[2].symbol = Symbol::JACK;
    cards[3].symbol = Symbol::QUEEN;
    cards[4].symbol = Symbol::KING;
 
    /************************************/
 
    std::cout << "Cards given to Alice: \n";
    for (Card& card : cards)
        std::cout << card;
 
    // Alice rearranges cards (while pretending to shuffle) using some algorithm, and hides the topmost card
    shuffle_cards(cards);
    std::cout << "\n\nHidden card by Alice: \n" << cards[0];
 
    std::cout << "\nCards given back to Bob: \n";
    // Cards available to Bob
    for (int i=1; i< CARDS_IN_HANDS; ++i)
        std::cout << cards[i];
 
    // Hidden Card (cards[0] originally) guessed by Bob
    Card guess = guess_card(cards[1], cards[2], cards[3], cards[4]); 
 
    std::cout << "\n\nGuessed Card:\n" << guess << std::endl;
 
    return (0);
 
}
