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

inline const char* eventTypeToString(EventType t) {
    switch (t) {
        case ADD: return "ADD";
        case CANCEL: return "CANCEL";
        default: return "UNKNOWN";
    }
}

enum OrderType{
    LIMIT = 1,
    MARKET = 2
};

#endif