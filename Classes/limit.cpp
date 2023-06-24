#include "order.cpp"

class Limit {
public:
    Order* head = nullptr;
    Order* tail = nullptr;
    int price = 0;
    int volume = 0;
    int numberOfOrders = 0;

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
        if ( head!=nullptr && head->id == order->id )
        {
            head = order->prev;
        }
        // did not use else if since head and tail can both be pointing to the same order
        if ( tail->id == order->id )
        {
            tail = order->next;
        }
        return LIMIT_SUCCESS;

    } 

    bool isEmpty () 
    {
        if ( head == nullptr && tail == nullptr ) 
        {
            return true;
        }
        return false;
    }
};


