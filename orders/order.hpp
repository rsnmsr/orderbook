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
        const std::string& getOrderId() const;
        int getQuantity() const;
        const std::string& getTicker() const;
        Side getSide() const;
        const std::chrono::system_clock::time_point& getTimestamp() const;

        virtual ~Order() = default;
        virtual void execute() const = 0;

};

#endif