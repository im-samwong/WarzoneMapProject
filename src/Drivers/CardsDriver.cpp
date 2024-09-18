#include "Cards.h"
#include <iostream>

void testCards() {
    Orders::OrderList orderList;
    Deck deck;
    Hand hand;

    std::cout << "Drawing 3 cards from the deck:" << std::endl;
    for (int i = 0; i < 3; i++) {
        Card* drawnCard = deck.draw();
        if (drawnCard) {
            std::cout << *drawnCard << std::endl;
            hand.addCard(drawnCard);
        }
    }

    std::cout << "\nPlaying all cards in hand:" << std::endl;
    while (!hand.getHandCards().empty()) {
        hand.playCard(hand.getHandCards()[0], &deck, &orderList);
    }

    std::cout << "\nRemaining orders in the list:" << std::endl;
    orderList.printOrders();
}

int main() {
    testCards();
    return 0;
}
