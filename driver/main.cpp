#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include "../external/json.hpp"
#include "../parsers/eventsParser.hpp"
#include "../debug/Logger.hpp"
#include "../orderbook/orderbook.hpp"
#include "../debug/Validator.hpp"

using json = nlohmann::json;

#include "../common/enums.hpp"

int main(int argc, char **argv)
{

    bool logInited = Logger::initLog("logs/orderbook.log", true);

    std::string filename;
    if (argc > 1)
    {
        filename = argv[1];
    }
    else
    {
        if (std::ifstream("test_generation/generated_events.json"))
        {
            filename = "test_generation/generated_events.json";
        }
        else if (std::ifstream("../test_generation/generated_events.json"))
        {
            filename = "../test_generation/generated_events.json";
        }
        else
        {
            std::cerr << "Error: Could not find generated_events.json in expected locations\n";
            if (logInited)
                Logger::shutdownLog();
            return 1;
        }
    }

    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file '" << filename << "'\n";
        if (logInited)
            Logger::shutdownLog();
        return 1;
    }

    try
    {
        json j;
        file >> j;

        OrderBook book;
        size_t totalTrades = 0;
        auto start = std::chrono::high_resolution_clock::now();
        // Example: Accessing data
        for (const auto &obj : j)
        {
            if (!obj.is_object())
                continue;
            Event e = EventParser().parse(obj);
            Logger::input("Parsed event successfully");
            std::string dispatchMsg = std::string("Dispatching event of type : ") + eventTypeToString(e.type) + " to orderbook";
            Logger::engine(dispatchMsg);
            auto trades = book.processEvent(std::move(e));
            totalTrades += trades.size();

#ifndef NDEBUG
            Validator::validate(book);
#endif

            // Validator::validate(book);

            for (const auto &t : trades)
            {
                std::string msg = "Trade: buy=" + t.buy_id + " sell=" + t.sell_id + " qty=" + std::to_string(t.quantity) + " price=" + std::to_string(t.price);
                Logger::engine(msg);
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto durationUs = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        std::cout << "\n===== BENCHMARK =====\n";
        std::cout << "Events Processed : " << j.size() << "\n";
        std::cout << "Trades Executed  : " << totalTrades << "\n";
        std::cout << "Total Time (us)  : " << durationUs << "\n";
        std::cout << "Avg Latency/event: "
                  << static_cast<double>(durationUs) / j.size()
                  << " us\n";
        std::cout << "Throughput       : "
                  << (j.size() * 1e6 / durationUs)
                  << " events/sec\n";
    }
    catch (const json::parse_error &e)
    {
        std::cerr << "Parse error at byte " << e.byte << ": " << e.what() << "\n";
        if (logInited)
            Logger::shutdownLog();
        return 1;
    }
    catch (const json::type_error &e)
    {
        std::cerr << "Type error: " << e.what() << "\n";
        if (logInited)
            Logger::shutdownLog();
        return 1;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        if (logInited)
            Logger::shutdownLog();
        return 1;
    }

    if (logInited)
        Logger::shutdownLog();
    return 0;
}