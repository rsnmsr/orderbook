#include "Validator.hpp"
#include <stdexcept>

void Validator::validate(const OrderBook& book) {
    if (!book.getBids().empty() && !book.getAsks().empty()) {
        double bestBid = book.getBids().begin()->first;
        double bestAsk = book.getAsks().begin()->first;

        if (bestBid >= bestAsk) {
            throw std::runtime_error("Crossed book detected");
        }
    }
    
    validateNoEmptyPriceLevels(book);
    validatePositiveQuantities(book);
}

void Validator::validatePositiveQuantities(const OrderBook& book) {
    const auto& bids = book.getBids();
    const auto& asks = book.getAsks();

    for (const auto& [price, queue] : bids) {
        for (const auto& orderPtr : queue) {
            if (orderPtr->getRemainingQuantity() <= 0) {
                throw std::runtime_error(
                    "Non-positive remaining quantity in bid book for order: " +
                    orderPtr->getOrderId()
                );
            }
        }
    }

    for (const auto& [price, queue] : asks) {
        for (const auto& orderPtr : queue) {
            if (orderPtr->getRemainingQuantity() <= 0) {
                throw std::runtime_error(
                    "Non-positive remaining quantity in ask book for order: " +
                    orderPtr->getOrderId()
                );
            }
        }
    }
}

void Validator::validateNoEmptyPriceLevels(const OrderBook& book) {
    const auto& bids = book.getBids();
    const auto& asks = book.getAsks();

    for (const auto& [price, queue] : bids) {
        if (queue.empty()) {
            throw std::runtime_error(
                "Empty bid price level detected at price: " +
                std::to_string(price)
            );
        }
    }

    for (const auto& [price, queue] : asks) {
        if (queue.empty()) {
            throw std::runtime_error(
                "Empty ask price level detected at price: " +
                std::to_string(price)
            );
        }
    }
}