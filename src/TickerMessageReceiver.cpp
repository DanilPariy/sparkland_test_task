#include "TickerMessageReceiver.h"
#include <nlohmann/json.hpp>
#include <iostream>

TickerMessageReceiver &TickerMessageReceiver::updateEMAs()
{
    if (mLastMessage)
    {
        auto price = mLastMessage->getParsedPrice();
        auto bestBid = mLastMessage->getParsedBestBid();
        auto bestAsk = mLastMessage->getParsedBestAsk();
        auto time = mLastMessage->getParsedTime();

        if (time)
        {
            if (price)
            {
                if (!mPriceEMA)
                {
                    mPriceEMA = std::make_unique<ExponentialMovingAverage>(*price, *time);
                }
                else
                {
                    mPriceEMA->update(*price, *time);
                }
            }
            if (bestBid && bestAsk)
            {
                if (!mMidPriceEMA)
                {
                    mMidPriceEMA = std::make_unique<ExponentialMovingAverage>((*bestBid + *bestAsk) / 2.0, *time);
                }
                else
                {
                    mMidPriceEMA->update((*bestBid + *bestAsk) / 2.0, *time);
                }
            }
        }
    }
    return *this;
}

TickerMessageReceiver &TickerMessageReceiver::logMessageWithEMAs()
{
    if (mLastMessage && mPriceEMA && mMidPriceEMA)
    {
        if (!mLogger)
        {
            mLogger = TickerMessageLogger::create("coinbase_log.csv");
        }
        if (mLogger)
        {
            mLogger->log(std::make_tuple(*mLastMessage, *mPriceEMA, *mMidPriceEMA));
        }
    }
    return *this;
}

TickerMessageReceiver &TickerMessageReceiver::onMessage(const std::string &aMessage)
{
    mLastMessage = parseMessage(aMessage);
    updateEMAs().logMessageWithEMAs();
    return *this;
}

std::unique_ptr<CoinbaseTickerMessage> TickerMessageReceiver::parseMessage(const std::string &aMessage)
{
    nlohmann::json json;
    try
    {
        json = nlohmann::json::parse(aMessage);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return nullptr;
    }
    
    if (!json.is_object())
        return nullptr;

    auto typeIt = json.find("type");
    if (typeIt == json.end() || !typeIt->is_string())
        return nullptr;

    if (*typeIt != CoinbaseTickerMessage::MessageType)
        return nullptr;

    return CoinbaseTickerMessage::create(json);
}
