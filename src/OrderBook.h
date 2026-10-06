#pragma once
#include <map>
#include <unordered_map>
#include <functional>
#include "Message.h"
#include "PriceLevel.h"

struct OrderLocation {
    Side     side;
    uint64_t price;
};

class OrderBook {
public:
    void addOrder(const Message& msg);
    void cancelOrder(const Message& msg);
    void fillOrder(const Message& msg);
    void printBook(int depth = 5) const;
    double spread() const;
private:
    std::map<uint64_t, PriceLevel, std::greater<uint64_t>> bids;
    std::map<uint64_t, PriceLevel> asks;
    std::unordered_map<uint64_t, OrderLocation> order_index;
};
