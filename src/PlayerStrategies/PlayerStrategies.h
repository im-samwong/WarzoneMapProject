#ifndef PLAYERSTRATEGIES_H
#define PLAYERSTRATEGIES_H
#include <ostream>
#include <vector>

class Territory;
class Player;

class PlayerStrategy {
protected:
    Player* player;
    std::string* description{};

public:
    //issueOrder with other players parameter because certain commands need Player infos
    virtual void issueOrder(std::vector<Player*>* players) = 0;
    virtual void toAttack(std::vector<Territory*> territoriesToAttack) = 0;
    virtual void toDefend(std::vector<Territory*> playerTerritories) = 0;
    virtual ~PlayerStrategy() = 0;

    //Getter for obtaining the player using this strategy
    Player* getPlayer() const;

    std::string getDescription() const;

    //Main constructor
    PlayerStrategy(Player* player, const std::string& description);

    //Copy constructor
    PlayerStrategy(const PlayerStrategy& other);

    //Assignment operator
    PlayerStrategy& operator=(const PlayerStrategy& other);

    //Stream insertion operator
    friend std::ostream& operator<<(std::ostream& os, const PlayerStrategy& strategy);
};

class AggressivePlayer: public PlayerStrategy {
public:
    explicit AggressivePlayer(Player* player);

    void issueOrder(std::vector<Player *> *players) override;

    void toAttack(std::vector<Territory*> territoriesToAttack) override;

    void toDefend(std::vector<Territory*> playerTerritories) override;

    ~AggressivePlayer() override;
};
#endif //PLAYERSTRATEGIES_H