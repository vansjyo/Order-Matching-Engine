#include "order.cpp"

using namespace std;

class Limit {
public:

    Order* head { nullptr };
    Order* tail { nullptr };

    int price;
    int volume { 0 };
    int numberOfOrders { 0 };


    // constructor sets only price as that is the characteristic of a limit object
    Limit ( int limitPrice ) : price { limitPrice } {}


    // checks if there is an order in the queue or not
    bool isEmpty () 
    {
        if ( head == nullptr && tail == nullptr ) return true;
        return false;
    }

    // removes the order from the queue
    limit_err_codes removeOrderFromQueue ( const Order* order ) 
    {
        numberOfOrders -= 1;
        volume -= order->size;

        if ( head == nullptr || tail == nullptr ) //safety check for debugging the program
        {
            cout << "How is the head/tail null?\n";
            exit(0); //change the code to the commnented part when the testing is over
            // return LIMIT_ERR;
        }
        
        // this also handles the case when order has a null prev/next pointer
        if ( head!=nullptr && head->id == order->id ) head = order->prev;

        // did not use else if since head and tail can both be pointing to the same order
        if ( tail->id == order->id ) tail = order->next;

        return LIMIT_SUCCESS;
    } 

    // adds an order to the queue
    limit_err_codes addOrderToQueue ( Order* order ) 
    {
        volume += order->size;
        numberOfOrders += 1;

        if ( isEmpty() ) 
        {
            head = order;
            tail = order;
            return LIMIT_SUCCESS;
        }

        // FIFO model - add the order at the tail/end of the queue and update the tail
        tail->prev = order;
        order->next = tail;
        tail = order;
        return LIMIT_SUCCESS;
    }


};


