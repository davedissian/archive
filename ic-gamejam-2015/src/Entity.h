#ifndef ENTITY_H
#define ENTITY_H

class Game;

class Entity
{
public:
    Entity() : mDead(false) {}
    virtual ~Entity() {}

    virtual bool Update(float dt) = 0;
    virtual void Draw() = 0;

    void Kill() { mDead = true; }
    bool IsDead() const { return mDead; }

private:
    bool mDead;

};

#endif /* ENTITY_H */
