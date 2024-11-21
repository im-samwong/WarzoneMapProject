#include "PlayerStrategies.h"

#include <set>

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
    std::cout << "AggressivePlayer:: Has started issuing orders" << std::endl;
    const int reinforcements = this->player->getReinforcements();
    const int unitsToUse = reinforcements;

    Territory* territoryToReinforce = this->getSourceTarget(this->player->toDefend());
    //Adds a singular order to reinforce one country with which it will push through
    this->player->getOrdersList()->addOrder(std::make_unique<DeployOrder>(this->player,territoryToReinforce,new int(unitsToUse)));

    //Once the deploy order is out we now proceed with the rest
    std::vector<Territory*> territoriesToAttack = this->toAttack();

    Territory* sourceTerritory = getSourceTarget(this->toDefend());
    Territory* targetTerritory = getAttackTarget(sourceTerritory, territoriesToAttack);

    std::vector<Card*> playerCards = this->player->getHand().getHandCards();
    for (Card* card: playerCards) {
        if (card->getTypeAsString() == "Reinforcement") {
            //Advance on a neighbor of the source territory with full force i.e use all available units
            this->player->getOrdersList()->addOrder(std::make_unique<AdvanceOrder>(this->player, sourceTerritory, targetTerritory, new int(sourceTerritory->getArmies())));
        } else if (card->getTypeAsString() == "Bomb") {
            //Bomb one of the neighbors of the sourceTerritory until all neighbors are conquered
            this->player->getOrdersList()->addOrder(std::make_unique<BombOrder>(this->player, sourceTerritory));
        } else if (card->getTypeAsString() == "Blockade") {
            //Purposely do a bad order to get rid of the card an aggressive player would never give up a territory
            this->player->getOrdersList()->addOrder(std::make_unique<BlockadeOrder>(nullptr, territoriesToAttack.at(0)));
        } else if (card->getTypeAsString() == "Airlift") {
            //Purposely do a bad order to get rid of the card?? Is a neutral so the aggressive player would not use it
            this->player->getOrdersList()->addOrder(std::make_unique<AirliftOrder>(this->player, territoriesToAttack.at(0), territoriesToAttack.at(1), nullptr));
        } else if (card->getTypeAsString() == "Diplomacy") {
            //Purposely do a bad order to remove it since Aggressive would never neogtiate
            this->player->getOrdersList()->addOrder(std::make_unique<NegotiateOrder>(this->player, this->player));
        }
    }
}

Territory* AggressivePlayer::getSourceTarget(const std::vector<Territory *>& playerTerritories) {
    int maxUnownedNeighbors = -1;
    int indexOfTerritory = 0;//If all else fails return the first player owned territory

    for (size_t i = 0; i < playerTerritories.size(); ++i) {
        int unownedCount = 0;
        for (const Territory* territory : *playerTerritories[i]->getNeighbors()) {
            if (territory->getOwner()->getName() != this->player->getName()) {
                ++unownedCount;
            }
        }

        if (unownedCount > maxUnownedNeighbors) {
            maxUnownedNeighbors = unownedCount;
            indexOfTerritory = static_cast<int>(i);
        }
    }

    return playerTerritories[indexOfTerritory];
}

Territory* AggressivePlayer::getAttackTarget(Territory* sourceTerritory, const std::vector<Territory*>& enemyTerritories) {
    int atkTargetIndex = 0;//Default is to try to attack first enemyTerritory available, may fail if it is not adjacent
    const std::vector<Territory*> neighbors = *sourceTerritory->getNeighbors();

    //Find target to attack based on neighbors and ensure it is inside the enemyTerritories
    for(size_t i = 0; i < neighbors.size(); ++i) {
        std::string atkTargetName = neighbors[i]->getName();
        const auto target = std::ranges::find_if(enemyTerritories, [&atkTargetName](Territory* territory) {
            return territory->getName() == atkTargetName;
        });
        bool targetIsAvailable = target != enemyTerritories.end();

        if (neighbors[i]->getOwner()->getName() == this->player->getName() && targetIsAvailable) {
            atkTargetIndex = static_cast<int>(i);
            break;
        }
    }

    return enemyTerritories[atkTargetIndex];
}

std::vector<Territory*> AggressivePlayer::toAttack() {
    std::set<Territory*> uniqueAtkTargets;
    for(const Territory* territory: this->player->getAllTerritories()) {
        for(const std::vector<Territory*> targets = *territory->getNeighbors(); Territory* target: targets) {
            if(target->getOwner()->getName() != this->player->getName()) {
                uniqueAtkTargets.insert(target);
            }
        }
    }

    std::vector<Territory*> territoriesToAttack;
    territoriesToAttack.assign(uniqueAtkTargets.begin(), uniqueAtkTargets.end());
    return territoriesToAttack;
}

std::vector<Territory*> AggressivePlayer::toDefend() {
    return this->player->getAllTerritories();
}