#include "PlayerStrategies.h"
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

void AggressivePlayer::issueOrder(std::vector<Player *> *players) {
    //TODO:
}

void AggressivePlayer::toAttack() {
    //TODO:
}

void AggressivePlayer::toDefend() {
    //TODO:
}