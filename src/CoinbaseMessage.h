#pragma once
#include <string>

class CoinbaseMessage
{
public:
    virtual ~CoinbaseMessage() = default;
    virtual std::string getType() const = 0;
};