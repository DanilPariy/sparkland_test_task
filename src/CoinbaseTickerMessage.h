#pragma once
#include "CoinbaseMessage.h"
#include <memory>
#include <chrono>
#include <expected>
#include <nlohmann/json.hpp>

class CoinbaseTickerMessage
    : public CoinbaseMessage
{
private:
    std::string mType;
    int64_t mSequence;
    std::string mProductId;
    std::string mPrice;
    std::string mOpen24h;
    std::string mVolume24h;
    std::string mLow24h;
    std::string mHigh24h;
    std::string mVolume30d;
    std::string mBestBid;
    std::string mBestBidSize;
    std::string mBestAsk;
    std::string mBestAskSize;
    std::string mSide;
    std::string mTime;
    int64_t mTradeId;
    std::string mLastSize;

public:
    static constexpr const char* MessageType = "ticker";

    static std::unique_ptr<CoinbaseTickerMessage> create(const nlohmann::json& aJson);

    virtual std::string getType() const override;
    int64_t getSequence() const;
    const std::string& getProductId() const;
    const std::string& getPrice() const;
    const std::string& getOpen24h() const;
    const std::string& getVolume24h() const;
    const std::string& getLow24h() const;
    const std::string& getHigh24h() const;
    const std::string& getVolume30d() const;
    const std::string& getBestBid() const;
    const std::string& getBestBidSize() const;
    const std::string& getBestAsk() const;
    const std::string& getBestAskSize() const;
    const std::string& getSide() const;
    const std::string& getTime() const;
    int64_t getTradeId() const;
    const std::string& getLastSize() const;

    std::expected<double, std::string> getParsedPrice() const;
    std::expected<double, std::string> getParsedBestBid() const;
    std::expected<double, std::string> getParsedBestAsk() const;
    std::expected<std::chrono::system_clock::time_point, std::string> getParsedTime() const;
};