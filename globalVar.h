#ifndef MY_GLOBALS_H
#define MY_GLOBALS_H

using namespace std;

enum types {BUY, SELL, NONE_TYPE};
enum states {PENDING, DONE, CANCELLED, NONE_STATE};
enum actions {ADD, CANCEL, NONE_ACTION};
enum order_err_codes {ORDER_ERR_SIZE, ORDER_ERR_STATE, ORDER_SUCCESS};
enum limit_err_codes {LIMIT_ERR, LIMIT_SUCCESS};

// extern types;
// extern states;
// extern actions;

#endif