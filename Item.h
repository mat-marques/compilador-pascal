#ifndef ITEM_H
#define ITEM_H

#include <iostream>
#include <string>

using namespace std;

class Item {
  public:
    string var_type;
    string var_name;
    string var_value;
    Item(string var_type, string var_name, string var_value);
    void show();
};

#endif