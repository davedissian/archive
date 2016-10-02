#ifndef TRAIN_H
#define TRAIN_H

#include "Entity.h"
#include "Waypoint.h"

struct Carrage
{
    sf::Sprite sprite;
    Waypoint::NodeIterator waypoint;
};

class Train : public Entity
{
public:
    Train(Waypoint* waypoint, int size, float speed, float health, int worth);
    ~Train();

    virtual void OnEvent(sf::Event&) {}
    virtual bool Update(float dt);
    virtual void Draw();
    void DrawHealth();

    void Damage(float amount);

    sf::Sprite& GetSprite() { return mCarrages[0].sprite; }
    sf::FloatRect GetBounds() const { return mCarrages[0].sprite.getGlobalBounds(); }
    float GetSpeed() const { return mSpeed; }
    int GetWorth() const { return mWorth; }

private:
    Waypoint* mWaypoint;

    vector<Carrage> mCarrages;

    sf::RectangleShape mHealthBar;
    float mHealthBarVisibility;

    float mInitialHealth;
    float mHealth;
    float mSpeed;
    int mWorth;

};

#endif /* TRAIN_H */
