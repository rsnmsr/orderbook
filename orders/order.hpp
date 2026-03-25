#ifndef ORDER_HPP
#define ORDER_HPP
#include<string>
#include<chrono>
#include<enums.hpp>

class Order{
    protected:
        std::string order_id;
        std::chrono::system_clock::time_point timestamp;
        int quantity;
        std::string ticker;
        Side side;

    public:
         Order (const std::string& order_id, 
              const std::chrono::system_clock::time_point& timestamp, 
              int quantity, 
              const std::string& ticker, 
              Side side
            )noexcept;

        // We have various getters to obtain the order state 
        const std::string& getOrderId();
        int getQuantity();
        const std::string& getTicker();
        Side getSide();
        const std::chrono::system_clock::time_point& getTimestamp();

        virtual ~Order() = default;
        virtual void execute() = 0;

};

#endif