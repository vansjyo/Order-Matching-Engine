#include "../headers/globalVar.h"
#include "../headers/helper.h"
// #include <chrono>

using namespace std;

// input:   string line, delmiter set as ' ', array<string>[4] row 
// returns: vector<string row> (in place)
void getArrayFromLine ( string line, string (&row)[4] ) 
{
    // convert line into a string stream object separated by delimiter
    string word;
    stringstream str(line);

    int k = 0;
    while ( getline(str, word, ' ') )
    {
        row[k++] = word;
    }
}

// input:   none
// returns: printed current Time steamp till microseconds
void printTime (ofstream &logFile) 
{
    using namespace std::chrono;
    auto timepoint = high_resolution_clock::now();
    auto coarse = system_clock::to_time_t(timepoint);
    auto fine = time_point_cast<std::chrono::milliseconds>(timepoint);
    auto finest = time_point_cast<std::chrono::microseconds>(timepoint);

    char buffer[sizeof "9999-12-31 23:59:59.999:999"];
    std::snprintf(buffer + std::strftime(buffer, sizeof buffer - 3, 
                    "%F %T.", std::localtime(&coarse)),
                    8, "%03lu:%03lu", fine.time_since_epoch().count() % 1000, finest.time_since_epoch().count()%1000);

    cout << buffer << " -> ";
    logFile << buffer << " -> ";
}


// input:   string messgae to be printed in the log fle, log file oftsream handler
// returns: prints Log message in the log file
void logMessage ( string msg, ofstream &logFile )
{
    printTime(logFile);
    cout << msg << endl;
    logFile <<  msg << "\n";
}


// input:   string to convert to int, string message to be printed in the log fle if conversion fails, log file oftsream handler
// returns: prints Log message in the log file
// bool checkStringToInt ( string str, string errMsg )
// {
//     try
//     {
//         long int orderId = stoi(str);
//         return true;
//     }
//     catch(const std::exception& e)
//     {
//         std::cerr << e.what() <<  "Could not convert to Int:" + errMsg;
//         return false;
//     }
// }