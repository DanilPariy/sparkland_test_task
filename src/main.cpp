#include <iostream>
#include <chrono>
#include <thread>

#include <ixwebsocket/IXWebSocket.h>
#include <nlohmann/json.hpp>

#include "TickerMessageReceiver.h"

int main()
{
    ix::WebSocket webSocket;
    webSocket.setUrl("wss://ws-feed.exchange.coinbase.com");
    TickerMessageReceiver receiver;

    webSocket.setOnMessageCallback([&](const ix::WebSocketMessagePtr& msg)
        {
            if (msg->type == ix::WebSocketMessageType::Message)
            {
                receiver.onMessage(msg->str);
            }
        }
    );

    nlohmann::json subscribeMsg = {
        {"type", "subscribe"},
        {"channels", {{
            {"name", "ticker"},
            {"product_ids", {"BTC-USD"}}
        }}}
    };

    webSocket.start();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    webSocket.send(subscribeMsg.dump());

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}