### Part 1: Setting Up TradingView Alerts

1.  **Create or Identify Your Trading Strategy**: Before setting up alerts, you need a clear trading strategy on TradingView that defines when to buy or sell.
    
2.  **Configure Alerts**: Once your strategy is in place, you'll set up alerts in TradingView that trigger based on your strategy's conditions.
    
    -   Go to the chart where your strategy is applied.
    -   Click on the `Alert` button on the upper right side of the screen.
    -   Set the condition that matches your entry or exit strategy.
    -   In the alert action settings, choose `Webhook URL` and enter your webhook URL formatted as `https://sc.oc.aliases.me/tdv/mt5id`, replacing `mt5id` with your actual MT5 account ID.
3.  **Webhook Message**: In the `Message` field of the alert, enter the syntax for your trading signal. For example:
    
    perlCopy code
    
    `Sell,XAUUSD,open,high,low,close,no,RR,riskeachtrade,lotsize,licenseid` 
    
    Replace `Sell` with `Buy` as per your alert conditions and fill in the respective fields like `RR`, `riskeachtrade`, `lotsize`, and `licenseid` according to your strategy and risk management.
    
4.  **Save the Alert**: Once configured, save the alert. It will now trigger according to the set conditions and send signals to the specified webhook URL.
    

### Part 2: EA Setup in MT5

### Expert Advisor (EA) User Guide: DeltaClient V1

#### Introduction

DeltaClient V1 is a sophisticated trading robot designed for MetaTrader 5, aimed at automating trading strategies based on specific trade signals. It incorporates advanced features like auto trailing stop loss, partial profit-taking, and the use of stop orders for entry.

#### Installation

1.  Download the `DeltaClient.mq5` file.
2.  Open MetaTrader 5.
3.  Click on `File` > `Open Data Folder`.
4.  Navigate to `MQL5` > `Experts` or `MQL5` > `Scripts` depending on your file.
5.  Copy and paste the downloaded file into this folder.
6.  Restart MetaTrader 5.
7.  You should now see `DeltaClient` in the `Navigator` panel under `Experts` or `Scripts`. Drag it onto the chart to activate.

#### Configuration Settings

Before you run DeltaClient, understanding and configuring its settings is essential for optimal performance.

-   **Trade Settings:**
    
    -   `RiskPerTrade`: Define the risk for each trade. Leave it empty to use remote configuration. Format can be an amount (e.g., "20" for $20) or a percentage of the total balance (e.g., "1%" for 1% of the total balance).
    -   `MaxTradesPerSymbol`: Maximum number of concurrent trades allowed per symbol.
    -   `RiskReward`: The risk-reward ratio for each trade. If set to 0, the ratio from the remote signal will be used.
    -   `StlSpread`: Spread to be added or subtracted from the stop-loss price to prevent stop-out due to spread widening.
    -   `AutoTrailingSL`: Enable automatic trailing of the stop-loss when profit reaches the RR target.
    -   `TrailingStepPoints`: Number of points to trail the stop loss.
-   **Stop Order Settings:**
    
    -   `UseStopOrder`: Enable or disable the use of stop orders instead of market orders for entry.
    -   `StpAdj`: Price adjustment for stop orders to prevent immediate triggering.
-   **Profit Management:**
    
    -   `TpIfReachedTarget`: Enable or disable partial profit taking when targets are reached.
    -   `FirstTarget` & `SecondTarget`: Define the targets for partial profit taking.
-   **License & PassKey Settings:**
    
    -   `LicenseId`: Your unique license ID for receiving signals.
    -   `SlaveKey`: If you're receiving signals as a slave from a master, specify the slave passkey here.
-   **Telegram & Alert Settings:**
    
    -   Provide your Telegram bot token and chat ID to receive notifications about trade executions.
-   **Debug & Logs:**
    
    -   Enable or disable hotkeys and debugging logs for troubleshooting purposes.

#### Operational Guide

-   Once configured, `DeltaClient` will listen for trade signals from a configured source and execute trades based on the parameters set in the settings.
-   Trailing stop loss and partial profit-taking actions are automated based on the trade's progress.
-   Ensure you monitor the EA's activity regularly to adjust settings as market conditions change.

#### Tips for Optimal Use

-   Start with a demo account to familiarize yourself with EA's operations and settings.
-   Adjust `RiskPerTrade` according to your risk management strategy.
-   Use `AutoTrailingSL` and `TpIfReachedTarget` judiciously to maximize profits and minimize losses.
