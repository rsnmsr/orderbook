#ifndef EVENT_PARSER_HPP
#define EVENT_PARSER_HPP

#include "../common/enums.hpp"
#include "../orders/Order.hpp"
#include "../orders/LimitOrder.hpp"
#include "../orders/MarketOrder.hpp"
#include "../orderbook/orderbook.hpp"
#include "../external/json.hpp"

#include <memory>
#include <string>
#include <chrono>

using json = nlohmann::json;

class EventParser {
public:
    Event parse(const json& j);

private:
    EventType parseEventType(const std::string& s);
    Side parseSide(const std::string& s);
    OrderType parseOrderType(const std::string& s);

    std::chrono::system_clock::time_point parseTimestamp(long long ts);

    std::unique_ptr<Order> createOrder(
        const json& j,
        const std::string& order_id,
        const std::chrono::system_clock::time_point& timestamp,
        Side side,
        int quantity,
        const std::string& ticker,
        OrderType orderType
    );
};

#endif