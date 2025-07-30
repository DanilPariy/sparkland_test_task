#pragma once
#include "MessageLogger.h"
#include "CoinbaseTickerMessage.h"
#include "ExponentialMovingAverage.h"

using MessageType = std::tuple<CoinbaseTickerMessage, ExponentialMovingAverage, ExponentialMovingAverage>;

class TickerMessageLogger
    : public MessageLogger<TickerMessageLogger, MessageType>
{
public:
    using MessageType = std::tuple<CoinbaseTickerMessage, ExponentialMovingAverage, ExponentialMovingAverage>;

    TickerMessageLogger(std::ofstream&& file)
        : MessageLogger(std::move(file))
    {}

    std::string headerLine() const
    {
        return "type,sequence,product_id,price,open_24h,volume_24h,low_24h,high_24h,volume_30d,best_bid,best_bid_size,best_ask,best_ask_size,side,time,trade_id,last_size,price_ema,mid_price_ema";
    }

    std::string formatLine(const MessageType& data) const
    {
        const auto& [msg, priceEma, midPriceEma] = data;

        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2);
        oss << msg.getType()
            << "," << msg.getSequence()
            << "," << msg.getProductId()
            << "," << msg.getPrice()
            << "," << msg.getOpen24h()
            << "," << msg.getVolume24h()
            << "," << msg.getLow24h()
            << "," << msg.getHigh24h()
            << "," << msg.getVolume30d()
            << "," << msg.getBestBid()
            << "," << msg.getBestBidSize()
            << "," << msg.getBestAsk()
            << "," << msg.getBestAskSize()
            << "," << msg.getSide()
            << "," << msg.getTime()
            << "," << msg.getTradeId()
            << "," << msg.getLastSize()
            << "," << priceEma.getValue()
            << "," << midPriceEma.getValue();

        return oss.str();
    }
};
