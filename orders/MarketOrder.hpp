#ifndef MARKET_ORDER_HPP
#define MARKET_ORDER_HPP

#include "order.hpp"

class MarketOrder : public Order{
    public:
    MarketOrder(const std::string& order_id, 
              const std::chrono::system_clock::time_point& timestamp, 
              int quantity, 
              const std::string& ticker, 
              Side side
            )noexcept;
            
            void execute() const override;

};

#endif