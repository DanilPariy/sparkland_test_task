#include "CoinbaseTickerMessage.h"

std::unique_ptr<CoinbaseTickerMessage> CoinbaseTickerMessage::create(const nlohmann::json &aJson)
{
    if (!aJson.is_object() || aJson.value("type", "") != CoinbaseTickerMessage::MessageType)
        return nullptr;

    auto message = std::make_unique<CoinbaseTickerMessage>();

    if (aJson.contains("type") && aJson["type"].is_string()) message->mType = aJson["type"];
    if (aJson.contains("product_id") && aJson["product_id"].is_string()) message->mProductId = aJson["product_id"];
    if (aJson.contains("price") && aJson["price"].is_string()) message->mPrice = aJson["price"];
    if (aJson.contains("open_24h") && aJson["open_24h"].is_string()) message->mOpen24h = aJson["open_24h"];
    if (aJson.contains("volume_24h") && aJson["volume_24h"].is_string()) message->mVolume24h = aJson["volume_24h"];
    if (aJson.contains("low_24h") && aJson["low_24h"].is_string()) message->mLow24h = aJson["low_24h"];
    if (aJson.contains("high_24h") && aJson["high_24h"].is_string()) message->mHigh24h = aJson["high_24h"];
    if (aJson.contains("volume_30d") && aJson["volume_30d"].is_string()) message->mVolume30d = aJson["volume_30d"];
    if (aJson.contains("best_bid") && aJson["best_bid"].is_string()) message->mBestBid = aJson["best_bid"];
    if (aJson.contains("best_bid_size") && aJson["best_bid_size"].is_string()) message->mBestBidSize = aJson["best_bid_size"];
    if (aJson.contains("best_ask") && aJson["best_ask"].is_string()) message->mBestAsk = aJson["best_ask"];
    if (aJson.contains("best_ask_size") && aJson["best_ask_size"].is_string()) message->mBestAskSize = aJson["best_ask_size"];
    if (aJson.contains("side") && aJson["side"].is_string()) message->mSide = aJson["side"];
    if (aJson.contains("time") && aJson["time"].is_string()) message->mTime = aJson["time"];
    if (aJson.contains("last_size") && aJson["last_size"].is_string()) message->mLastSize = aJson["last_size"];

    if (aJson.contains("sequence") && aJson["sequence"].is_number_unsigned()) message->mSequence = aJson["sequence"];
    if (aJson.contains("trade_id") && aJson["trade_id"].is_number_unsigned()) message->mTradeId = aJson["trade_id"];

    return message;
}

std::string CoinbaseTickerMessage::getType() const
{
    return CoinbaseTickerMessage::MessageType;
}

int64_t CoinbaseTickerMessage::getSequence() const
{
    return mSequence;
}

const std::string& CoinbaseTickerMessage::getProductId() const
{
    return mProductId;
}

const std::string& CoinbaseTickerMessage::getPrice() const
{
    return mPrice;
}

const std::string& CoinbaseTickerMessage::getOpen24h() const
{
    return mOpen24h;
}

const std::string& CoinbaseTickerMessage::getVolume24h() const
{
    return mVolume24h;
}

const std::string& CoinbaseTickerMessage::getLow24h() const
{
    return mLow24h;
}

const std::string& CoinbaseTickerMessage::getHigh24h() const
{
    return mHigh24h;
}

const std::string& CoinbaseTickerMessage::getVolume30d() const
{
    return mVolume30d;
}

const std::string& CoinbaseTickerMessage::getBestBid() const
{
    return mBestBid;
}

const std::string& CoinbaseTickerMessage::getBestBidSize() const
{
    return mBestBidSize;
}

const std::string& CoinbaseTickerMessage::getBestAsk() const
{
    return mBestAsk;
}

const std::string& CoinbaseTickerMessage::getBestAskSize() const
{
    return mBestAskSize;
}

const std::string& CoinbaseTickerMessage::getSide() const
{
    return mSide;
}

const std::string& CoinbaseTickerMessage::getTime() const
{
    return mTime;
}

int64_t CoinbaseTickerMessage::getTradeId() const
{
    return mTradeId;
}

const std::string& CoinbaseTickerMessage::getLastSize() const
{
    return mLastSize;
}

std::expected<double, std::string> CoinbaseTickerMessage::getParsedPrice() const
{
    double value;
    auto [ptr, ec] = std::from_chars(mPrice.data(), mPrice.data() + mPrice.size(), value);
    if (ec != std::errc())
    {
        return std::unexpected("Invalid price format: " + mPrice);
    }
    return value;
}

std::expected<double, std::string> CoinbaseTickerMessage::getParsedBestBid() const
{
    double value;
    auto [ptr, ec] = std::from_chars(mBestBid.data(), mBestBid.data() + mBestBid.size(), value);
    if (ec != std::errc())
    {
        return std::unexpected("Invalid bestBid format: " + mBestBid);
    }
    return value;
}

std::expected<double, std::string> CoinbaseTickerMessage::getParsedBestAsk() const
{
    double value;
    auto [ptr, ec] = std::from_chars(mBestAsk.data(), mBestAsk.data() + mBestAsk.size(), value);
    if (ec != std::errc())
    {
        return std::unexpected("Invalid bestAsk format: " + mBestAsk);
    }
    return value;
}

std::expected<std::chrono::system_clock::time_point, std::string> CoinbaseTickerMessage::getParsedTime() const
{
    std::istringstream ss(mTime);
    std::tm t = {};
    char dot = '\0';
    int microseconds = 0;

    ss >> std::get_time(&t, "%Y-%m-%dT%H:%M:%S");

    if (ss.fail())
    {
        return std::unexpected("Failed to parse date/time prefix from: " + mTime);
    }

    if (ss.peek() == '.')
    {
        ss >> dot >> microseconds;
        if (ss.fail())
        {
            return std::unexpected("Failed to parse microseconds from: " + mTime);
        }
    }

    time_t raw_time = timegm(&t);
    if (raw_time == -1)
    {
        return std::unexpected("timegm() failed for parsed tm struct");
    }

    auto tp = std::chrono::system_clock::from_time_t(raw_time);
    tp += std::chrono::microseconds(microseconds);

    return tp;
}