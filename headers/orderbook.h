#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include "limit.h"
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

    Limit* bestSell {nullptr};
    Limit* bestBuy {nullptr};

    // prints the order book of given depth (# of levels)
    // input:  int depth
    // output: print, void return
    void printBook ( int depth );

    limit_err_codes removeSellOrderFromBook ( Order* order );

    limit_err_codes removeBuyOrderFromBook ( Order* order );

    limit_err_codes addSellOrderToBook ( Order* order );

    limit_err_codes addBuyOrderToBook ( Order* order );

    matching_engine_status_codes runMatchingEngine ();

};

#endif