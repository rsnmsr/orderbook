#include<iostream>
#include<fstream>
#include <string>
#include "../external/json.hpp"
#include "../parsers/eventsParser.hpp"
#include "../debug/Logger.hpp"
#include "../orderbook/orderbook.hpp"

using json = nlohmann::json;


int main(int argc, char** argv){

    bool logInited = Logger::initLog("logs/orderbook.log", true);

    std::string filename;
    if (argc > 1) {
        filename = argv[1];
    } else {
        if (std::ifstream("test_generation/generated_events.json")) {
            filename = "test_generation/generated_events.json";
        } else if (std::ifstream("../test_generation/generated_events.json")) {
            filename = "../test_generation/generated_events.json";
        } else {
            std::cerr << "Error: Could not find generated_events.json in expected locations\n";
            if (logInited) Logger::shutdownLog();
            return 1;
        }
    }

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << filename << "'\n";
        if (logInited) Logger::shutdownLog();
        return 1;
    }

    try {
    json j;
    file >> j;

    OrderBook book;

    // Example: Accessing data
    for (const auto& obj : j) {
        if (!obj.is_object()) continue;
        Event e = EventParser().parse(obj);
        Logger::input("Parsed event successfully");

        auto trades = book.processEvent(std::move(e));
        Logger::engine("Dispatching event to orderbook");

        for (const auto &t : trades) {
            std::string msg = "Trade: buy=" + t.buy_id + " sell=" + t.sell_id + " qty=" + std::to_string(t.quantity) + " price=" + std::to_string(t.price);
            Logger::engine(msg);
        }
    }

    } catch (const json::parse_error& e) {
        std::cerr << "Parse error at byte " << e.byte << ": " << e.what() << "\n";
        if (logInited) Logger::shutdownLog();
        return 1;
    } catch (const json::type_error& e) {
        std::cerr << "Type error: " << e.what() << "\n";
        if (logInited) Logger::shutdownLog();
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        if (logInited) Logger::shutdownLog();
        return 1;
    }

    if (logInited) Logger::shutdownLog();
    return 0;
}