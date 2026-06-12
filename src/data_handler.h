#ifndef BACKTESTER_DATA_HANDLER_H
#define BACKTESTER_DATA_HANDLER_H
#include <string>
#include <vector>
#include "bar.h"
#include <queue>
#include <memory>
#include "event.h"


class DataHandler {
public:
    virtual void update_bars() = 0;

    virtual Bar get_latest_bar(const std::string& symbol) const = 0;

    virtual std::vector<Bar> get_latest_bars(const std::string& symbol, int n) const = 0;

    virtual bool has_more_data() const = 0;

    virtual ~DataHandler() = default;
protected:
    std::queue<std::unique_ptr<Event>> *events_;
};

class HistoricalCSVDataHandler : public DataHandler {
public:
    HistoricalCSVDataHandler(std::queue<std::unique_ptr<Event>>* events,
                             const std::string& csv_path,
                             const std::string& symbol);

    void update_bars() override;
    Bar get_latest_bar(const std::string& symbol) const override;
    std::vector<Bar> get_latest_bars(const std::string& symbol, int n) const override;
    bool has_more_data() const override;

private:
    std::string symbol_;
    std::vector<Bar> all_bars_;     // entire dataset loaded once
    int current_index_;
};


#endif
