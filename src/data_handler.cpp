#include "data_handler.h"
#include <sstream>
#include <algorithm>
#include <fstream>


HistoricalCSVDataHandler :: HistoricalCSVDataHandler(std::queue<std::unique_ptr<Event>>* events,
    const std::string& csv_path,
    const std::string& symbol)
        :  symbol_(symbol), current_index_(-1)
{
    events_= events;
    std::ifstream file(csv_path);
    std::string line;
    std::string header;
    getline(file, header);
    while (getline(file, line)) {
        Bar bar;
        std::stringstream ss(line);
        std::string p;

        getline(ss, bar.date, ',');

        getline(ss, p, ',');
        p.erase(0, 1);
        bar.close = stod(p);

        getline(ss, p, ',');
        bar.volume = stoll(p);

        getline(ss, p, ',');
        p.erase(0, 1);
        bar.open = stod(p);

        getline(ss, p, ',');
        p.erase(0, 1);
        bar.high = stod(p);

        getline(ss, p, ',');
        p.erase(0, 1);
        bar.low = stod(p);

        all_bars_.push_back(bar);
    }
    std::reverse(all_bars_.begin(), all_bars_.end());
}

void HistoricalCSVDataHandler::update_bars() {
    if (current_index_+1 >= all_bars_.size() ) return;
    current_index_++;
    events_->push(std::make_unique<MarketEvent>());
}

Bar HistoricalCSVDataHandler::get_latest_bar(const std::string& symbol) const {
    int idx = current_index_- latency_bars_;
    if (idx<0) idx = 0;
    return all_bars_[idx];
}

bool HistoricalCSVDataHandler::has_more_data() const{
    return current_index_ + 1 < all_bars_.size();
}

std::vector<Bar> HistoricalCSVDataHandler :: get_latest_bars(const std::string &symbol, int n) const {
    int idx = current_index_- latency_bars_;
    if (idx<0) return {};
    int start_index = idx-n+1;
    if (start_index < 0) start_index = 0;
    return std::vector<Bar> (all_bars_.begin()+start_index, all_bars_.begin()+idx+1);
}



Bar HistoricalCSVDataHandler::get_current_bar(const std::string& symbol) const {
    return all_bars_[current_index_];   // no latency, true current bar
}