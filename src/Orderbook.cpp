#include "OrderBook.h"
#include <iostream>
#include <iomanip>

void OrderBook::addOrder(const Message& msg) { 
    auto order = std::make_shared<Order>(Order{
        .order_id           = msg.order_id, 
        .side               = msg.side,
        .price              = msg.price,
        .original_quantity  = msg.size,
        .remaining_quantity = msg.size
    }); 

    if (msg.side == Side::Bid) { 
        bids[msg.price].orders.push_back(order); 
        bids[msg.price].total_quantity += msg.size; 
    } else { 
        asks[msg.price].orders.push_back(order);
        asks[msg.price].total_quantity += msg.size;
    }
    order_index[msg.order_id] = { msg.side, msg.price}; 
}

void OrderBook::cancelOrder(const Message& msg) {
    if (order_index.find(msg.order_id) == order_index.end()) return;

    auto location = order_index[msg.order_id];
    auto& level = (location.side == Side::Bid) 
                  ? bids[location.price] 
                  : asks[location.price];

    // now one single for loop works for both sides
    for (auto it = level.orders.begin(); it != level.orders.end(); ++it) {
        if ((*it)->order_id == msg.order_id) {
            level.total_quantity -= (*it)->remaining_quantity;
            level.orders.erase(it);
            break;
        }
    }

    if (level.orders.empty()) {
        if (location.side == Side::Bid) bids.erase(location.price);
        else asks.erase(location.price);
    }

    order_index.erase(msg.order_id);
}

void OrderBook::printBook(int depth) const {
    std::cout << "=== AAPL Order Book ===\n";
    std::cout << "---- ASKS ----\n";

    int count = 0;
    for (auto it = asks.begin(); it != asks.end() && count < depth; ++it, ++count) {
        double price = it->first / 1'000'000'000.0;
        std::cout << "$" << std::fixed << std::setprecision(3)
                  << price << "  x  " << it->second.total_quantity << "\n";
    }

    std::cout << "---- BIDS ----\n";
    count = 0;
    for (auto it = bids.begin(); it != bids.end() && count < depth; ++it, ++count) {
        double price = it->first / 1'000'000'000.0;
        std::cout << "$" << std::fixed << std::setprecision(3)
                  << price << "  x  " << it->second.total_quantity << "\n";
    }

    if (!bids.empty() && !asks.empty()) {
        std::cout << "Spread: $" << std::fixed << std::setprecision(3) << spread() << "\n";
    }
}

double OrderBook::spread() const {
    if (bids.empty() || asks.empty()) return 0.0;
    return (asks.begin()->first - bids.begin()->first) / 1'000'000'000.0;
}

void OrderBook::fillOrder(const Message& msg) {
    if (order_index.find(msg.order_id) == order_index.end()) return;

    auto location = order_index[msg.order_id];
    auto& level = (location.side == Side::Bid) 
                  ? bids[location.price] 
                  : asks[location.price];
                  
    for (auto it = level.orders.begin(); it != level.orders.end(); ++it) { 
        if ((*it)->order_id == msg.order_id) {        // ← found the order
            (*it)->remaining_quantity -= msg.size;     // ← reduce remaining
            level.total_quantity -= msg.size;          // ← reduce level total

            if ((*it)->remaining_quantity == 0) {      // ← fully filled?
                level.orders.erase(it);                // ← remove from deque
                if (level.orders.empty()) {            // ← level now empty?
                    if (location.side == Side::Bid) bids.erase(location.price);
                    else asks.erase(location.price);   // ← remove level
                }
                order_index.erase(msg.order_id);       // ← remove from index
            }
            break;                                     // ← stop searching
        }
    }
}