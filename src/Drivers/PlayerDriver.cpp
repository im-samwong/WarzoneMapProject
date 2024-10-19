#include "../Cards/Cards.h"
#include "../Map/Map.h"
#include "../Orders/Orders.h"
#include "../Player/Player.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

void testPlayers() {
    // Create a player using the parameterized constructor (name)
    Player *player1 = new Player("Caitlin");

    Continent *sampleContinent = nullptr;

    Territory *t1 = new Territory("Iceland", 100, 200, sampleContinent);
    Territory *t2 = new Territory("Norway", 120, 220, sampleContinent);

    // Create a player using the parameterized constructor (name, territories)
    std::vector<Territory *> territories = {t1, t2};
    Player *player2 = new Player("Stephen", territories);

    // Create a dummy order and issue it
    std::unique_ptr<Order> order = std::make_unique<DeployOrder>();
    player2->issueOrder(std::move(order));

    // Print out player details (name, number of territories and list of orders)
    std::cout << *player1 << std::endl;
    std::cout << *player2 << std::endl;

    // Testing toAttack() method
    std::cout << "Player " << player2->getName() << "'s territories to attack: " << std::endl;
    std::vector<Territory *> toAttackList = player2->toAttack();
    for (Territory *territory : toAttackList) {
        std::cout << "- " << territory->getName() << std::endl; // returns arbitrary list of territories-- Assignment 1
    }
    std::cout << "\n"
              << std::endl;

    // Testing toDefend() method
    std::cout << "Player " << player2->getName() << "'s territories to defend: " << std::endl;
    std::vector<Territory *> toDefendList = player2->toDefend();
    for (Territory *territory : toDefendList) {
        std::cout << "- " << territory->getName() << std::endl;
    }
    std::cout << "\n"
              << std::endl;

    // Creating a deck and drawing cards
    Deck deck;
    player1->getHand().addCard(deck.draw());
    player1->getHand().addCard(deck.draw());

    // Printing cards
    std::cout << "Player " << player1->getName() << "'s hand:" << std::endl;
    std::vector<Card *> handCards = player1->getHand().getHandCards();
    for (Card *card : handCards) {
        std::cout << *card << std::endl;
    }

    // Clean up
    delete player1;
    delete player2;
}
