#include "../globalVar.h"
#include <chrono>

class Order {
public:
    int id;
    int limitPrice;
    int size;
    std::chrono::system_clock::time_point placedDate;
    std::chrono::system_clock::time_point actionDate;
    
    types type;
    states status;

    Order* next;
    Order* prev;

    order_err_codes cancelOrder(int volume) {

        switch ( status )
        {
            case PENDING:
                break;

            default:
                return ERR_STATE;
                
        }

        int diff = size - volume;
        switch ( diff ) 
        {
            case 0:
                break;
            
            default:
                return ERR_SIZE;

        }

        status = CANCELLED;
        actionDate = std::chrono::system_clock::now();
        if ( prev != nullptr ) prev->next = next;
        if ( next != nullptr ) next->prev = prev;
        next = nullptr;
        prev = nullptr;
        return SUCCESS;
        
    }
    
};