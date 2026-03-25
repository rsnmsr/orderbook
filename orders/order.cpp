#include "order.hpp"

Order::Order(const std::string &order_id_,
             const std::chrono::system_clock::time_point &timestamp_,
             int quantity_,
             const std::string &ticker_,
             Side side_) noexcept
    : order_id(order_id_),
      timestamp(timestamp_),
      quantity(quantity_),
      ticker(ticker_),
      side(side_)
{
}