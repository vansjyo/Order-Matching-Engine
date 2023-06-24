#include <bits/stdc++.h>

using namespace std;

int main() {
    string s = "12";
    string t = "r4";
    char buffer[sizeof "9999-12-31 23:59:59.999:999"];
    unordered_map<int,int> m;

    try
    {
        int a = m.at(8);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << "Count find in map\n";
        // return 0;
    }
    

    cout << sizeof(buffer)/sizeof(buffer[0]) << "\n";
    if(2>1){
        throw invalid_argument("kill");
    }

    try
    {
        int a = stoi(s);
        cout << "I am\n";
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << "count convert s\n";
    }

    try
    {
        int a = stoi(t);
        cout << "I was\n";
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << ":count convert t\n";
        cout << "Heree\n";
    }

    cout << "Hi\n";
    
    

    return 0;
}