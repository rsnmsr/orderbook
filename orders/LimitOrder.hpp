#ifndef LIMIT_ORDER_HPP
#define LIMIT_ORDER_HPP

#include "order.hpp"

class LimitOrder : public Order{
    private:
    int price;
    public:
    LimitOrder(const std::string &order_id_,
                         const std::chrono::system_clock::time_point &timestamp_,
                         int quantity_,
                         const std::string &ticker_,
                         Side side_,
                        int price
                    ) noexcept;

    int getPrice();
    void execute() const override;
    };


#endif