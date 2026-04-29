#ifndef AUTH_H
#include <Arduino.h>
#include <string.h>
// #include <primary_auth.h>
struct auth_state
{
private:
    const char token[70] = "token_sck"; 
public:
    bool check_auth(String auth_h = "xxx");
};

#endif