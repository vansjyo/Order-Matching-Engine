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

    depth = 200;

    printTab ( { " ", " ", " ", "________ Sell ________ \n" } );
    printTab( { " ", " ", " ", "Price", "#Orders", "Volume" } );
    for ( int k = depth; k > 0 && bestSell != nullptr; k-- ) 
    {
        
        Limit* ptr = sellSide[bestSell->price + k];
        // printing the buy side of the Order Book
        if ( ptr!=nullptr && !ptr->isEmpty() )
        {
            printTab ( { " ", " ", " "}, 0 );
            cout << (ptr->price)/100 <<  "." << ( ( (ptr->price)%100 < 10 )? "0" : "" );
            printTab ( { (ptr->price)%100, ptr->numberOfOrders, ptr->volume } );
        }
    }
    
    
    cout << "\n_________ Buy _________ \n\n";
    printTab ( {"Price", "#Orders", "Volume"} );
    for ( int k = 0; k < depth && bestBuy!=nullptr; k++ ) 
    {

        Limit* ptr = buySide[bestBuy->price - k];
        // printing the Sell side of the Order Boo
        if( ptr!=nullptr && !ptr->isEmpty() )
        {
            cout << (ptr->price)/100 << "." << ( ( (ptr->price)%100 < 10 )? "0" : "" );
            printTab ( { (ptr->price)%100, ptr->numberOfOrders, ptr->volume } );
        }
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
            numberOfSellOrders -= 1;

            // if the limit object has gotten empty, set the index position to nullptr
            if ( limit->numberOfOrders == 0 ) sellSide[price] = nullptr;

             // if this was the only buy order in the book, set best buy to nullptr
            if ( numberOfSellOrders == 0 ) bestSell = nullptr;

            // if the cancelled order was from the best sell Limit object and this was the only order in the limit, move to next valid Sell order
            if ( bestSell != nullptr  &&  bestSell->price == order->limitPrice  &&  limit->numberOfOrders == 0 )
            {
                int k = price;
                while ( sellSide[k] == nullptr || sellSide[k]->numberOfOrders == 0 ) { k++; }
                bestSell = sellSide[k];
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
            numberOfBuyOrders -= 1;

            // if the limit has gotten empty, set the index position to nullptr
            if ( limit->numberOfOrders == 0 ) buySide[price] = nullptr;

            // if this was the only buy order in the book, set best buy to nullptr
            if ( numberOfBuyOrders == 0 ) bestBuy = nullptr;

            // if the cancelled order was from the best Buy Limit object and this was the only order in the limit, move to next valid buy order
            if ( bestBuy != nullptr  &&  bestBuy->price == order->limitPrice  &&  limit->numberOfOrders == 0 )
            {
                int k = price;
                while ( buySide[k] == nullptr || buySide[k]->numberOfOrders == 0 ) { k--; }
                bestBuy = buySide[k];
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
            numberOfSellOrders += 1;
            if ( limit->numberOfOrders == 1  && ( bestSell==nullptr || ( bestSell != nullptr && limit->price < bestSell->price ) ) )
            {
                
                bestSell = limit;

                if ( (bestSell != nullptr) && (bestBuy!=nullptr) && (bestSell->price <= bestBuy->price) ) 
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
            // update the order book for best sell limit if either there is no previously set best buy (so current order is best)
            // or if sell offer is lower than best and a new limit was added (only one order in the limit)
            numberOfBuyOrders += 1;
            if ( limit->numberOfOrders == 1  && ( bestBuy==nullptr || ( bestBuy != nullptr && limit->price > bestBuy->price ) ) )
            {
                bestBuy = limit;

                if ( (bestSell != nullptr) && (bestBuy!=nullptr) && (bestSell->price <= bestBuy->price) )
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

    // sets the bestBuy when the first order is added to the exchange
    if ( numberOfBuyOrders == 1 ) bestBuy = limit;
    return LIMIT_SUCCESS;
}


// algorithm for matching the ordes
// input:  nothing, void
// return: status_codes based on matching status
matching_engine_status_codes OrderBook :: runMatchingEngine ()
{
    cout << "running the matching engine...\n";
    // while there exists spread, keep matching orders
    while ( bestBuy != nullptr  &&
            bestSell != nullptr  &&  
            bestBuy->price >= bestSell->price )
    {
        Order* buyOrder = bestBuy->head;
        Order* sellOrder = bestSell->head;

        int fill = min( (buyOrder->size - buyOrder->filled), (sellOrder->size - sellOrder->filled) );
        cout << "Order Matched\n";

        // orders matched
        switch ( buyOrder->fillOrder ( fill ) )
        {
            case ORDER_SUCCESS:
                break;
            default:
                exit(0);
                break;  
        }

        switch ( sellOrder->fillOrder ( fill ) )
        {
            case ORDER_SUCCESS:
                break;
            default:
                exit(0);
                break;  
        }

        // if the entire buy order has been filled, update the best Sell
        // if there are more orders in the bestBuy Limit, then simply move the head of bestBuy to current_head->prev and reset the next pointer of new head to nullptr
        // if there are no more orders in the limit, loop till you get the next best Limit and reset the bestBuy to the new limit object
        switch ( buyOrder->status )
        {
            case EXECUTED:
                removeBuyOrderFromBook ( buyOrder );
                break;
            case PENDING:
                bestBuy->volume -= fill;
                break;
            default:
                break;
        }

        switch ( sellOrder->status )
        {
            case EXECUTED:
                removeSellOrderFromBook ( sellOrder );
                break;
            case PENDING:
                bestSell->volume -= fill;
                break;
            default:
                break;
        }

    }
    
    return ME_SUCESS;
}
