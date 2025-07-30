#include <cassert>
#include <iostream>
#include "TickerMessageReceiver.h"

void testParsing()
{
    std::string sample = R"({
        "type": "ticker",
        "sequence": 37475248783,
        "product_id": "ETH-USD",
        "price": "1285.22",
        "open_24h": "1310.79",
        "volume_24h": "245532.79269678",
        "low_24h": "1280.52",
        "high_24h": "1313.8",
        "volume_30d": "9788783.60117027",
        "best_bid": "1285.04",
        "best_bid_size": "0.46688654",
        "best_ask": "1285.27",
        "best_ask_size": "1.56637040",
        "side": "buy",
        "time": "2022-10-19T23:28:22.061769Z",
        "trade_id": 370843401,
        "last_size": "11.4396987"
    })";

    auto message = TickerMessageReceiver::parseMessage(sample);
    assert(message != nullptr);
    assert(message->getSequence() == 37475248783);
    assert(message->getProductId() == "ETH-USD");
    assert(message->getPrice() == "1285.22");
    assert(message->getOpen24h() == "1310.79");
    assert(message->getTime() == "2022-10-19T23:28:22.061769Z");
    assert(message->getTradeId() == 370843401);
    assert(message->getParsedPrice() == 1285.22);
    
    sample = R"({
        "type": "notticker"
    })";
    message = TickerMessageReceiver::parseMessage(sample);
    assert(message == nullptr);

    sample = R"({
        "type": "ticker",
        "time": "invalid time"
    })";
    message = TickerMessageReceiver::parseMessage(sample);
    assert(!message->getParsedTime());
    
    sample = R"({
        bad json
    })";
    message = TickerMessageReceiver::parseMessage(sample);
    assert(message == nullptr);

    std::cout << "JSON parsing test passed!" << std::endl;
}

void testEMA()
{
    using namespace std::chrono;

    const double tau = 5.0;
    const double epsilon = 1e-6;
    auto now = system_clock::now();

    {
        ExponentialMovingAverage ema(10.0, now);
        auto t1 = now + seconds(5);
        ema.update(20.0, t1, tau);

        double alpha = 1.0 - std::exp(-5.0 / tau);
        double expected = alpha * 20.0 + (1.0 - alpha) * 10.0;
        assert(std::fabs(ema.getValue() - expected) < epsilon);
    }

    {
        ExponentialMovingAverage ema(10.0, now);
        auto t1 = now + seconds(1);
        ema.update(20.0, t1, tau);

        double alpha = 1.0 - std::exp(-1.0 / tau);
        double expected = alpha * 20.0 + (1.0 - alpha) * 10.0;
        assert(std::fabs(ema.getValue() - expected) < epsilon);
    }

    {
        ExponentialMovingAverage ema(10.0, now);
        auto t1 = now + seconds(10);
        ema.update(20.0, t1, tau);

        double alpha = 1.0 - std::exp(-10.0 / tau);
        double expected = alpha * 20.0 + (1.0 - alpha) * 10.0;
        assert(std::fabs(ema.getValue() - expected) < epsilon);
    }

    {
        ExponentialMovingAverage ema(100.0, now);

        auto t1 = now + seconds(2);
        ema.update(80.0, t1, tau);
        double alpha1 = 1.0 - std::exp(-2.0 / tau);
        double v1 = alpha1 * 80.0 + (1 - alpha1) * 100.0;

        auto t2 = t1 + seconds(7);
        ema.update(60.0, t2, tau);
        double alpha2 = 1.0 - std::exp(-7.0 / tau);
        double v2 = alpha2 * 60.0 + (1 - alpha2) * v1;

        assert(std::fabs(ema.getValue() - v2) < epsilon);
    }

    std::cout << "All EMA calculation tests passed!" << std::endl;
}

int main()
{
    testParsing();
    testEMA();
    return 0;
}