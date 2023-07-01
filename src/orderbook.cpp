#include "../headers/limit.h"
#include "../headers/orderbook.h"
#include "../headers/helper.h"
#include <iostream>
#include <initializer_list>

using namespace std;

void OrderBook :: printBook ( int depth ) 
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
        printTab ( { " ", " ", " ", "________ Buy ________ \n" });
        printTab( { " ", " ", " ", "Price", "#Orders", "Volume" } );
        if( buySide[headBuy->price]!=nullptr && !buySide[headBuy->price]->isEmpty() )
        {
            printTab ( { " ", " ", " "}, 0 );
            printTab ( { headBuy->price, headBuy->numberOfOrders, headBuy->volume} );
        }
        k++;
        --headBuy;
    }
    
    k = 0;
    while ( k < depth && k < numberOfSellOrders )
    {

        // printing the Sell side of the Order Book
        cout << "\n_________ Sell _________ \n\n";
        printTab ( {"Price", "#Orders", "Volume"} );
        if( !sellSide[headSell->price]->isEmpty() )
        {
            printTab ( {headSell->price, headSell->numberOfOrders, headSell->volume} );
        }
        k++;
        ++headSell;
    }

}


// removes the order from the queue and orderbook (note: not using switch to get order type as that would also need same amount of copied code, no effect on efficiency)
// input:  Order* order
// output: Enum indicating sucess of execution
limit_err_codes OrderBook :: removeSellOrderFromBook ( Order* order ) 
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
        
        default:
            cout << "I am not sure what happened\n";
            break;
    }

    return LIMIT_SUCCESS;

}



// removes the order from the queue and orderbook (note: not using switch to get order type as that would also need same amount of copied code, no effect on efficiency)
// input:  Order* order
// output: Enum indicating sucess of execution
limit_err_codes OrderBook :: removeBuyOrderFromBook ( Order* order ) 
{
    // the order has already been voided i.e. status changed to cancelled and next/prev set to nullptr 
    // but we need to remove it from the queue now and reset the best sell if applicable

    int price = order->limitPrice;

    Limit* limit = buySide[price];

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
            numberOfBuyOrders -= 1;

            if ( limit->numberOfOrders == 0 ) buySide[price] = nullptr;

            if ( bestBuy->price == order->limitPrice )
            {
                bestBuy = limit;

                // if the cancelled order was from the best buy Limit object and this was the only order in the limit, move to next valid Buy order
                if ( limit->numberOfOrders == 0 )
                {
                    int k = price;
                    while ( buySide[k] == nullptr || buySide[k]->numberOfOrders == 0 ) { k--; }
                    bestBuy = buySide[k];
                }
            }
            return LIMIT_SUCCESS;

            break;

        default:
            cout << " I am not sure what happened\n";
    }

    return LIMIT_SUCCESS;

}



// handles adding the order and updating everything
limit_err_codes OrderBook :: addSellOrderToBook ( Order* order ) 
{
    // check if the array position at idx is nullptr, if yes, create a new Limit
    int price = order->limitPrice;
    if ( sellSide[price] == nullptr ) sellSide[price] = new Limit( price );

    Limit* limit = sellSide[price];

    // call the AddOrderToQueue function
    switch ( limit->addOrderToQueue ( order ) )
    {
        case LIMIT_SUCCESS:
            // update the order book for best sell limit if sell offer is lower than best and a new limit was added (only one order in the limit)
            if ( limit->numberOfOrders == 1  && bestSell != nullptr && limit->price < bestSell->price )
            {
                bestSell = limit;

                if ( bestSell->price <= bestBuy->price ) 
                {
                    switch ( runMatchingEngine() )
                    {
                        case ME_SUCESS:
                            cout << "Matching engine ran successfully\n";
                            break;

                        default: 
                            cout << "Some problem occured with the matching engine\n"; 
                            break;
                    }
                }  
            }
        break;

        default:
            cerr << "Could not add order to the queue for some reason\n";
        break;
    }

    numberOfSellOrders += 1;
    // sets the bestSell when the first order is added to the exchange
    if ( numberOfSellOrders == 1 ) bestSell = limit;
    return LIMIT_SUCCESS;
}


// handles adding the order and updating everything
limit_err_codes OrderBook :: addBuyOrderToBook ( Order* order ) 
{
    // check if the array position at idx is nullptr, if yes, create a new Limit
    int price = order->limitPrice;
    if ( buySide[price] == nullptr ) buySide[price] = new Limit( price );

    Limit* limit = buySide[price];

    // call the AddOrderToQueue function
    switch ( limit->addOrderToQueue ( order ) )
    {
        case LIMIT_SUCCESS:
            // update the order book for best sell limit if sell offer is lower than best and a new limit was added (only one order in the limit)
            if ( limit->numberOfOrders == 1  &&  bestBuy != nullptr && limit->price > bestBuy->price )
            {
                bestBuy = limit;

                if ( bestSell->price <= bestBuy->price ) 
                {
                    switch ( runMatchingEngine() )
                    {
                        case ME_SUCESS:
                            cout << "Matching engine ran successfully\n";
                            break;

                        default: 
                            cout << "Some problem occured with the matching engine\n"; 
                            break;
                    }
                }  
            }
        break;

        default:
            cout << "Could not add order to the queue for some reason\n";
        break;
    }

    numberOfBuyOrders += 1;
    // sets the bestBuy when the first order is added to the exchange
    if ( numberOfBuyOrders == 1 ) bestBuy = limit;
    return LIMIT_SUCCESS;
}


// algorithm for matching the ordes
// input:  nothing, void
// return: status_codes based on matching status
matching_engine_status_codes OrderBook :: runMatchingEngine ()
{
    // while there exists spread, keep matching orders
    while ( bestBuy->price >= bestSell->price )
    {
        Order* buyOrder = bestBuy->head;
        Order* sellOrder = bestSell->head;

        int fill = min( buyOrder->size, sellOrder->size );

        // orders matched
        buyOrder->filled = fill;
        sellOrder->filled = fill;

        // if the entire buy order has been filled, update the best Sell
        // if there are more orders in the bestBuy Limit, then simply move the head of bestBuy to current_head->prev and reset the next pointer of new head to nullptr
        // if there are no more orders in the limit, loop till you get the next best Limit and reset the bestBuy to the new limit object
        if ( buyOrder->size == buyOrder->filled )
        {
            buyOrder->status = EXECUTED;
            buyOrder->actionDate = std::chrono::system_clock::now();
            bestBuy->numberOfOrders -= 1;

            if ( bestBuy->numberOfOrders >= 1 )
            {
                bestBuy->head = bestBuy->head->prev;
                bestBuy->head->next = nullptr;
                
            }
            else
            {
                int k = bestBuy->price;
                while ( buySide[k] == nullptr || buySide[k]->numberOfOrders == 0 ) { k--; }
                bestBuy = buySide[k];
            }
        }

        // do the same thing for the sell side
        if ( sellOrder->size == sellOrder->filled )
        {
            sellOrder->status = EXECUTED;
            if ( bestSell->numberOfOrders > 1 )
            {
                bestSell->head = bestSell->head->prev;
                bestSell->head->next = nullptr;
            }
            else
            {
                int k = bestSell->price;
                while ( sellSide[k] == nullptr || sellSide[k]->numberOfOrders == 0 ) { k--; }
                bestSell = sellSide[k];
            }
        }
    }
    
    return ME_SUCESS;
}
