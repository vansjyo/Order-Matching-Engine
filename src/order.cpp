#include "../headers/order.h"


// class constructor
Order :: Order ( int counter, int p, int s, types t ) : 
        id { counter }, 
        limitPrice { p }, 
        size { s }, 
        type { t },
        placedDate { std::chrono::system_clock::now() } 
{
    // usually if the variable is just an int, initialization does not much have much efficiency over assignment (specially since c++17)
    // however, since Order constructor is going to be called too often, we want to adopt an optimized approach of initialization over assignment
    // just trying to learn more of C++ :)
    
}


// change the order attributes to indicate a cancelled orde object
order_err_codes Order :: cancelOrder ( int volume ) {

    // check if the order status was initially pending
    switch ( status )
    {
        case PENDING:
            break;

        default:
            return ORDER_ERR_STATE;
            
    }

    // check if the order volume to be cancelled is the same as the size of the original size of placed order (no partial cancels currently)
    int diff = size - volume;
    switch ( diff ) 
    {
        case 0:
            break;
        
        default:
            return ORDER_ERR_SIZE;

    }

    status = CANCELLED;
    actionDate = std::chrono::system_clock::now();

    // removes the order from the doubly linkedin list (order queue)
    if ( prev != nullptr ) prev->next = next;
    if ( next != nullptr ) next->prev = prev;
    // this step must not be done here since we will require these variables while executing-> removeOrderFromQueue ( Order* order )
    // they will be needed for resetting the head/tail of the limit object to which the order belonged (if the order was at head/tail)
    // next = nullptr;
    // prev = nullptr;
    return ORDER_SUCCESS;
    
}
