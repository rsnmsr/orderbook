#ifndef LIMIT_ORDER_HPP
#define LIMIT_ORDER_HPP

#include "order.hpp"

class LimitOrder : public Order{
    private:
    int price_;
    public:
    LimitOrder(const std::string &order_id_,
                         const std::chrono::system_clock::time_point &timestamp_,
                         int quantity_,
                         const std::string &ticker_,
                         Side side_,
                        int price_
                    ) noexcept;

    int getPrice() const {return price_;}
    void execute() const override;
    };


#endif