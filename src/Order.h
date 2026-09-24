#pragma once
#include <cstdint>
#include "Message.h"


struct Order { 
    uint64_t order_id; 
    Side side; 
    uint64_t price; 
    uint32_t original_quantity; 
    uint32_t remaining_quantity; 
}; 


