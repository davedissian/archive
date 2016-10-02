#ifndef GAME_H
#define GAME_H

class Cursor;
class Map;
class Train;
class Turret;
class Bullet;
class Explosion;

class Game
{
public:
    // States
    enum State
    {
        STATE_MENU,
        STATE_BUILD,
        STATE_GAME
    };

    Game();

    int Run();
    void Draw(sf::Drawable& drawable);

    sf::Texture& LoadTexture(const string& filename);

    void AddTurret(shared_ptr<Turret> t) { mTurrets.push_back(t); }
    void AddBullet(shared_ptr<Bullet> b) { mBullets.push_back(b); }
    void SpawnExplosion(const Vec2f& p, bool large);
   
    void AlterBalance(int amount) { mBalance += amount; }

    void TrainEscaped() { mEscaped++; }
    vector<shared_ptr<Train>> Trains() { return mTrains; }
    vector<shared_ptr<Turret>> Turrets() { return mTurrets; }
    shared_ptr<Map> GetMap() { return mMap; }

    int GetBalance() const { return mBalance; }

private:
    void Update(float dt);
    void DrawAll();

    void SetGUISlide(float slide);
    void SetGUIVisible(bool visible);

    // The Director
    void StartRound();
    void DirectorTick(float dt);
    void DispatchTrain();

    sf::RenderWindow mWindow;
    map<string, sf::Texture> mTextures;
    sf::Font mFont;

    // Game state
    int mState;
    int mDifficulty;

    // Main Menu
    sf::Text mTitle;
    sf::Text mCaption;
    sf::Text mAim;

    // Game objects
    shared_ptr<Cursor> mCursor;
    shared_ptr<Map> mMap;
    vector<shared_ptr<Train>> mTrains;
    vector<shared_ptr<Turret>> mTurrets;
    vector<shared_ptr<Bullet>> mBullets;
    vector<shared_ptr<Explosion>> mExplosions;

    bool mGUIOpen;
    float mGUISlide;
    
    int mBalance;
    int mRemaining;
    int mEscaped;

    
    /// BUILD MODE
    sf::Text mBuildMode;

    // GUI
    sf::Text mBalanceDisplay;

    sf::RectangleShape mTurretSelectorBox;
    // Possible turrets
    vector<shared_ptr<Turret>> mPossibleTurrets;
    vector<shared_ptr<Turret>>::iterator mSelectedTurret;
    sf::RectangleShape mTurretSelectorCursor;

    // Tooltip
    sf::RectangleShape mTooltipBox;
    sf::Text mTooltip;


    /// GAME MODE
    int mSpawnCounter;
    int mTrainSize;
    float mTimeSinceLastSpawn;
};

extern Game* gGame;

#endif /* GAME_H */
