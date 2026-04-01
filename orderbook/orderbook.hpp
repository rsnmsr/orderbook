
#ifndef ORDERBOOK_HPP
#define ORDERBOOK_HPP

//  Include headers

#include <map>
#include <deque>
#include <string>
#include <memory>
#include <vector>
#include <functional>
#include <unordered_map>
#include "../orders/Order.hpp"
#include "../orders/LimitOrder.hpp"
#include "../orders/MarketOrder.hpp"

using OrderQueue = std::deque<std::unique_ptr<Order>>;
using OrderIt = OrderQueue::iterator;

struct TradingEvent{
   std::string sell_id;
   std::string buy_id;
   int quantity;
   double price;
};

struct OrderLocation {
    Side side;
    double price;
    OrderIt queue_iterator;
};

struct Event {
    EventType type;
    std::unique_ptr<Order> order;  
    std::string order_id;          
};

class OrderBook{
    private:
    std::map <double, OrderQueue, std::greater<double>> bids;
    std::map <double, OrderQueue> asks;
    std::unordered_map<std::string, OrderLocation> order_map;
    void addToBook(std::unique_ptr<Order> order);  
    void MatchBuyOrder(Order& order, std::vector<TradingEvent>& trades);
    void MatchSellOrder(Order& order, std::vector<TradingEvent>& trades);
    public:
    OrderBook();
    
    void cancelOrder(const std::string& order_id);
//    std::vector<TradingEvent> processOrder(std::unique_ptr<Order> order);
    std::vector<TradingEvent> processEvent(Event event);

};
#endif