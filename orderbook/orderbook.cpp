#include "orderbook.hpp"

OrderBook::OrderBook(){};

std::vector<TradingEvent> OrderBook::processOrder(std::unique_ptr<Order> order){

    std::vector<TradingEvent> trades;

    Order& orderRef = *order;

    if(orderRef.getSide() == BUY){
        MatchBuyOrder(orderRef, trades);
    } else {
        MatchSellOrder(orderRef, trades);
    }

    // after matching:
    // if LIMIT and remaining > 0 → add to book

    return trades;

}

void OrderBook::MatchBuyOrder(Order& order, std::vector<TradingEvent>& trades){
    while (!asks.empty() && (order.isMarket() || asks.begin()->first<=order.getPrice())){
        auto best_ask_level = asks.begin();

    }

}

void OrderBook::MatchBuyOrder(Order& order, std::vector<TradingEvent>& trades){
    while (!bids.empty() && (order.isMarket() || bids.begin()->first>=order.getPrice())){

    }

}
