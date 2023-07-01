#ifndef HELPER_H
#define HELPER_H

#include <bits/stdc++.h>

// #include "helper.cpp"
using namespace std;

const char separator    = ' ';
const int nameWidth     = 11;

// input:   list/vector of things to print eg. {2, "hi", 8}
// returns: prints all elements of list seperated by tab 
template <class T>
void printTab ( const initializer_list<T> list, int i = 1 ) 
{
    for( auto elem : list )
    {
        cout << left << setw(nameWidth) << setfill(separator) << elem;
    }
    if (i) cout << endl;
   
}

void getArrayFromLine ( string line, string (&row)[4] );

void printTime ( ofstream& logFile );

void logMessage ( string msg, ofstream &logFile );

bool checkStringToInt ( string str, string errMsg );

#endif