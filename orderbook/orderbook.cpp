#include <algorithm>
#include "orderbook.hpp"
#include "../debug/Logger.hpp"

OrderBook::OrderBook() {};

std::vector<TradingEvent> OrderBook::processEvent(Event event)
{

    std::vector<TradingEvent> trades;

    if (event.type == CANCEL)
    {
        cancelOrder(event.order_id);
    }
    else
    {

    std::unique_ptr<Order> incoming = std::move(event.order);
    Order& order = *incoming;

    if (order.getSide() == BUY) {
        MatchBuyOrder(order, trades);
    } else {
        MatchSellOrder(order, trades);
    }

    if (!order.isMarket() && !order.isFilled()) {
        addToBook(std::move(incoming));
    }

    }
    
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

        trades.push_back({order.getOrderId(),    // buy_id
                          resting->getOrderId(), // sell_id
                          trade_qty,
                          trade_price});

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

        trades.push_back({resting->getOrderId(), // buy_id
                          order.getOrderId(),    // sell_id
                          trade_qty,
                          trade_price});

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

void OrderBook::addToBook(std::unique_ptr<Order> order)
{
    auto orderId = order->getOrderId();
    auto orderPrice = order->getPrice();
    auto side = order->getSide();
    if (side == BUY)
    {
        auto &level = bids[orderPrice];
        level.push_back(std::move(order));
        auto it = std::prev(level.end());
        order_map[orderId] = {BUY, orderPrice, it};
    }
    else
    {
        auto &level = asks[orderPrice];
        level.push_back(std::move(order));
        auto it = std::prev(level.end());
        order_map[orderId] = {SELL, orderPrice, it};
    }
}

void OrderBook::cancelOrder(const std::string &order_id)
{
    auto cancelLocation = order_map.find(order_id);
    if (cancelLocation == order_map.end())
        return;

    Side side = cancelLocation->second.side;
    double price = cancelLocation->second.price;
    OrderIt it = cancelLocation->second.queue_iterator;

    if (side == BUY)
    {
        auto levelIt = bids.find(price);
        if (levelIt != bids.end())
        {
            auto &level = levelIt->second;
            level.erase(it);
            if (level.empty())
            {
                bids.erase(price);
            }
        }
    }
    else
    {
        auto levelIt = asks.find(price);
        if (levelIt != asks.end())
        {
            auto &level = levelIt->second;
            level.erase(it);
            if (level.empty())
            {
                asks.erase(price);
            }
        }
    }
    order_map.erase(order_id);
}