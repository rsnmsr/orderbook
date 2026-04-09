#ifndef ORDERBOOK_COMMON_ENUMS_HPP
#define ORDERBOOK_COMMON_ENUMS_HPP

enum Side {
    BUY = 1,
    SELL = 2
};

enum EventType {
    ADD = 1,
    CANCEL = 2
};

enum OrderType{
    LIMIT = 1,
    MARKET = 2
};

#endif