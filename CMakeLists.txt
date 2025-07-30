
cmake_minimum_required(VERSION 3.10)
project(CoinbaseTicker)

set(CMAKE_CXX_STANDARD 23)

add_compile_options(-std=c++23)

include_directories($ENV{HOME}/vcpkg/installed/x64-linux/include)

add_executable(ticker
    src/main.cpp
    src/TickerMessageReceiver.cpp
    src/CoinbaseTickerMessage.cpp
)

add_executable(test
    src/test.cpp
    src/TickerMessageReceiver.cpp
    src/CoinbaseTickerMessage.cpp
)

find_package(Threads)

target_link_libraries(ticker
    Threads::Threads
    $ENV{HOME}/vcpkg/installed/x64-linux/lib/libixwebsocket.a
    $ENV{HOME}/vcpkg/installed/x64-linux/lib/libz.a
    -lssl
    -lcrypto
)

target_link_libraries(test
    Threads::Threads
)
