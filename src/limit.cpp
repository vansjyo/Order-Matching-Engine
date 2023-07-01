#include "../headers/limit.h"

using namespace std;

// constructor sets only price as that is the characteristic of a limit object
Limit :: Limit ( int limitPrice ) : price { limitPrice } {}


// checks if there is an order in the queue or not
bool Limit :: isEmpty () 
{
    if ( head == nullptr && tail == nullptr ) return true;
    return false;
}


// removes the order from the queue
// input:  const Order* order (automatically casts non-constant to const)
// output: Enum indicating sucess of execution
limit_err_codes Limit :: removeOrderFromQueue ( const Order* order ) 
{

    if ( head == nullptr || tail == nullptr ) //safety check for debugging the program
    {
        cout << "How is the head/tail null?\n";
        exit(0); //change the code to the commnented part when the testing is over
        // return LIMIT_ERR;
    }

    numberOfOrders -= 1;
    volume -= ( order->size );
    
    // this also handles the case when order has a null prev/next pointer
    if ( head != nullptr  &&  head->id == order->id ) head = order->prev;

    // did not use else if since head and tail can both be pointing to the same order
    if ( tail != nullptr  &&  tail->id == order->id ) tail = order->next;

    return LIMIT_SUCCESS;
} 


// adds an order to the queue
// input:  Order* order
// output: Enum indicating sucess of execution
limit_err_codes Limit :: addOrderToQueue ( Order* order ) 
{
    // General: cannot pass a pointer to constant for this function as then you cannot even assign that const <T>* pointer to head/tail, let alone change 
    // casting it to non-constant reference using const_cast<T*>() is not recommended: https://blog.aaronballman.com/2011/06/when-should-you-use-const_cast/
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


