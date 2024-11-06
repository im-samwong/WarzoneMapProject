#include "Cards.h"
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()

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
        delete this->type; // Free existing memory before copying
        this->type = new CardType(*other.type);
    }
    return *this;
}

// Get the card type as a string
std::string Card::getTypeAsString() const {
    switch (*type) {
    case BOMB:
        return "Bomb";
    case REINFORCEMENT:
        return "Reinforcement";
    case BLOCKADE:
        return "Blockade";
    case AIRLIFT:
        return "Airlift";
    case DIPLOMACY:
        return "Diplomacy";
    default:
        return "Unknown";
    }
}

// Play the card and return it to the deck
void Card::play(OrderList* orderList) {
    // Play the card based on its type
    std::cout << "Playing card: " << getTypeAsString() << std::endl;
    // Send specific order to order list
    switch (*type) {
    case BOMB:
        std::cout << "Adding Order: Bomb" << std::endl;
        orderList->addOrder(std::make_unique<BombOrder>());
        break;
    case REINFORCEMENT:
        std::cout << "Adding Order: Reinforcement(Deploy)" << std::endl;
        orderList->addOrder(std::make_unique<DeployOrder>());
        break;
    case BLOCKADE:
        std::cout << "Adding Order: Blockade" << std::endl;
        orderList->addOrder(std::make_unique<BlockadeOrder>());
        break;
    case AIRLIFT:
        std::cout << "Adding Order: Airlift" << std::endl;
        orderList->addOrder(std::make_unique<AirliftOrder>());
        break;
    case DIPLOMACY:
        std::cout << "Adding Order: Diplomacy(Negotiate)" << std::endl;
        orderList->addOrder(std::make_unique<NegotiateOrder>());
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
    // Add cards of different types to the deck, 3 of each kind so players have enough to choose from.
    cards.push_back(new Card(Card::BOMB));
    cards.push_back(new Card(Card::BOMB));
    cards.push_back(new Card(Card::BOMB));
    cards.push_back(new Card(Card::REINFORCEMENT));
    cards.push_back(new Card(Card::REINFORCEMENT));
    cards.push_back(new Card(Card::REINFORCEMENT));
    cards.push_back(new Card(Card::BLOCKADE));
    cards.push_back(new Card(Card::BLOCKADE));
    cards.push_back(new Card(Card::BLOCKADE));
    cards.push_back(new Card(Card::AIRLIFT));
    cards.push_back(new Card(Card::AIRLIFT));
    cards.push_back(new Card(Card::AIRLIFT));
    cards.push_back(new Card(Card::DIPLOMACY));
    cards.push_back(new Card(Card::DIPLOMACY));
    cards.push_back(new Card(Card::DIPLOMACY));
}

// Copy constructor (deep copy)
Deck::Deck(const Deck& other) {
    for (auto card : other.cards) {
        cards.push_back(new Card(*card)); // Deep copy each card
    }
}

// Destructor
Deck::~Deck() {
    for (Card* card : cards) {
        delete card;
    }
}

// Assignment operator (deep copy)
Deck& Deck::operator=(const Deck& other) {
    if (this != &other) {
        for (Card* card : cards) {
            delete card;
        }
        cards.clear();

        for (auto card : other.cards) {
            cards.push_back(new Card(*card)); // Deep copy each card
        }
    }
    return *this;
}

// Get the cards in the deck
std::vector<Card*> Deck::getCards() {
    return cards;
}

// Draw a random card from the deck
Card* Deck::draw() {
    if (cards.empty())
        return nullptr;
    // Seed the random number generator with current time to ensure randomness
    srand(static_cast<unsigned int>(time(0)));

    int index = rand() % cards.size();
    Card* drawnCard = cards.at(index);
    cards.erase(cards.begin() + index);
    return drawnCard;
}

// Stream insertion operator for Deck
std::ostream& operator<<(std::ostream& out, const Deck& deck) {
    out << "Deck contains " << deck.cards.size() << " cards:";
    for (auto card : deck.cards) {
        out << "\n  - " << card->getTypeAsString();
    }
    return out;
}

// Hand Class Implementation
// Constructor
Hand::Hand() {}

// Copy constructor (deep copy)
Hand::Hand(const Hand& other) {
    for (auto card : other.handCards) {
        handCards.push_back(new Card(*card)); // Deep copy each card
    }
}

// Destructor
Hand::~Hand() {
    for (Card* card : handCards) {
        delete card;
    }
}

// Assignment operator (deep copy)
Hand& Hand::operator=(const Hand& other) {
    if (this != &other) {
        for (Card* card : handCards) {
            delete card;
        }
        handCards.clear();

        for (auto card : other.handCards) {
            handCards.push_back(new Card(*card)); // Deep copy each card
        }
    }
    return *this;
}

// Get the cards in the hand
std::vector<Card*> Hand::getHandCards() {
    return handCards;
}

void Hand::setHandCards(const std::vector<Card *>& newCards) {
    this->handCards = newCards;
}


// Add a card to the hand
void Hand::addCard(Card* card) {
    handCards.push_back(card);
}

// Play a card from the hand
void Hand::playCard(Card* card, Deck* deck, OrderList* orderList) {
    card->play(orderList); // Play the card

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

// Stream insertion operator for Hand
std::ostream& operator<<(std::ostream& out, const Hand& hand) {
    out << "Hand contains " << hand.handCards.size() << " cards:";
    for (auto card : hand.handCards) {
        out << "\n  - " << card->getTypeAsString();
    }
    return out;
}
