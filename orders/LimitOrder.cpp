#include "LimitOrder.hpp"
#include <iostream> 

LimitOrder::LimitOrder(const std::string &order_id_,
                         const std::chrono::system_clock::time_point &timestamp_,
                         int quantity_,
                         const std::string &ticker_,
                         Side side_,
                         double price_ ) noexcept
                         :Order(order_id_,
                                timestamp_,
                                quantity_,
                                ticker_,
                                side_),
                                price(price_)
{}

    double LimitOrder::getPrice() const {
        return price;
    }

    bool LimitOrder::isMarket() const{
    return false;
}

    void LimitOrder::execute() const {
    // Do some manupulation on the order book to execute this order
    // Buy 100 APPL, and use these order attribute on a runbook
    Side s = getSide();
    std::string side_str = (s == Side::BUY) ? "BUY" : "SELL";

    int qty = getQuantity();
    int price = getPrice();
    const std::string& ticker = getTicker();

    std::cout<<"Executing "<<side_str<<" "<<qty<<" "<<ticker<<" & "<<price<<std::endl;

}
