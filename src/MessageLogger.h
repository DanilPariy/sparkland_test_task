#pragma once
#include <fstream>

template <typename Derived, typename MessageType>
class MessageLogger
{
private:
    std::ofstream mFile;
    bool mIsHeaderPrinted;

public:
    MessageLogger(std::ofstream&& aFile)
        : mFile(std::move(aFile))
        , mIsHeaderPrinted(false)
    {}

    virtual ~MessageLogger()
    {
        if (mFile.is_open())
        {
            mFile.close();
        }
    }

    void log(const MessageType& aMessage)
    {
        if (!mIsHeaderPrinted)
        {
            mFile << static_cast<Derived*>(this)->headerLine() << std::endl;
            mIsHeaderPrinted = true;
        }

        auto line = static_cast<Derived*>(this)->formatLine(aMessage);
        if (!line.empty())
        {
            mFile << line << std::endl;
        }
    }

    static std::unique_ptr<Derived> create(const std::string& aFilename)
    {
        std::ofstream file(aFilename, std::ios::out | std::ios::trunc);
        if (!file.is_open())
            return nullptr;

        return std::make_unique<Derived>(std::move(file));
    }
};