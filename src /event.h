#ifndef EVENT_H
#define EVENT_H
#include <string>
enum class EventType {
    MARKET,
    SIGNAL,
    ORDER,
    FILL
};

enum class OrderType { MARKET, LIMIT };

struct Event {
    virtual EventType type() const = 0;
    virtual ~Event() = default;
};

struct MarketEvent : Event {
    EventType type() const override { return EventType::MARKET; }
};

struct OrderEvent : Event {
    std::string symbol;
    OrderType  order_type;
    int quantity;
    double limit_price;
    std::string direction;

    EventType type() const override { return EventType::ORDER; }
};
struct FillEvent : Event {
    std::string symbol;
    std::string direction;
    int quantity;
    double fill_price;
    double commission;
    double slippage;
    EventType type() const override { return EventType::FILL; }
};

struct SignalEvent : Event {
    std::string symbol;
    std::string direction;
    double strength;
    EventType type() const override { return EventType::SIGNAL; }
};


#endif