#pragma once
#include <string>
#include <memory>
#include "CoinbaseTickerMessage.h"
#include "ExponentialMovingAverage.h"
#include "TickerMessageLogger.h"

class TickerMessageReceiver
{
private:
    std::unique_ptr<CoinbaseTickerMessage> mLastMessage;
    std::unique_ptr<ExponentialMovingAverage> mPriceEMA;
    std::unique_ptr<ExponentialMovingAverage> mMidPriceEMA;
    std::unique_ptr<TickerMessageLogger> mLogger;
    
private:
    TickerMessageReceiver& updateEMAs();
    TickerMessageReceiver& logMessageWithEMAs();

public:
    TickerMessageReceiver& onMessage(const std::string& aMessage); 

    static std::unique_ptr<CoinbaseTickerMessage> parseMessage(const std::string& aMessage);
};