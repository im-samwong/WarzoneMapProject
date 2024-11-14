#ifndef PLAYERSTRATEGIES_H
#define PLAYERSTRATEGIES_H
#include <vector>

class Player;

class PlayerStrategy {
protected:
    Player* player;

public:
    virtual issueOrder(std::vector<Player*>* players) = 0;
    virtual toAttack() = 0;
    virtual toDefend() = 0;
    virtual ~PlayerStrategy() = 0;

};
#endif //PLAYERSTRATEGIES_H