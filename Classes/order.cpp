#include "../globalVar.h"
#include <chrono>

class Order {
public:

    int id;
    int limitPrice;
    int size;

    std::chrono::system_clock::time_point placedDate; // time at which the order was added to the queue
    std::chrono::system_clock::time_point actionDate; // time at which the order was removed from the queue
    
    types type;
    states status { PENDING };

    Order* next { nullptr };
    Order* prev { nullptr };

    Order ( int counter, int p, int s, types t ) : id { counter }, limitPrice { p }, size { s }, type { t } {
        // usually if the variable is just an int, initialization does not much have much efficiency over assignment (specially since c++17)
        // however, since Order constrctor is going to be called too often, we want to adopt an optimized approach
        placedDate = std::chrono::system_clock::now();
    }

    order_err_codes cancelOrder(int volume) {

        switch ( status )
        {
            case PENDING:
                break;

            default:
                return ORDER_ERR_STATE;
                
        }

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
        if ( prev != nullptr ) prev->next = next;
        if ( next != nullptr ) next->prev = prev;
        next = nullptr;
        prev = nullptr;
        return ORDER_SUCCESS;
        
    }
    
};