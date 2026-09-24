#pragma once
#include <deque>
#include <memory>
#include <cstdint> 
#include "Order.h"

struct PriceLevel { 
    std::deque<std::shared_ptr<Order>>  orders;
    uint32_t total_quantity;
}; 