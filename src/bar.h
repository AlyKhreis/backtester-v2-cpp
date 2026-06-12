#ifndef BACKTESTER_BAR_H
#define BACKTESTER_BAR_H

#include <string>
using std::string;

struct Bar {
    string date;
    double  close;
    double  open;
    long long volume;
    double  high;
    double low;
};

#endif //BACKTESTER_BAR_H