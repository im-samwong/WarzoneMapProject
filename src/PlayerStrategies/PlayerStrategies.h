#ifndef PLAYERSTRATEGIES_H
#define PLAYERSTRATEGIES_H
#include <vector>
#include <string>

class Territory;
class Player;

class PlayerStrategy {
protected:
    Player* player;
    std::string* description{};

public:
    //issueOrder with other players parameter because certain commands need Player infos
    virtual void issueOrder(std::vector<Player*>* players) = 0;
    virtual std::vector<Territory*> toAttack() = 0;
    virtual std::vector<Territory*> toDefend() = 0;
    virtual ~PlayerStrategy() = default;

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

    std::vector<Territory*> toAttack() override;

    std::vector<Territory*> toDefend() override;

    /**
     * Given a set of player owned territories, finds the source territory the player should used based on the idea that
     * the strongest country is that which has the most neighbors as once all are conquered more territories are controlled.
     * @param playerTerritories Player owned territories i.e toDefend
     * @return Pointer to Territory that should be used as the source territory for orders
     */
    Territory* getSourceTarget(const std::vector<Territory*>& playerTerritories);

    /**
     * Given a set of enemy territories, finds the target territory the player should attack based on the source territory
     * @param sourceTerritory Source territory that the player will attack from
     * @param enemyTerritories Player owned territories i.e toAttack
     * @return Pointer to Territory that should be used as the target territory for orders
     */
    Territory* getAttackTarget(Territory* sourceTerritory, const std::vector<Territory*>& enemyTerritories);

    ~AggressivePlayer() override;
};

/** requires user interactions to make decisions, including deploy and advance orders, as well as
 playing any card
 **/
class HumanPlayer: public PlayerStrategy {
public:
    explicit HumanPlayer(Player* player);

    void issueOrder(std::vector<Player *> *players) override;

    std::vector<Territory*> toAttack() override;

    std::vector<Territory*> toDefend() override;

    ~HumanPlayer() override;
};

/** Does nothing until attacked, where it changes strategies to Aggressive
 **/
class NeutralPlayer: public PlayerStrategy {
public:
    explicit NeutralPlayer(Player* player);

    void issueOrder(std::vector<Player *> *players) override;

    std::vector<Territory*> toAttack() override;

    std::vector<Territory*> toDefend() override;

    ~NeutralPlayer() override;
};

/** Focuses on protecting its weak countries **/
class BenevolentPlayer: public PlayerStrategy {
public:
    explicit BenevolentPlayer(Player* player);

    void issueOrder(std::vector<Player *> *players) override;

    std::vector<Territory*> toAttack() override;

    std::vector<Territory*> toDefend() override;

    ~BenevolentPlayer() override;

    Territory* findWeakestTerritory(); // finds player's weakest territory

};

/** Conquers all territories that are adjacent to its own territories **/
class CheaterPlayer: public PlayerStrategy {
public:
    explicit CheaterPlayer(Player* player);

    void issueOrder(std::vector<Player *> *players) override;

    std::vector<Territory*> toAttack() override;

    std::vector<Territory*> toDefend() override;

    ~CheaterPlayer() override;
};

#endif //PLAYERSTRATEGIES_H
