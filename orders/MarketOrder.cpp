#include "MarketOrder.hpp"
#include <iostream> 

MarketOrder::MarketOrder(const std::string &order_id_,
                         const std::chrono::system_clock::time_point &timestamp_,
                         int quantity_,
                         const std::string &ticker_,
                         Side side_) : Order(order_id_,
                                             timestamp_,
                                             quantity_,
                                             ticker_,
                                             side_)

{
}

bool MarketOrder::isMarket() const{
    return true;
}


double MarketOrder::getPrice() const{
    return 0;
}

void MarketOrder::execute() const {
    // Do some manupulation on the order book to execute this order
    // Buy 100 APPL, and use these order attribute on a runbook
    Side s = getSide();
    std::string side_str = (s == Side::BUY) ? "BUY" : "SELL";

    int qty = getQuantity();
    const std::string& ticker = getTicker();

    std::cout<<"Executing "<<side_str<<" "<<qty<<" "<<ticker<<std::endl;

}