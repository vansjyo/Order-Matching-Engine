#ifndef LIMIT_H
#define LIMIT_H

#include "order.h"

using namespace std;

class Limit {
public:

    // class variables
    Order* head { nullptr };
    Order* tail { nullptr };

    int price;
    int volume { 0 };
    int numberOfOrders { 0 };

    // class methods
    Limit ( int limitPrice );

    bool isEmpty ();

    limit_err_codes removeOrderFromQueue ( const Order* order );

    limit_err_codes addOrderToQueue ( Order* order );

};

#endif