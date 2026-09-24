#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include <map>
#include <deque>

enum class Action { 
    Add,
    Cancel,
    Fill,
    Trade,
    Modify
}; 

enum class Side { 
    Bid,
    Ask,
    None,
}; 

struct Message { 
    uint64_t ts_event; 
    Action action; 
    Side side; 
    uint64_t price; 
    uint32_t size; 
    uint64_t order_id; 

}; 