// #include <fstream>
// #include <sstream>
#include "globalVar.h"
#include "./Classes/orderbook.cpp"
#include <bits/stdc++.h>

using namespace std;

int main() { 
    fstream orderFile ("order_format.csv", ios_base::in);
    ofstream logFile ("logs.txt", ios_base::app);

    OrderBook book;
    long int orderCounter = 0;

    unordered_map<char, types> getOrderType{ {'B', BUY}, {'S', SELL} }; 
    unordered_map<char, actions> getOrderAction{ {'A', ADD}, {'C', CANCEL} }; 

    unordered_map<long int, Order*> sellOrderMap;
    unordered_map<long int, Order*> buyOrderMap;

    if ( orderFile.is_open() ) 
    {
        string line;
        string row[4]; //4 is the message lenth (in words)

        while ( getline(orderFile, line) ) 
        {
            
            getArrayFromLine(line, row);
            // message format:
            // For Add orders: "SELL/BUY ADD PRICE SIZE"
            // For Cancel Orders: "SELL/BUY CANCEL ORDER_ID SIZE"

            types orderType = ( getOrderType.find(row[0][0]) == getOrderType.end() ) ? NONE_TYPE : getOrderType.at(row[0][0]);
            actions orderAction = ( getOrderAction.find(row[1][0]) == getOrderAction.end() ) ? NONE_ACTION : getOrderAction.at(row[1][0]);

            // check Order Type = BUY / SELL
            switch ( orderType )
            {
                case SELL:

                    // check action to be done = ADD / CANCEL
                    switch ( orderAction )
                    {
                        case CANCEL:
                            // fetch the price at which the order was placed, return error message if does not exist
                            // if exists, check if size matches with input, return error if not (partial cancels not supported)
                            // go to the position in the double linked list, remove the order and reset the linked list
                            // update the volume of the limit object
                            // look up the order in the sell order map, change status to cancelled, left and right pointers as nullptr
                            // log message cancel successfull and remove the order from the Order Map and save it in archive/disc
                            
                            long int orderId;
                            try
                            {
                                orderId = stoi(row[2]);
                            }
                            catch(const std::exception& e)
                            {
                                cerr << ": Order ID" << e.what();
                                logMessage ( "Order ID" + row[2] + " not an integer", logFile );
                                break;
                            }

                            int size;
                            try
                            {
                                size = stoi(row[3]);
                            }
                            catch(const std::exception& e)
                            {
                                cerr << ": Order Size" << e.what();
                                logMessage ( "Order size" + row[3] + " not an integer", logFile );
                                break;
                            }

                            Order* order;
                            try
                            {
                                order = sellOrderMap.at(orderId);
                            }
                            catch(const std::exception& e)
                            {
                                cerr <<  ": Order ID not found" << e.what();
                                logMessage ( "Order ID " + to_string(orderId) + "not found in the Exchange.", logFile );
                                break;
                            }
 
                            bool FLAG = 1; // denoted red flag is on
                            switch ( order->cancelOrder(size) )
                            {
                                case ORDER_ERR_SIZE:
                                    logMessage ( "Cannot place partial or larger cancel orders on Exchange", logFile );
                                    break;

                                case ORDER_ERR_STATE:
                                    logMessage ( "Order status cannot be changed.", logFile );
                                    break;
                                
                                default:
                                    FLAG = 0;
                                    logMessage ( "Order " + to_string(orderId) + " cancelled successfully.", logFile );
                                    break;
                            }

                            if ( FLAG ) break;

                            long int idx = sellOrderMap.at(orderId)->limitPrice;
                            Limit limit = book.sellSide[idx];

                            FLAG = 1;
                            switch ( limit.removeOrderFromQueue(order) ) 
                            {
                                case LIMIT_ERR:
                                    logMessage ( "Could not remove order from Queue. Some error occured", logFile );
                                    break;

                                case LIMIT_SUCCESS:
                                    FLAG = 0;
                                    logMessage ( "Order " + to_string(orderId) + " removed from the Queue successfully.", logFile );
                                    break;
                            }
                            if ( FLAG ) break; //currently seems like not needed since break anyways, but maybe later would be useful

                            break;
                        
                            
                        
                        case ADD:
                            orderCounter += 1;
                            int price = stoi(row[2]);
                            int size = stoi(row[3]);

                        default:
                            throw std::invalid_argument("Invalid Order Action: . Only Add or Cancel Orders permitted.");
                            break;

                    }
                    Order sellOrder;
                    sellOrder.id = orderCounter;
                    // book.sellSide[price]


                    // logMessage("Sell order placed for" + to_string(size) + " assets at $" + to_string(price), logFile);



                case BUY:

            }
            
        }
    }

    orderFile.close();
    logFile.close();

}