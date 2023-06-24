#ifndef HELPER_H
#define HELPER_H

#include "helper.cpp"

template <class T>
void printTab(initializer_list<T> list);

void getArrayFromLine(string line, string (&row)[4]);

void printTime();

void logMessage (string msg, ofstream &logFile);

bool checkStringToInt(string str, string errMsg);

#endif