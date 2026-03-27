#include "LimitOrder.hpp"

LimitOrder::LimitOrder(const std::string &order_id_,
                         const std::chrono::system_clock::time_point &timestamp_,
                         int quantity_,
                         const std::string &ticker_,
                         Side side_,
                         int price_ ) noexcept
                         :Order(order_id_,
                                timestamp_,
                                quantity_,
                                ticker_,
                                side_),
                                price(price_)
{}

    int LimitOrder::getPrice() {
        return price;
    }
