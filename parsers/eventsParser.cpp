#include "eventsParser.hpp"
#include <stdexcept>

EventType EventParser::parseEventType(const std::string& s) {
    if (s == "ADD") return EventType::ADD;
    if (s == "CANCEL") return EventType::CANCEL;
    throw std::runtime_error("Unknown event type");
}

Side EventParser::parseSide(const std::string& s) {
    if (s == "BUY") return Side::BUY;
    if (s == "SELL") return Side::SELL;
    throw std::runtime_error("Unknown side");
}

OrderType EventParser::parseOrderType(const std::string& s) {
    if (s == "LIMIT") return OrderType::LIMIT;
    if (s == "MARKET") return OrderType::MARKET;
    throw std::runtime_error("Unknown order type");
}

std::chrono::system_clock::time_point 
EventParser::parseTimestamp(long long ts) {
    return std::chrono::system_clock::time_point(
        std::chrono::nanoseconds(ts)
    );
}

std::unique_ptr<Order> EventParser::createOrder(
    const json& j,
    const std::string& order_id,
    const std::chrono::system_clock::time_point& timestamp,
    Side side,
    int quantity,
    const std::string& ticker,
    OrderType orderType
) {
    if (orderType == MARKET) {
        return std::make_unique<MarketOrder>(
            order_id, timestamp, quantity, ticker, side
        );
    }

    // LIMIT order
    if (!j.contains("price")) {
        throw std::runtime_error("LIMIT order missing price");
    }

    double price = j.at("price").get<double>();

    return std::make_unique<LimitOrder>(
        order_id, timestamp, quantity, ticker, side, price
    );
}

Event EventParser::parse(const json& j) {

    std::string eventTypeStr = j.at("event_type").get<std::string>();
    EventType type = parseEventType(eventTypeStr);

    // -------- CANCEL --------
    if (type == EventType::CANCEL) {
        Event e;
        e.type = EventType::CANCEL;
        e.order = nullptr;
        e.order_id = j.at("orderid").get<std::string>();
        return e;
    }

    // -------- ADD --------
    std::string order_id = j.at("orderid").get<std::string>();
    long long ts = j.at("timestamp").get<long long>();
    auto timestamp = parseTimestamp(ts);

    Side side = parseSide(j.at("direction").get<std::string>());
    int quantity = j.at("quantity").get<int>();
    std::string ticker = j.at("ticker").get<std::string>();
    OrderType orderType = parseOrderType(j.at("order_type").get<std::string>());

    std::unique_ptr<Order> order = createOrder(
        j, order_id, timestamp, side, quantity, ticker, orderType
    );

    Event e;
    e.type = EventType::ADD;
    e.order = std::move(order);
    e.order_id = order_id;

    return e;
}