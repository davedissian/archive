#ifndef CURSOR_H
#define CURSOR_H

class Turret;

class Cursor
{
public:
    Cursor();
   
    void ClearTurret();
    void SetTurret(int id);
    void PlaceTurret();
    void SetPosition(const Vec2i& position);
    void Draw();

private:
    sf::RectangleShape mRect;
    sf::CircleShape mRange;
    shared_ptr<Turret> mHeldTurret;
    Vec2i mPosition;

    bool mCanPlace;

};

#endif /* CURSOR_H */
