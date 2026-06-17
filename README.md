# Event-Driven Backtester (C++)

A C++ backtesting engine for equity trading strategies built around an event-driven
architecture with four core components — DataHandler, Strategy, Portfolio, and
ExecutionHandler. Designed so the same strategy code runs in both backtesting and
(future) live trading by swapping only the data and execution layers. 


## Architecture 
```
┌─────────────────────────────────────────────────────────────────┐
│                       BACKTEST ENGINE                            │
│                  (event queue + dispatch loop)                   │
└─────────────────────────────────────────────────────────────────┘
                                │
              ┌─────────────────┴─────────────────┐
              │                                   │
        push MarketEvent                   pop & dispatch
              │                                   ▼
       ┌──────────────┐                    ┌──────────────┐
       │ DataHandler  │   get_latest_bar   │   Strategy   │
       │              │  ◄──────────────── │              │
       │ Reads CSV    │                    │ MA / RSI /   │
       │ Owns bars    │                    │ Momentum     │
       └──────────────┘                    └──────────────┘
                                                  │
                                          push SignalEvent
                                                  ▼
                                          ┌──────────────┐
                                          │  Portfolio   │
                                          │              │
                                          │ Cash,        │
                                          │ positions,   │
                                          │ equity curve │
                                          └──────────────┘
                                              │       ▲
                              push OrderEvent │       │ FillEvent
                                              ▼       │ updates state
                                       ┌──────────────────┐
                                       │ ExecutionHandler │
                                       │                  │
                                       │ Slippage         │
                                       │ Commission       │
                                       └──────────────────┘
```

## Strategies

- **Moving Average Crossover** — buys when fast MA crosses above slow MA, exits on the reverse cross. Parameters: fast period, slow period.
- **RSI** — buys when RSI drops below the oversold threshold, exits when it rises above overbought. Parameters: period, overbought, oversold.
- **Momentum** — buys when price is above its value N bars ago, exits when below. Parameter: lookback period.

## Sample Results

Backtested on AAPL daily bars, 2021–2026 (1255 bars), starting capital $100,000,
FixedFractionalPortfolio at 95% equity per trade, 5bp slippage, $0.005/share commission ($1 min).

| Strategy         | Parameters    | Return | Trades | Win Rate | Sharpe | Max DD | Profit Factor |
|------------------|---------------|--------|--------|---------|--------|--------|---------------|
| MA Crossover     | (10, 50)      | 60.08% | 17     | 29.41%  | 0.67   | -23.5% | 1.95          |
| RSI              | (14, 70, 30)  | 49.42% | 15     | 73.33%  | 0.51   | -28.6% | 3.08          |
| Momentum         | (40)          | 33.66% | 51     | 23.5%   | 0.44   | -33.0% | 1.24          |
## Build

Requires CMake 3.20+ and a C++17 compiler.

\`\`\`bash
git clone https://github.com/AlyKhreis/backtester-v2-cpp.git
cd backtester-v2-cpp
mkdir cmake-build-debug && cd cmake-build-debug
cmake ..
make
./backtester
\`\`\`

## Usage

The program prompts for a strategy and its parameters at runtime, runs the backtest
on `data/AAPL.csv`, prints performance metrics, and exports `trades.csv` and
`equity.csv` to the build directory.

\`\`\`
Select Strategy:
1. Moving Average Crossover
2. RSI
3. Momentum
   Choice: 3
   Period: 40

Final equity: $133658
Total return: 33.6584%
Bars: 1255
Sharpe ratio: 0.436437
Max drawdown: -33.0406%
CAGR: 5.99848%
Trades: 51
Win rate: 23.5294%
Profit factor: 1.24353
\`\`\`



## What's Modeled

- **Event-driven execution** — every interaction routed through a typed event queue;
  no component calls another directly.
- **Slippage** — configurable basis-point cost applied to fill price.
- **Commission** — per-share with a per-trade minimum (Interactive Brokers model).
- **Position sizing** — pluggable via the Portfolio interface; two implementations
  shipped (fixed-share `NaivePortfolio` and percent-of-equity `FixedFractionalPortfolio`).
- **Risk-adjusted metrics** — Sharpe, max drawdown, CAGR, win rate, profit factor.
- **Trade log** — every round-trip with entry/exit dates, prices, P&L, exported to CSV.

## What's Not Modeled

- **Latency** — strategy sees each bar instantly; in reality there's a real delay
  between market data arrival and strategy action that erodes signal quality.
- **Partial fills** — every order fills in full at the simulated price; real markets
  often fill orders in chunks across multiple price levels.
- **Limit order queue position** — limit orders aren't simulated; only market orders
  are supported.
- **Multi-symbol** — DataHandler is single-symbol; portfolio logic is symbol-agnostic
  and would generalize, but data layer needs extending.
- **Live data** — the architecture supports it (just swap DataHandler and
  ExecutionHandler), but no live connector is implemented.
- **Walk-forward validation** — strategies are evaluated on a single in-sample window;
  results are likely overfit.



## Roadmap

- Latency simulation between bar arrival and strategy access, measured impact on Sharpe
- Walk-forward validation across multiple train/test windows
- Multi-symbol DataHandler with synchronized bar streams
- Limit orders with queue-position-aware simulated fills
- Live data + broker connector (Interactive Brokers / Alpaca) using the existing
  Strategy and Portfolio interfaces unchanged
- Benchmark suite reporting events/sec and per-event latency percentiles
