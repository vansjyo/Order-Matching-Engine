#include "limit.cpp"
#include "helper.h"
#include <iostream>

using namespace std;

class OrderBook {
public:
    // nullptr can be initialzed to pointers on default using this syntax, use `.fill` for other default values 
    Limit* buySide[100000] { nullptr }; 
    Limit* sellSide[100000] { nullptr };

    int numberOfSellOrders = 0;
    int numberOfBuyOrders = 0;

    Limit* bestSell;
    Limit* bestBuy;

    void printBook ( int depth ) 
    {
        if( depth > 50 ) 
        {
            cout << "Error: Cannot request depth more than 50\n";
            return;
        }

        int k = 0;
        Limit* headBuy = bestBuy;
        Limit* headSell = bestSell;
        while ( k < depth  &&  k < numberOfBuyOrders ) 
        {

            // printing the buy side of the Order Book
            cout << "Buy:\n";
            printTab( {"Price", "#Orders", "Volume"} );
            if( buySide[headBuy->price]->price != 0 )
            {
                printTab( {headBuy->price, headBuy->numberOfOrders, headBuy->volume} );
                k++;
            }
            headBuy--;
        }
        
        k = 0;
        while ( k < depth && k < numberOfSellOrders )
        {

            // printing the Sell side of the Order Book
            cout << "Sell:\n";
            printTab ( {"Price", "#Orders", "Volume"} );
            if( buySide[headSell->price]->price != 0 )
            {
                printTab ( {headSell->price, headSell->numberOfOrders, headSell->volume} );
                k++;
            }
            headSell++;
        }

    }


    // removes the order from the queue and orderbook
    // input:  Order* order
    // output: Enum indicating sucess of execution
    limit_err_codes removeSellOrderFromBook ( Order* order ) 
    {
        // the order has already been voided i.e. status changed to cancelled and next/prev set to nullptr 
        // but we need to remove it from the queue now and reset the best sell if applicable

        int price = order->limitPrice;

        Limit* limit = sellSide[price];

        // call the RemoveOrderFromQueue function
        switch ( limit->removeOrderFromQueue ( order ) ) 
        {
            case LIMIT_ERR:
                return LIMIT_ERR;
                break;

            case LIMIT_SUCCESS:
                order->actionDate = std::chrono::system_clock::now();
                order->prev = nullptr;
                order->next = nullptr;
                numberOfSellOrders -= 1;

                if ( limit->numberOfOrders == 0 ) sellSide[price] = nullptr;

                if ( bestSell->price == order->limitPrice )
                {
                    bestSell = limit;

                    // if the cancelled order was from the best sell Limit object and this was the only order in the limit, move to next valid Sell order
                    if ( limit->numberOfOrders == 0 )
                    {
                        int k = price;
                        while ( sellSide[k] == nullptr || sellSide[k]->numberOfOrders == 0 ) { k++; }
                        bestSell = sellSide[k];
                    }
                }
                return LIMIT_SUCCESS;

                break;
        }

    }

    

    // handles adding the order and updating everything
    limit_err_codes addSellOrderToBook ( Order* order ) 
    {
        // check if the array position at idx is nullptr
        int price = order->limitPrice;
        if ( sellSide[price] == nullptr ) sellSide[price] = new Limit( price );

        Limit* limit = sellSide[price];

        // call the AddOrderToQueue function
        switch ( limit->addOrderToQueue ( order ) )
        {
            case LIMIT_SUCCESS:
                // update the order book for best sell limit if sell offer is lower than best and a new limit was added (only one order in the limit)
                if ( limit->numberOfOrders == 1  &&  limit->price < bestSell->price )
                {
                    bestSell = limit;

                    if ( bestSell->price <= bestBuy->price ) 
                    {
                        // run the matching engine
                    }  
                }
                break;

            default:
                cerr << "Could not add order to the queue for some reason\n";
                break;
        }

        numberOfSellOrders += 1;
    }

    // void removeOrderFromQueue ( types ordertype, int idx ) 
    // {
    //     switch ( ordertype )
    //     {
    //         case BUY:
    //             buySide[idx].numberOfOrders -= 1;
    //             sellSide[idx].volume -= size;
    //             break;
    //         case SELL:
    //             break;
    //         default:
    //             break;
    //     }
        
    // } 

};