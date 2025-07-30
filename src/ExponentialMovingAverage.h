#pragma once
#include <chrono>
#include <cmath>

class ExponentialMovingAverage
{
private:
    double mValue;
    std::chrono::system_clock::time_point mTime;

public:
    ExponentialMovingAverage(double aInitialValue, std::chrono::system_clock::time_point aTime)
        : mValue(aInitialValue)
        , mTime(aTime)
    {}

    void update(double aNewValue, std::chrono::system_clock::time_point aNewTime, double aTauSeconds = 5.0)
    {
        using namespace std::chrono;

        auto dt = duration_cast<duration<double>>(aNewTime - mTime).count();
        if (dt <= 0.0)
            return;

        double alpha = 1.0 - std::exp(-dt / aTauSeconds);

        mValue = alpha * aNewValue + (1.0 - alpha) * mValue;
        mTime = aNewTime;
    }

    double getValue() const
    {
        return mValue;
    }
};