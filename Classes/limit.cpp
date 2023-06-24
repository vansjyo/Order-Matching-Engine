#include "order.cpp"

class Limit {
public:
    Order* head = nullptr;
    Order* tail = nullptr;
    int price = 0;
    int volume = 0;
    int numberOfOrders = 0;

    // LimitTree* left;
    // LimitTree* right;
    // LimitTree* parent;
};


