#include "limit.cpp"
#include "helper.h"
#include <iostream>

using namespace std;

class OrderBook {
public:
    Limit buySide[100000];
    Limit sellSide[100000];
    int numberOfSellOrders = 0;
    int numberOfBuyOrders = 0;
    Limit* bestSell;
    Limit* bestBuy;

    void printBook (int depth) 
    {
        if( depth > 50 ) {
            cout << "Error: Cannot request depth more than 50\n";
            return;
        }

        int k = 0;
        Limit* headBuy = bestBuy;
        Limit* headSell = bestSell;
        while( k < depth && k < numberOfBuyOrders ) 
        {

            // printing the buy side of the Order Book
            cout << "Buy:\n";
            printTab( {"Price", "#Orders", "Volume"} );
            if( buySide[headBuy->price].price != 0 )
            {
                printTab( {headBuy->price, headBuy->numberOfOrders, headBuy->volume} );
                k++;
            }
            headBuy--;
        }
        
        k = 0;
        while( k < depth && k < numberOfSellOrders )
        {

            // printing the Sell side of the Order Book
            cout << "Sell:\n";
            printTab( {"Price", "#Orders", "Volume"} );
            if( buySide[headSell->price].price != 0 )
            {
                printTab( {headSell->price, headSell->numberOfOrders, headSell->volume} );
                k++;
            }
            headSell++;
        }

    }

    void removeOrderFromQueue (int idx) 
    {
        
    }
};