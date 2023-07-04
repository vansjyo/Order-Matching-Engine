#ifndef ORDER_H
#define ORDER_H

#include "globalVar.h"
#include <bits/stdc++.h>
#include <chrono>

class Order {
public:

    // -> class variables 
    
    int id;
    int limitPrice;
    int size; // what was the original size of the order placed
    int filled { 0 }; // how much of the order has currently been filled

    types type;
    states status { PENDING };

    std::chrono::system_clock::time_point placedDate; // time at which the order was added to the queue
    std::chrono::system_clock::time_point actionDate; // time at which the order was removed (cancelled or executed) from the queue

    Order* next { nullptr };
    Order* prev { nullptr };

    // -> class methods

    Order ( int counter, int p, int s, types t );
    
    order_err_codes cancelOrder ( int volume );

    order_err_codes fillOrder ( int fill );
};

#endif