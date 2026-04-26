#ifndef VALIDATOR_HPP
#define VALIDATOR_HPP

#include "../orderbook/orderbook.hpp"

class Validator {
private:
    static void validateNoEmptyPriceLevels(const OrderBook& book);
    static void validatePositiveQuantities(const OrderBook& book);

public:
    static void validate(const OrderBook& book);

};

#endif