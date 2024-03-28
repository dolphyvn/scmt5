// The top of every source code file must include this line
#include "sierrachart.h"
//#include "C:\git\vcpkg\packages\curl_x64-windows\include\curl\curl.h"
//#include "curl\curl.h"
// For reference, refer to this page:
// https://www.sierrachart.com/index.php?page=doc/AdvancedCustomStudyInterfaceAndLanguage.php

// This line is required. Change the text within the quote
// marks to what you want to name your group of custom studies. 
SCDLLName("SignalBackTest")





//This is the basic framework of a study function. Change the name 'TemplateFunction' to what you require.
SCSFExport scsf_SignalBackTest(SCStudyInterfaceRef sc)
{
	//Define references to the Subgraphs and Inputs for easy reference
	SCSubgraphRef Subgraph_BuyEntry = sc.Subgraph[0];
	SCSubgraphRef Subgraph_BuyExit = sc.Subgraph[1];
	SCSubgraphRef Subgraph_SellEntry = sc.Subgraph[2];
	SCSubgraphRef Subgraph_SellExit = sc.Subgraph[3];

	SCSubgraphRef Subgraph_SimpMovAvg = sc.Subgraph[4];

	SCInputRef Input_Enabled = sc.Input[0];
	SCInputRef Input_TargetValue = sc.Input[1];
	SCInputRef Input_StopValue = sc.Input[2];

	SCInputRef Input_StudySubgraph1 = sc.Input[3];
	SCInputRef Input_StudySubgraph2 = sc.Input[4];
    SCInputRef Input_RiskPerTrade = sc.Input[5];
    SCInputRef Input_RiskRewardRatio = sc.Input[6];
    SCInputRef Input_ContractSize = sc.Input[7];

	if (sc.SetDefaults)
	{
		// Set the study configuration and defaults.

		sc.GraphName = "Trading base on signal";
		Input_StudySubgraph1.Name = "Study for Buy Signal";
		Input_StudySubgraph1.SetStudySubgraphValues(0,0);
        //Input_StudySubgraph1.SetChartStudySubgraphValues(0,0,0);

		Input_StudySubgraph2.Name = "Study for Sell Signal";
		Input_StudySubgraph2.SetStudySubgraphValues(0,0);

		Subgraph_BuyEntry.Name = "Buy Entry";
		Subgraph_BuyEntry.DrawStyle = DRAWSTYLE_ARROW_UP;
		Subgraph_BuyEntry.PrimaryColor = RGB(0, 255, 0);
		Subgraph_BuyEntry.LineWidth = 2;
		Subgraph_BuyEntry.DrawZeros = false;

		Subgraph_BuyExit.Name = "Buy Exit";
		Subgraph_BuyExit.DrawStyle = DRAWSTYLE_ARROW_DOWN;
		Subgraph_BuyExit.PrimaryColor = RGB(255, 128, 128);
		Subgraph_BuyExit.LineWidth = 2;
		Subgraph_BuyExit.DrawZeros = false;

		Subgraph_SellEntry.Name = "Sell Entry";
		Subgraph_SellEntry.DrawStyle = DRAWSTYLE_ARROW_DOWN;
		Subgraph_SellEntry.PrimaryColor = RGB(255, 0, 0);
		Subgraph_SellEntry.LineWidth = 2;
		Subgraph_SellEntry.DrawZeros = false;

		Subgraph_SellExit.Name = "Sell Exit";
		Subgraph_SellExit.DrawStyle = DRAWSTYLE_ARROW_UP;
		Subgraph_SellExit.PrimaryColor = RGB(128, 255, 128);
		Subgraph_SellExit.LineWidth = 2;
		Subgraph_SellExit.DrawZeros = false;

		// Subgraph_SimpMovAvg.Name = "Simple Moving Average";
		// Subgraph_SimpMovAvg.DrawStyle = DRAWSTYLE_LINE;
		// Subgraph_SimpMovAvg.DrawZeros = false;

		Input_Enabled.Name = "Enabled";
		Input_Enabled.SetYesNo(0); 

		Input_TargetValue.Name = "Target Value";
		Input_TargetValue.SetFloat(1.0f);

		Input_StopValue.Name = "Stop Value";
		Input_StopValue.SetFloat(1.0f);

		Input_RiskPerTrade.Name = "Risk Per Trade";
		Input_RiskPerTrade.SetFloat(20.0f);

		Input_RiskRewardRatio.Name = "Risk Reward Ratio";
		Input_RiskRewardRatio.SetFloat(3.0f);

		Input_ContractSize.Name = "Contract Size";
		Input_ContractSize.SetFloat(100.0);

		sc.StudyDescription = "This study function is an example of how to use the ACSIL Trading Functions. This function will display a simple moving average and perform a Buy Entry when the Last price crosses the moving average from below and a Sell Entry when the Last price crosses the moving average from above. A new entry cannot occur until the Target or Stop has been hit. When an order is sent, a corresponding arrow will appear on the chart to show that an order was sent. This study will do nothing until the Enabled Input is set to Yes.";

		sc.AutoLoop = 0;
		sc.GraphRegion = 0;

		//Any of the following variables can also be set outside and below the sc.SetDefaults code block

		sc.AllowMultipleEntriesInSameDirection = true; 
		sc.MaximumPositionAllowed = 1000;
		sc.SupportReversals = true;

		// This is false by default. Orders will go to the simulation system always.
		sc.SendOrdersToTradeService = false;

		sc.AllowOppositeEntryWithOpposingPositionOrOrders = true;
		sc.SupportAttachedOrdersForTrading = false;
		sc.CancelAllOrdersOnEntriesAndReversals= true;
		sc.AllowEntryWithWorkingOrders = true;

		// Only 1 trade for each Order Action type is allowed per bar.
		sc.AllowOnlyOneTradePerBar = false; 

		//This needs to be set to true when a trading study uses trading functions.
		sc.MaintainTradeStatisticsAndTradesData = true;


		return;
	}

	if (!Input_Enabled.GetYesNo())
		return;

    // Additional logic to track high/low of the signal candle
    SCFloatArrayRef HighArray = sc.High;
    SCFloatArrayRef LowArray = sc.Low;

    // Presumably, you have a method to track whether an entry was made on the current bar
    // For the purpose of this example, let's assume these persistent variables track it
    int& r_LastBuySignalIndex = sc.GetPersistentInt(1);
    int& r_LastSellSignalIndex = sc.GetPersistentInt(2);

    SCFloatArray Study1Array, Study2Array;
    sc.GetStudyArrayUsingID(Input_StudySubgraph1.GetStudyID(), Input_StudySubgraph1.GetSubgraphIndex(), Study1Array);
    sc.GetStudyArrayUsingID(Input_StudySubgraph2.GetStudyID(), Input_StudySubgraph2.GetSubgraphIndex(), Study2Array);	


	SCFloatArrayRef Last = sc.Close;
	
	// Calculate the moving average
	//sc.SimpleMovAvg(Last, Subgraph_SimpMovAvg,  sc.Index, 10);
	

	if (sc.IsFullRecalculation)
		return;

	// Get the Trade Position data to be used for position exit processing.
	s_SCPositionData PositionData;
	sc.GetTradePosition(PositionData) ;


	float LastTradePrice = sc.Close[sc.Index];

	int64_t& r_BuyEntryInternalOrderID = sc.GetPersistentInt64(1);

	// Create an s_SCNewOrder object. 
	s_SCNewOrder NewOrder;
	NewOrder.OrderQuantity = 1;
	NewOrder.OrderType = SCT_ORDERTYPE_MARKET;
	NewOrder.TextTag = "Trade tag";
    float dollarRiskPerTrade = Input_RiskPerTrade.GetFloat();
    float ContractSize = Input_ContractSize.GetFloat();
	NewOrder.TimeInForce = SCT_TIF_GOOD_TILL_CANCELED;

	int Result = 0;

	//Optional: Check if within allowed time range. In this example we will use 10:00 through 14:00. This is according to the time zone of the chart.
	//int BarTime = sc.BaseDateTimeIn[sc.Index].GetTime();
	//bool TradingAllowed = BarTime >= HMS_TIME(10,  0, 0) && BarTime  < HMS_TIME(14,  0, 0);

	bool TradingAllowed = true;

    for (int Index = sc.UpdateStartIndex; Index < sc.ArraySize; Index++)
    { 
        // Ensure we only act on the last bar when it has closed
        // if ((sc.UpdateStartIndex == sc.ArraySize - 2))
        // {
            int BarIndex = sc.ArraySize - 2;
            //Handle BUY signal
            if (Study1Array[BarIndex] > 1 && TradingAllowed)
            {
                float PriceMovement = sc.Close[BarIndex] -  sc.Low[BarIndex];
                float dollarPerPip = PriceMovement * ContractSize;
                float quantityOfTrade = dollarRiskPerTrade/dollarPerPip;
                //NewOrder.OrderQuantity = quantityOfTrade;
                //NewOrder.Price1 = sc.Low[sc.Index];
                Result = static_cast<int>(sc.BuyEntry(NewOrder));
                //Result = sc.BuyOrder(NewOrder);
                //If there has been a successful order entry, then draw an arrow at the low of the bar.
                if (Result > 0)
                {
                    r_BuyEntryInternalOrderID = NewOrder.InternalOrderID;
                    SCString InternalOrderIDNumberString;
                    InternalOrderIDNumberString.Format("BuyEntry Internal Order ID: %d", r_BuyEntryInternalOrderID);
                    sc.AddMessageToLog(InternalOrderIDNumberString, 0);

                    Subgraph_BuyEntry[BarIndex] = sc.Low[BarIndex];
                }
            }

            // Example for buy exits
            if(PositionData.PositionQuantity > 0) // We have a long position
            {
                // Retrieve the low of the buy signal bar
                float BuySignalBarLow = LowArray[r_LastBuySignalIndex];
                float RiskAmount = (float)PositionData.AveragePrice - BuySignalBarLow;
                // Exit logic based on the low of the signal bar
                //if((LastTradePrice <= BuySignalBarLow) || (LastTradePrice >= PositionData.AveragePrice + RiskAmount*Input_RiskRewardRatio.GetFloat())) // Last price crosses below the low of the signal bar
                if((LastTradePrice <= PositionData.AveragePrice - Input_StopValue.GetFloat()) || (LastTradePrice >= PositionData.AveragePrice + Input_TargetValue.GetFloat()))
                {
                    Result = static_cast<int>(sc.BuyExit(NewOrder));
                    if (Result > 0)
                    {
                        Subgraph_BuyExit[BarIndex] = sc.High[BarIndex]; // Assuming you want to mark the exit on the chart
                        r_LastBuySignalIndex = 0; // Reset the index since we exited
                    }
                }
            }

            // Handling for Sell Signal
            if (Study2Array[BarIndex] > 1 && TradingAllowed)
            {
                float PriceMovement = sc.High[BarIndex] -  sc.Close[BarIndex];
                float dollarPerPip = PriceMovement * ContractSize;
                float quantityOfTrade = dollarRiskPerTrade/dollarPerPip;
                //NewOrder.OrderQuantity = quantityOfTrade;
                Result = static_cast<int>(sc.SellEntry(NewOrder));
                
                // If there has been a successful order entry, then draw an arrow at the high of the bar.
                if (Result > 0)
                {
                    Subgraph_SellEntry[BarIndex] = sc.High[BarIndex];
                }            
            }


            // For sell exits
            if(PositionData.PositionQuantity < 0) // We have a short position
            {
                // Retrieve the high of the sell signal bar
                float SellSignalBarHigh = HighArray[r_LastSellSignalIndex];
                float RiskAmount = SellSignalBarHigh - (float)PositionData.AveragePrice;
                // Exit logic based on the high of the signal bar
                //if((LastTradePrice >= SellSignalBarHigh) || (LastTradePrice <= PositionData.AveragePrice - RiskAmount*Input_RiskRewardRatio.GetFloat())) // Last price crosses above the high of the signal bar
                if( (LastTradePrice >= PositionData.AveragePrice + Input_StopValue.GetFloat()) || (LastTradePrice <= PositionData.AveragePrice - Input_TargetValue.GetFloat()) )
                {
                    Result = static_cast<int>(sc.SellExit(NewOrder));
                    if (Result > 0)
                    {
                        Subgraph_SellExit[BarIndex] = sc.Low[BarIndex]; // Assuming you want to mark the exit on the chart
                        r_LastSellSignalIndex = 0; // Reset the index since we exited
                    }
                }
            }


            //Example to check the status of the order using the Internal Order ID remembered in a reference to a persistent variable.
            if (r_BuyEntryInternalOrderID != 0)
            {
                s_SCTradeOrder TradeOrder;
                sc.GetOrderByOrderID(r_BuyEntryInternalOrderID, TradeOrder);

                // Order has been found.
                if (TradeOrder.InternalOrderID == r_BuyEntryInternalOrderID)
                {
                    bool IsOrderFilled = TradeOrder.OrderStatusCode == SCT_OSC_FILLED;
                    double FillPrice = TradeOrder.LastFillPrice;
                }
            }
    }

}
