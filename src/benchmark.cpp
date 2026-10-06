#include "Parser.h"
#include "OrderBook.h"
#include <algorithm>
#include <chrono>
#include <vector>
#include <iostream>

int main() {
    // 1. Parse the whole feed into memory first (NOT timed)
    Parser parser("../data/xnas-itch-20240116.mbo.csv");
    std::vector<Message> events;
    while (auto msg = parser.next()) {
        events.push_back(*msg);
    }

    // 2. Time just the book updates — warm up first, then take the median of several runs
    //    to smooth out CPU cache / branch predictor / allocator noise.
    constexpr int kWarmup = 2;
    constexpr int kRuns = 10;
    std::vector<double> times;

    for (int i = 0; i < kWarmup + kRuns; ++i) {
        OrderBook book;  // fresh book every run — replaying into a populated book
                          // would hit cancels/fills for orders already removed
        auto start = std::chrono::steady_clock::now();
        for (const Message& msg : events) {
            if (msg.action == Action::Add)         book.addOrder(msg);
            else if (msg.action == Action::Cancel) book.cancelOrder(msg);
            else if (msg.action == Action::Fill)   book.fillOrder(msg);
            // Action::Trade  — informational; the corresponding fill already
            //                  arrives as its own Action::Fill message
            // Action::Modify — not present in this dataset (verified: only
            //                  A/C/F/T action codes occur in the source CSV)
        }
        auto end = std::chrono::steady_clock::now();

        if (i >= kWarmup) {
            times.push_back(std::chrono::duration<double>(end - start).count());
        }
        volatile double sink = book.spread();  // keep the loop from being optimized away
        (void)sink;
    }

    std::sort(times.begin(), times.end());
    double median = times[kRuns / 2];
    std::cout << events.size() << " events, " << kRuns << " runs (median "
              << median << " s)\n"
              << "median " << events.size() / median / 1e6 << " M events/sec"
              << " (min " << events.size() / times.back() / 1e6
              << ", max " << events.size() / times.front() / 1e6 << ")\n";
}
