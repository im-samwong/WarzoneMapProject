#include "PlayerStrategies.h"

#include <algorithm>

#include "../Player/Player.h"
#include <limits>
#include <set>


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
    os << "PlayerStrategy: " << strategy.getDescription() << std::endl;
    os << "Player using this strategy: " << strategy.getPlayer()->getName() << std::endl;

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
    std::cout << "AggressivePlayer:: " << player->getName() << " has started issuing orders" << std::endl;
    const int reinforcements = this->player->getReinforcements();
    const int unitsToUse = reinforcements;

    Territory* territoryToReinforce = this->getSourceTarget(this->player->toDefend());
    //Adds a singular order to reinforce one country with which it will push through
    this->player->getOrdersList()->addOrder(std::make_unique<DeployOrder>(this->player,territoryToReinforce,new int(unitsToUse)));

    //Once the deploy order is out we now proceed with the rest
    std::vector<Territory*> territoriesToAttack = this->toAttack();

    Territory* sourceTerritory = getSourceTarget(this->toDefend());
    Territory* targetTerritory = getAttackTarget(sourceTerritory, territoriesToAttack);

    bool issuedAttack = false;
    //Use cards
    std::vector<Card*> playerCards = this->player->getHand().getHandCards();
    //Remove nulls from hand
    playerCards.erase(std::ranges::remove(playerCards, nullptr).begin(), playerCards.end());
    for (Card* card: playerCards) {
        if (card->getTypeAsString() == "Reinforcement") {
            //Advance on a neighbor of the source territory with full force i.e use all available units or use just enough if above target army count
            int atkUnitsToUse = sourceTerritory->getArmies();
            if (sourceTerritory->getArmies() >= targetTerritory->getArmies()) {
                atkUnitsToUse = targetTerritory->getArmies();
            }
            this->player->getOrdersList()->addOrder(
                std::make_unique<AdvanceOrder>(this->player, sourceTerritory, targetTerritory, new int(atkUnitsToUse)));
            issuedAttack = true;
        } else if (card->getTypeAsString() == "Bomb") {
            //Bomb one of the neighbors of the sourceTerritory until all neighbors are conquered
            this->player->getOrdersList()->addOrder(std::make_unique<BombOrder>(this->player, targetTerritory));
        } else if (card->getTypeAsString() == "Blockade") {
            //Purposely do a bad order to get rid of the card an aggressive player would never give up a territory
            this->player->getOrdersList()->
                    addOrder(std::make_unique<BlockadeOrder>(nullptr, territoriesToAttack.at(0)));
        } else if (card->getTypeAsString() == "Airlift") {
            //Purposely do a bad order to get rid of the card?? Is a neutral so the aggressive player would not use it
            this->player->getOrdersList()->addOrder(
                std::make_unique<AirliftOrder>(this->player, territoriesToAttack.at(0), territoriesToAttack.at(1),
                                               nullptr));
        } else if (card->getTypeAsString() == "Diplomacy") {
            //Purposely do a bad order to remove it since Aggressive would never neogtiate
            this->player->getOrdersList()->addOrder(std::make_unique<NegotiateOrder>(this->player, this->player));
        }
    }

    //Remove them
    for(int i = 0; i < playerCards.size(); i++) {
        delete playerCards[i];
    }

    playerCards.clear();
    player->getHand().setHandCards(playerCards);//Update the player hand

    //Issue advance order if none was used via cards due to not having an aggressive one or having no cards
    if (!issuedAttack) {
        int unitsToAtk = sourceTerritory->getArmies();
        if (sourceTerritory->getArmies() >= targetTerritory->getArmies()) {
            unitsToAtk = targetTerritory->getArmies();
        }
        this->player->getOrdersList()->addOrder(std::make_unique<AdvanceOrder>(this->player, sourceTerritory, targetTerritory, new int(unitsToAtk)));
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
    std::vector<Territory*> neighbors = *sourceTerritory->getNeighbors();

    for (size_t i = 0; i < neighbors.size(); ++i) {
        if (neighbors[i]->getOwner()->getName() != this->player->getName()) {
            for (size_t j = 0; j < enemyTerritories.size(); j++) {
                if (neighbors[i]->getName() == enemyTerritories[j]->getName()) {
                    atkTargetIndex = static_cast<int>(j);
                    break;
                }
            }
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
    std::vector<Territory*> defendableTerritories;

    for (auto territory: this->player->getAllTerritories()) {
        if (territory->getOwner()->getName() == this->player->getName()) {
            defendableTerritories.push_back(territory);
        }
    }

    return defendableTerritories;
}

HumanPlayer::HumanPlayer(Player *player): PlayerStrategy(player, "HumanPlayer") {}

HumanPlayer::~HumanPlayer() {
    //DO NOT DELETE PLAYER AS WE DON'T OWN IT
    delete description;
}

/**
 * The Human Player's issueOrders logic requires user interactions to make decisions.
 * @param players List of players in the game
 */
void HumanPlayer::issueOrder(std::vector<Player*>* players) {
    std::cout << "Here are the territories you should defend:" << std::endl;
    std::vector<Territory*> playerTerritories = player->toDefend();
    std::vector<Territory*> territoriesToAttack = player->toAttack();
    std::vector<Territory*> allTerritories;

    allTerritories.reserve(playerTerritories.size() + territoriesToAttack.size());
    allTerritories.insert(allTerritories.end(),playerTerritories.begin(),playerTerritories.end());
    allTerritories.insert(allTerritories.end(), territoriesToAttack.begin(),territoriesToAttack.end());

    for(Territory* territory: playerTerritories) {
        std::cout << territory->getName() << std::endl;
    }

    std::cout << "\nFor now, use the deploy order until you have no more reinforcements to deploy." << std::endl;

    int reinforcements = player->getReinforcements();
    while (reinforcements != 0) {
        std::cout << "\nRemaining units: " << reinforcements << std::endl;

        int unitsToUse = 0;

        std::cout << "How many units would you like to deploy?" << std::endl;
        std::cin >> unitsToUse;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "To which territory do you want to deploy these " << unitsToUse << "? Type the name of the territory as you see it." << std::endl;
        std::string territoryName;
        std::getline(std::cin,territoryName);

        auto target = std::ranges::find_if(playerTerritories, [&territoryName](Territory* territory) {
                return territory->getName() == territoryName;
        });

        reinforcements -= unitsToUse;
        if(target != playerTerritories.end()) {
            player->getOrdersList()->addOrder(std::make_unique<DeployOrder>(player,*target,new int(unitsToUse)));
        } else {
            player->getOrdersList()->addOrder(std::make_unique<DeployOrder>(player,nullptr,new int(unitsToUse)));
        }
    }

    std::cout << "\nFinished deploying units, now for advancing:" << std::endl;
    
    std::cout << "\nHere are the territories you should attack:" << std::endl;
    for(Territory* territory: territoriesToAttack) {
        std::cout << territory->getName() << std::endl;
    }

    std::string sourceTerritory;
    std::string targetTerritory;
    int numOfUnits = 0;

    std::cout << "From which territory do you wish to move units from?" << std::endl;
    std::getline(std::cin,sourceTerritory);
    auto source = std::ranges::find_if(allTerritories, [&sourceTerritory](Territory* territory) {
        return territory->getName() == sourceTerritory;
    });


    std::cout << "To what territory should these units be deployed?" << std::endl;
    std::getline(std::cin,targetTerritory);
    auto target = std::ranges::find_if(allTerritories, [&targetTerritory](Territory* territory) {
        return territory->getName() == targetTerritory;
    });

    std::cout << "Lastly, how many units do you want to move." << std::endl;
    std::cin >> numOfUnits;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (source != allTerritories.end() && target != allTerritories.end()) {
        player->getOrdersList()->addOrder(std::make_unique<AdvanceOrder>(player, *source, *target, new int(numOfUnits)));
    } else {
        std::cout << "The target or source territory is not in the list of territories to defend or attack. Try again" << std::endl;
    }

    if(player->getHand().getHandCards().empty()) {
        std::cout << "You have no cards in your hand. Your turn for issuing orders is over" << std::endl;
        return;
    }

    std::cout << "\nYou have now issued deployment and advancement orders. You can now issue orders other than deploy now." << std::endl;

    //Once all reinforcements are deployed and an advance order is made, show the toAttack stuff

    std::cout << "\nHere are the territories you should attack:" << std::endl;
    for(Territory* territory: territoriesToAttack) {
        std::cout << territory->getName() << std::endl;
    }

    bool playerHasFinishedIssuingOrders = false;
    const std::vector<std::string> possibleOrders = {"AdvanceOrder","AirliftOrder","BlockadeOrder","BombOrder","NegotiateOrder"};
    const std::vector<std::string> stringEnumCards = {"Reinforcement","Airlift","Blockade","Bomb","Diplomacy"};

    std::vector<Card*> playerHandOrders = player->getHand().getHandCards();

    //At some point nulls will be added to the hand if the Deck runs out of cards
    playerHandOrders.erase(std::ranges::remove(playerHandOrders, nullptr).begin(), playerHandOrders.end());

    while (!playerHasFinishedIssuingOrders) {
        if(player->getHand().getHandCards().empty()) {
            std::cout << "You have no cards in your hand. Your turn for issuing orders is over" << std::endl;
            playerHasFinishedIssuingOrders = true;
            break;
        }
        std::cout << "\nYour available orders are:" << std::endl;
        int chosenOrderIndex = -1;
        for (const Card* card: playerHandOrders) {
            std::cout << card->getTypeAsString() << std::endl;
        }

        std::cout << "\nType the order as you see it if you wish to issue it or type endissueorders to stop issuing orders" << std::endl;

        std::string orderInput;
        std::cin >> orderInput;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (orderInput == "endissueorders") {
            playerHasFinishedIssuingOrders = true;
            std::cout << "Player has decided to stop issuing orders. Ending issue order turn for this player" << std::endl;
            break;
        }

        if (orderInput == stringEnumCards[0]) {
            std::string sourceTerritory;
            std::string targetTerritory;
            int numOfUnits = 0;

            std::cout << "From which territory do you wish to move units from?" << std::endl;
            std::getline(std::cin,sourceTerritory);
            auto source = std::ranges::find_if(allTerritories, [&sourceTerritory](Territory* territory) {
                return territory->getName() == sourceTerritory;
            });


            std::cout << "To what territory should these units be deployed?" << std::endl;
            std::getline(std::cin,targetTerritory);
            auto target = std::ranges::find_if(allTerritories, [&targetTerritory](Territory* territory) {
                return territory->getName() == targetTerritory;
            });

            std::cout << "Lastly, how many units do you want to move." << std::endl;
            std::cin >> numOfUnits;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (source != allTerritories.end() && target != allTerritories.end()) {
                player->getOrdersList()->addOrder(std::make_unique<AdvanceOrder>(player, *source, *target, new int(numOfUnits)));
                chosenOrderIndex = 0;
            } else {
                std::cout << "The target or source territory is not in the list of territories to defend or attack. Try again" << std::endl;
            }
        } else if (orderInput == stringEnumCards[1]) {
            std::string sourceTerritory;
            std::string targetTerritory;
            int numOfUnits = 0;

            std::cout << "From which territory do you wish to airlift units from?" << std::endl;
            std::getline(std::cin,sourceTerritory);
            auto source = std::ranges::find_if(allTerritories, [&sourceTerritory](Territory* territory) {
                return territory->getName() == sourceTerritory;
            });

            std::cout << "To what territory should these units be deployed?" << std::endl;
            std::getline(std::cin,targetTerritory);
            auto target = std::ranges::find_if(allTerritories, [&targetTerritory](Territory* territory) {
                return territory->getName() == targetTerritory;
            });

            std::cout << "Lastly, how many units do you want to airlift. Source territory has "<< (*source)->getArmies() << " units you can move" << std::endl;
            std::cin >> numOfUnits;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (source != allTerritories.end() && target != allTerritories.end()) {
                player->getOrdersList()->addOrder(std::make_unique<AirliftOrder>(player,  *source, *target, new int(numOfUnits)));
                chosenOrderIndex = 1;
            } else {
                std::cout << "The target or source territory is not in the list of territories to defend or attack. Try again" << std::endl;
            }
        } else if (orderInput == stringEnumCards[2]) {
            std::string targetTerritory;
            std::cout << "What territory do you want to blockade?" << std::endl;
            std::getline(std::cin,targetTerritory);

            auto target = std::ranges::find_if(allTerritories, [&targetTerritory](Territory* territory) {
                return territory->getName() == targetTerritory;
            });

            if (target != playerTerritories.end()) {
                player->getOrdersList()->addOrder(std::make_unique<BlockadeOrder>(player, *target));
                chosenOrderIndex = 2;
            } else {
                std::cout << "The target territory was not found. Try again" << std::endl;
            }
        } else if (orderInput == stringEnumCards[3]) {
            std::string targetTerritory;
            std::cout << "What territory do you want to bomb?" << std::endl;
            std::getline(std::cin,targetTerritory);

            auto target = std::ranges::find_if(allTerritories, [&targetTerritory](Territory* territory) {
                return territory->getName() == targetTerritory;
            });

            if (target != playerTerritories.end()) {
                player->getOrdersList()->addOrder(std::make_unique<BombOrder>(player, *target));
                chosenOrderIndex = 3;
            } else {
                std::cout << "The target territory was not found. Try again" << std::endl;
            }
        } else if (orderInput == stringEnumCards[4]) {
            std::string targetPlayer;
            std::cout << "With who do you want to negotiate with? Type their name as it appears:" << std::endl;
            for(Player* player: *players) {
                std::cout << player->getName() << std::endl;
            }

            std::getline(std::cin, targetPlayer);

            auto target = std::ranges::find_if(*players, [&targetPlayer](Player* player) {
                return player->getName() == targetPlayer;
            });

            if (target != players->end()) {
                player->getOrdersList()->addOrder(std::make_unique<NegotiateOrder>(player,*target));
                chosenOrderIndex = 4;
            } else {
                std::cout << "The target player was not found. Try again" << std::endl;
            }
        } else {
            std::cout << "Invalid command. Did nothing, please re-issue your order" << std::endl;
        }

        if (chosenOrderIndex != -1) {
            for(int i = 0; i < playerHandOrders.size(); i++) {
                if (playerHandOrders[i]->getTypeAsString() == stringEnumCards[chosenOrderIndex]) {
                    delete playerHandOrders[i];
                    playerHandOrders.erase(playerHandOrders.begin() + i);
                    player->getHand().setHandCards(playerHandOrders);//Update the player hand
                    break;
                }
            }
        }
    }

}

/**
 * Method used to implement the toAttack logic of the HumanPlayer
 */
std::vector<Territory*> HumanPlayer::toAttack() {
    std::set<Territory*> uniqueAtkTargets;
    for(const Territory* territory: player->getAllTerritories()) {
        for(const std::vector<Territory*> targets = *territory->getNeighbors(); Territory* target: targets) {
            if(target->getOwner()->getName() != player->getName()) {
                uniqueAtkTargets.insert(target);
            }
        }
    }

    std::vector<Territory*> territoriesToAttack;
    territoriesToAttack.assign(uniqueAtkTargets.begin(), uniqueAtkTargets.end());
    return territoriesToAttack;
}

std::vector<Territory*> HumanPlayer::toDefend() {
    return player->getAllTerritories();
}

NeutralPlayer::NeutralPlayer(Player *player): PlayerStrategy(player, "NeutralPlayer") {}

NeutralPlayer::~NeutralPlayer() {
    //DO NOT DELETE PLAYER AS WE DON'T OWN IT
    delete description;
}

/**
 * The Neutral Player's issueOrders logic does nothing until attacked, where it switches strategies to Aggressive.
 * @param players List of players in the game
 */
void NeutralPlayer::issueOrder(std::vector<Player*>* players) {
    for (Territory* terr : player->toDefend()) {
        if (terr->getAttacked() == true) {
            Player* p = player;
            std::cout << "Neutral player " << player->getName() << " has been attacked, setting strategy to Aggressive..." << std::endl;
            player->setPlayerStrategy(new AggressivePlayer(p));
        }
    }
}

std::vector<Territory*> NeutralPlayer::toAttack() {
    std::set<Territory*> uniqueAtkTargets;
    for(const Territory* territory: player->getAllTerritories()) {
        for(const std::vector<Territory*> targets = *territory->getNeighbors(); Territory* target: targets) {
            if(target->getOwner()->getName() != player->getName()) {
                uniqueAtkTargets.insert(target);
            }
        }
    }

    std::vector<Territory*> territoriesToAttack;
    territoriesToAttack.assign(uniqueAtkTargets.begin(), uniqueAtkTargets.end());
    return territoriesToAttack;
}

std::vector<Territory*> NeutralPlayer::toDefend() {
    return player->getAllTerritories();
}

BenevolentPlayer::BenevolentPlayer(Player* player): PlayerStrategy(player, "BenevolentPlayer") {}

BenevolentPlayer::~BenevolentPlayer() {
    delete description;
}

void BenevolentPlayer::issueOrder(std::vector<Player*>* players) {
    std::cout << "The Benevolent Player is issuing orders..." << std::endl;

    const int reinforcements = this->player->getReinforcements();
    const int unitsToUse = reinforcements;

    //reinforcing the weakest territory
    Territory* weakestTerritory = this->findWeakestTerritory();
    if (weakestTerritory != nullptr) {
        this->player->getOrdersList()->addOrder(std::make_unique<DeployOrder>(this->player, weakestTerritory, new int(unitsToUse)));
    }

    //uses cards without purposefully harming anyone
    std::vector<Card*> playerCards = this->player->getHand().getHandCards();
    for (Card* card : playerCards) {
        if (card->getTypeAsString() == "Reinforcement") { //deploy to the weakest territory
            this->player->getOrdersList()->addOrder(std::make_unique<DeployOrder>(this->player, weakestTerritory, new int(unitsToUse)));
        }
        else if (card->getTypeAsString() == "Airlift") {
            //deploy units to another of the player's own weak territories
            Territory* weakTerritory2 = this->findWeakestTerritory();
            if (weakTerritory2 != nullptr && weakTerritory2 != weakestTerritory) {
                this->player->getOrdersList()->addOrder(std::make_unique<AirliftOrder>(this->player, weakestTerritory, weakTerritory2, new int(unitsToUse)));
            }
        }
        else if (card->getTypeAsString() == "Blockade") {
            if (weakestTerritory != nullptr) {
                this->player->getOrdersList()->addOrder(std::make_unique<BlockadeOrder>(this->player, weakestTerritory));

                //looking for a neutral player
                Player* neutralPlayer = nullptr;
                for (Player* p : *players) {
                    if (p->getName() == "Neutral") {
                        neutralPlayer = p;
                        break;
                    }
                }

                if (!(neutralPlayer == nullptr)) {
                    //transfer territory to neutral player
                    weakestTerritory->setOwner(neutralPlayer);
                }
                else {
                    //no neutral player exists so we create one
                    neutralPlayer = new Player("Neutral");
                    PlayerStrategy* playerStrat = new NeutralPlayer(neutralPlayer);
                    players->push_back(neutralPlayer);
                    weakestTerritory->setOwner(neutralPlayer);

                }
            }

        }
        else if (card->getTypeAsString() == "Diplomacy") {
        // negotiate
            for (Player* player2 : *players) {
                if (player2 != this->player) {
                    this->player->getOrdersList()->addOrder(std::make_unique<NegotiateOrder>(this->player, player2));
                    break;
                }
            }
        }
        else if (card->getTypeAsString() == "Bomb") {
            //this card will always cause harm to other players so it will not be used
        }
    }

    for(int i = 0; i < playerCards.size(); i++) {
        delete playerCards[i];
    }

    playerCards.clear();
    player->getHand().setHandCards(playerCards);  //update player's hand
}

Territory* BenevolentPlayer::findWeakestTerritory() {
    //territory with the fewest armies
    Territory* weakestTerritory = nullptr;
    int leastArmies = std::numeric_limits<int>::max(); //start with large integer

    for (Territory* territory : this->player->getAllTerritories()) {
        if (territory->getArmies() < leastArmies) {
            leastArmies = territory->getArmies();
            weakestTerritory = territory;
        }
    }
    return weakestTerritory;
}

std::vector<Territory*> BenevolentPlayer::toAttack() {
    return {};  //doesn't attack
}

std::vector<Territory*> BenevolentPlayer::toDefend() {
    return this->player->getAllTerritories();
}



CheaterPlayer::CheaterPlayer(Player* player): PlayerStrategy(player, "CheaterPlayer") {}

CheaterPlayer::~CheaterPlayer() {
    delete description;
}

void CheaterPlayer::issueOrder(std::vector<Player*>* players) { //doesn't use cards
    std::cout << "Cheater Player is issuing orders..." << std::endl;

    const int reinforcements = this->player->getReinforcements();
    const int unitsToUse = reinforcements;

    //deploying units to one of the player's territory (in this case, we're using the first in the list)
    Territory* territoryToReinforce = this->player->getAllTerritories().front();
    this->player->getOrdersList()->addOrder(std::make_unique<DeployOrder>(this->player, territoryToReinforce, new int(unitsToUse)));

    //conquering adjacent territories (once per turn)
    std::vector<Territory*> territoriesToAttack = this->toAttack();
    for (Territory* target : territoriesToAttack) {
        this->player->getOrdersList()->addOrder(std::make_unique<AdvanceOrder>(this->player, territoryToReinforce, target, new int(territoryToReinforce->getArmies())));
    }

}

std::vector<Territory*> CheaterPlayer::toAttack() {
    std::set<Territory*> uniqueAtkTargets;
    for (const Territory* territory : this->player->getAllTerritories()) {
        for (Territory* neighbor : *territory->getNeighbors()) {
            if (neighbor->getOwner()->getName() != this->player->getName()) {
                uniqueAtkTargets.insert(neighbor);
            }
        }
    }

    std::vector<Territory*> territoriesToAttack;
    territoriesToAttack.assign(uniqueAtkTargets.begin(), uniqueAtkTargets.end());
    return territoriesToAttack;
}

std::vector<Territory*> CheaterPlayer::toDefend() {
    return this->player->getAllTerritories();
}
