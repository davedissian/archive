#pragma once

class Timer
{
public:
    Timer();

    void pause();
    void resume();
    void reset();

    bool isPaused() const;
    float getSeconds() const;

private:
    bool paused_;
    float pauseTime_;
    float offset_;
    sf::Clock clock_;
};
