#include "Parser.h"
#include <sstream>

Parser::Parser(const std::string& filename) {
    file.open(filename);
    std::string header;
    std::getline(file, header);  // skip header row
}

std::optional<Message> Parser::next() {
    std::string line;
    if (!std::getline(file, line)) {
        return std::nullopt;
    }

    std::stringstream ss(line);
    std::string field;

    std::getline(ss, field, ',');  // ts_recv — skip
    std::getline(ss, field, ',');  // ts_event — keep
    // store field as ts_event
    uint64_t ts_event = 0;  // simplified for now

    std::getline(ss, field, ',');  // rtype — skip
    std::getline(ss, field, ',');  // publisher_id — skip
    std::getline(ss, field, ',');  // instrument_id — skip

    std::getline(ss, field, ',');  // action — keep
    // store field as action

    char action_char = field[0];
    Action action;
    if (action_char == 'A') action = Action::Add;
    else if (action_char == 'C') action = Action::Cancel;
    else if (action_char == 'F') action = Action::Fill;
    else if (action_char == 'T') action = Action::Trade;
    else action = Action::Modify;

    std::getline(ss, field, ',');  // side — keep
    Side side = (field[0] == 'B') ? Side::Bid : Side::Ask;

    std::getline(ss, field, ',');  // price — keep
    uint64_t price = (uint64_t)(std::stod(field) * 1'000'000'000);

    std::getline(ss, field, ',');  // size — keep
    uint32_t size = std::stoul(field);

    std::getline(ss, field, ',');  // channel_id — skip

    std::getline(ss, field, ',');  // order_id — keep
    uint64_t order_id = std::stoull(field);

    return Message{
        .ts_event = ts_event,
        .action   = action,
        .side     = side,
        .price    = price,
        .size     = size,
        .order_id = order_id
    };
}