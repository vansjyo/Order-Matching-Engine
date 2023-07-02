#include <bits/stdc++.h>

using namespace std;

int main() 
{
    int numberOfSamples = 1000;
    std::default_random_engine generator;
    std::uniform_real_distribution<double> buy_sell(0.0,1.0);
    std::uniform_real_distribution<double> add_cancel(0.0,1.0);
    std::uniform_real_distribution<double> price(7799,7999);
    std::uniform_real_distribution<double> size(1, 500);
    std::uniform_real_distribution<double> depth(1, 70);

    ofstream sampleFile;
    sampleFile.open ( "samples.csv", ios::app|ios::out );

    vector<vector<string>> orders;
    while ( numberOfSamples )
    {
        vector<string> s;

        int bs = round(buy_sell(generator));
        if(bs)
        {
            s.push_back("BUY");
        }
        else 
        {
            s.push_back("SELL");
        }

        s.push_back("ADD");
        s.push_back(to_string( (int) round( price (generator) ) ));
        s.push_back(to_string( (int) round( size (generator) ) ));
        
        orders.push_back(s);

        string line = s[0] + " " + s[1] + " " + s[2] + " " + s[3];

        sampleFile << line << endl;

        if(numberOfSamples%7==0)
        {
            std::uniform_real_distribution<double> cancel(0, orders.size()-1);
            int idx = round( cancel (generator) );
            string l = orders[idx][0] + " CANCEL " + to_string(idx+1) + " " + orders[idx][3];  
            sampleFile << l << endl;
        }
        

        // if(numberOfSamples%10==0)
        // {
            // sampleFile << "printorderbook " << round( depth(generator) ) << endl;
        // }
        numberOfSamples--;
    }
    return 0;
}