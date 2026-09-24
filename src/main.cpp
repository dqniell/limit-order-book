#include "Parser.h"
#include "OrderBook.h"

int main() {
    Parser parser("../data/xnas-itch-20240116.mbo.csv");
    OrderBook book;

    while (auto msg = parser.next()) {
        if (msg->action == Action::Add)         book.addOrder(*msg);
        else if (msg->action == Action::Cancel) book.cancelOrder(*msg);
        else if (msg->action == Action::Fill)   book.fillOrder(*msg);
        // Action::Trade — ignore for now
    }

    book.printBook();
    return 0;
}
