#include "PlayerStrategies.h"

#include <bits/ranges_algo.h>

#include "../Player/Player.h"

Player* PlayerStrategy::getPlayer() const {
    return player;
}

PlayerStrategy::PlayerStrategy(Player* player, const std::string& description) {
    //No need to own this player just use the ptr
    this->player = player;
    this->description = new std::string(description);
}

PlayerStrategy& PlayerStrategy::operator=(const PlayerStrategy &other) {
    if (this != &other) {
        player = other.player;
    }
    return *this;
}

PlayerStrategy::PlayerStrategy(const PlayerStrategy &other) {
    player = other.player;
}

std::string PlayerStrategy::getDescription() const {
    return *description;
}

std::ostream& operator<<(std::ostream& os, const PlayerStrategy& strategy) {
    os << "PlayerStrategy: " << strategy.description << std::endl;
    os << "Player using this strategy: " << strategy.player->getName() << std::endl;

    return os;
}

AggressivePlayer::AggressivePlayer(Player* player): PlayerStrategy(player, "AggressivePlayer") {}

AggressivePlayer::~AggressivePlayer() {
    //DO NOT DELETE PLAYER AS WE DON'T OWN IT
    delete description;
}

/**
 * The AggressivePlayer's issueOrder logic is to reinforce a single owned country, with all of their available reinforcements, that has the most
 * neighbors it can attack as it will allow them to conquer as many as possible. If for whatever reason all the countries have the same amount
 * of neighbors then it will reinforce the first country in their owned territories.
 * @param players Unused for this strategy
 */
void AggressivePlayer::issueOrder(std::vector<Player *> *players) {
    std::vector<Territory*> playerTerritories = this->player->toDefend();
    std::vector<Territory*> territoriesToAttack = this->player->toAttack();
    const int reinforcements = this->player->getReinforcements();
    const int unitsToUse = reinforcements;

    int maxNeighbors = -1;
    for (const Territory* territory: playerTerritories) {
        if (territory->getNeighbors()->size() > maxNeighbors) {
            maxNeighbors = static_cast<int>(territory->getNeighbors()->size());
        }
    }

    const auto target = std::ranges::find_if(playerTerritories, [&maxNeighbors](const Territory* territory) {
        return territory->getNeighbors()->size() == maxNeighbors;
    });

    if(target != playerTerritories.end()) {
        this->player->getOrdersList()->addOrder(std::make_unique<DeployOrder>(this->player,*target,new int(unitsToUse)));
    } else {
        this->player->getOrdersList()->addOrder(std::make_unique<DeployOrder>(this->player,playerTerritories[0],new int(unitsToUse)));
    }
}

/**
 * Method used to implement the attack logic of the AggressivePlayer
 * @param territoriesToAttack List of territories that should be attacked
 */
void AggressivePlayer::toAttack(std::vector<Territory*> territoriesToAttack) {
    std::vector<Territory*> playerTerritories = this->player->toDefend();

    int maxNeighbors = -1;
    for (const Territory* territory: playerTerritories) {
        if (territory->getNeighbors()->size() > maxNeighbors) {
            maxNeighbors = static_cast<int>(territory->getNeighbors()->size());
        }
    }

    const auto sourceTerritory = std::ranges::find_if(playerTerritories, [&maxNeighbors](const Territory* territory) {
        return territory->getNeighbors()->size() == maxNeighbors;
    });

    if (sourceTerritory != playerTerritories.end()) {

    }

    std::vector<Card*> playerCards = this->player->getHand().getHandCards();
    for (Card* card: playerCards) {
        if (card->getTypeAsString() == "Reinforcement") {
            //Advance on a neighor of the source territory
            // this->player->getOrdersList()->addOrder(std::make_unique<AdvanceOrder>(this, *sourceTerritory, *target, new int(numOfUnits)));
        } else if (card->getTypeAsString() == "Bomb") {
            //Bomb one of the neighbors of the sourceTerritory until all neighbors are conquered
        } else if (card->getTypeAsString() == "Blockade") {
            //Purposely do a bad order to get rid of the card
        } else if (card->getTypeAsString() == "Airlift") {
            //Purposely do a bad order to get rid of the card??
        } else if (card->getTypeAsString() == "Diplomacy") {
            //Purposely do a bad order to get rid of the card
        }
    }
}

void AggressivePlayer::toDefend(std::vector<Territory*> playerTerritories) {
    //TODO:
}