#include <algorithm>
#include "orderbook.hpp"

OrderBook::OrderBook() {};

std::vector<TradingEvent> OrderBook::processOrder(std::unique_ptr<Order> order)
{

    std::vector<TradingEvent> trades;

    Order &orderRef = *order;

    if (orderRef.getSide() == BUY)
    {
        MatchBuyOrder(orderRef, trades);
    }
    else
    {
        MatchSellOrder(orderRef, trades);
    }

    if (!order->isMarket() && !order->isFilled()){
        addToBook(std::move(order));//  asks[order.getPrice()].push_back(std::move(order));
    }

    // after matching:
    // if LIMIT and remaining > 0 → add to book

    return trades;
}

void OrderBook::MatchBuyOrder(Order &order, std::vector<TradingEvent> &trades)
{
    while (!asks.empty() && (order.isMarket() || asks.begin()->first <= order.getPrice()))
    {
        auto best_ask_level = asks.begin();
        auto &resting = best_ask_level->second.front();
        int trade_qty = std::min(resting->getRemainingQuantity(), order.getRemainingQuantity());
        double trade_price = best_ask_level->first;
        order.fill(trade_qty);
        resting->fill(trade_qty);

        trades.push_back({
        order.getOrderId(),              // buy_id
        resting->getOrderId(),           // sell_id
        trade_qty,
        trade_price
        });

        if (resting->isFilled())
        {
            std::string resting_id = resting->getOrderId();
            best_ask_level->second.pop_front();
            order_map.erase(resting_id);
        }

        if (best_ask_level->second.empty())
        {
            asks.erase(best_ask_level);
        }

        if (order.isFilled())
        {
            break;
        }
    }
}

void OrderBook::MatchSellOrder(Order &order, std::vector<TradingEvent> &trades)
{
    while (!bids.empty() && (order.isMarket() || bids.begin()->first >= order.getPrice()))
    {
        auto best_bid_level = bids.begin();
        auto &resting = best_bid_level->second.front();
        int trade_qty = std::min(resting->getRemainingQuantity(), order.getRemainingQuantity());
        double trade_price = best_bid_level->first;
        order.fill(trade_qty);
        resting->fill(trade_qty);

        trades.push_back({
            order.getOrderId(),              // sell_id
            resting->getOrderId(),           // buy_id
            trade_qty,
            trade_price
        });

        if (resting->isFilled())
        {
            std::string resting_id = resting->getOrderId();
            best_bid_level->second.pop_front();
            order_map.erase(resting_id);
        }

        if (best_bid_level->second.empty())
        {
            bids.erase(best_bid_level);
        }

        if (order.isFilled())
        {
            break;
        }
    }

}

void OrderBook::addToBook(std::unique_ptr<Order> order){
    auto orderId = order->getOrderId();
    auto orderPrice = order->getPrice();
    auto side = order->getSide();
    if(side==BUY){
        auto& level = bids[orderPrice];
        level.push_back(std::move(order));
        auto it = std::prev(level.end());
        order_map[orderId] = { BUY, orderPrice, it };
    }else{
        auto& level = asks[orderPrice];
        level.push_back(std::move(order));
        auto it = std::prev(level.end());
        order_map[orderId] = { SELL, orderPrice, it };
    }

}