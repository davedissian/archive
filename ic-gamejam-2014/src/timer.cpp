#include "main.h"

Timer::Timer() : paused_(false), pauseTime_(0.0f), offset_(0.0f)
{
}

void Timer::pause()
{
    paused_ = true;
    pauseTime_ = clock_.getElapsedTime().asSeconds();
}

void Timer::resume()
{
    offset_ += clock_.getElapsedTime().asSeconds() - pauseTime_;
    paused_ = false;
}

void Timer::reset()
{
    offset_ = 0.0f;
    pauseTime_ = 0.0f;
    clock_.restart();
}

bool Timer::isPaused() const
{
    return paused_;
}

float Timer::getSeconds() const
{
    if (paused_)
    {
        return pauseTime_;
    }
    else
    {
        return clock_.getElapsedTime().asSeconds() - offset_;
    }
}
