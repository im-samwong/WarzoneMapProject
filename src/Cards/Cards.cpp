#include "Cards.h"
#include <cstdlib>  // For rand() and srand()
#include <ctime>  // For time()

namespace Cards {
    // Card Class Implementation
    // Constructor
    Card::Card(CardType type) {
        this->type = new CardType(type);
    }
    
    // Copy constructor
    Card::Card(const Card& other) {
        this->type = new CardType(*other.type);
    }
    
    // Destructor
    Card::~Card() {
        delete type;
    }
    
    // Assignment operator
    Card& Card::operator=(const Card& other) {
        if (this != &other) {
            delete this->type;  // Free existing memory before copying
            this->type = new CardType(*other.type);
        }
        return *this;
    }
    
    // Get the card type as a string
    std::string Card::getTypeAsString() const {
        switch (*type) {
            case BOMB: return "Bomb";
            case REINFORCEMENT: return "Reinforcement";
            case BLOCKADE: return "Blockade";
            case AIRLIFT: return "Airlift";
            case DIPLOMACY: return "Diplomacy";
            default: return "Unknown";
        }
    }
    
    // Play the card and return it to the deck
    void Card::play(Orders::OrderList* orderList) {
        // Play the card based on its type
        std::cout << "Playing card: " << getTypeAsString() << std::endl;
        // Send specific order to order list
        switch (*type) {
            case BOMB:
                std::cout << "Adding Order: Bomb" << std::endl;
                orderList->addOrder(std::make_unique<Orders::BombOrder>());
                break;
            case REINFORCEMENT:
                std::cout << "Adding Order: Reinforcement(Deploy)" << std::endl;
                orderList->addOrder(std::make_unique<Orders::DeployOrder>());
                break;
            case BLOCKADE:
                std::cout << "Adding Order: Blockade" << std::endl;
                orderList->addOrder(std::make_unique<Orders::BlockadeOrder>());
                break;
            case AIRLIFT:
                std::cout << "Adding Order: Airlift" << std::endl;
                orderList->addOrder(std::make_unique<Orders::AirliftOrder>());
                break;
            case DIPLOMACY:
                std::cout << "Adding Order: Diplomacy(Negotiate)" << std::endl;
                orderList->addOrder(std::make_unique<Orders::NegotiateOrder>());
                break;
            default:
                break;
        }
    }
    
    // Overloaded stream operator to print card details
    std::ostream& operator<<(std::ostream& out, const Card& card) {
        out << "Card type: " << card.getTypeAsString();
        return out;
    }
    
    // Deck Class Implementation
    // Constructor
    Deck::Deck() {
        // Add cards of different types to the deck
        cards.push_back(new Card(Card::BOMB));
        cards.push_back(new Card(Card::REINFORCEMENT));
        cards.push_back(new Card(Card::BLOCKADE));
        cards.push_back(new Card(Card::AIRLIFT));
        cards.push_back(new Card(Card::DIPLOMACY));
    }
    
    // Destructor
    Deck::~Deck() {
        for (Card* card : cards) {
            delete card;
        }
    }
    
    // Get the cards in the deck
    std::vector<Card*> Deck::getCards() {
        return cards;
    }
    
    // Draw a random card from the deck
    Card* Deck::draw() {
        if (cards.empty()) return nullptr;
        // Seed the random number generator with current time to ensure randomness
        srand(static_cast<unsigned int>(time(0)));
    
        int index = rand() % cards.size();
        Card* drawnCard = cards.at(index);
        cards.erase(cards.begin() + index);
        return drawnCard;
    }
    
    // Hand Class Implementation
    // Constructor
    Hand::Hand() {}
    
    // Destructor
    Hand::~Hand() {
        for (Card* card : handCards) {
            delete card;
        }
    }
    
    // Get the cards in the hand
    std::vector<Card*> Hand::getHandCards() {
        return handCards;
    }
    
    // Add a card to the hand
    void Hand::addCard(Card* card) {
        handCards.push_back(card);
    }
    
    // Play a card from the hand
    void Hand::playCard(Card* card, Deck* deck, Orders::OrderList* orderList) {
    card->play(orderList);  // Play the card
    
    // Iterate over handCards and find the matching card
    for (auto it = handCards.begin(); it != handCards.end(); ++it) {
        if (*it == card) {
            // Remove the card from handCards
            handCards.erase(it);
            break; // Break the loop once the card is found and erased
        }
    }
    
    // Return the card to the deck
    deck->getCards().push_back(card);
}

}
