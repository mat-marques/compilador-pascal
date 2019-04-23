#include <iostream>
#include <string>
#include "Item.h"

Item::Item(string v_type, string name, string value){
    this->var_type = v_type;
    this->var_name = name;
    this->var_value = value;
}

void Item::show(){
    cout<< this->var_type << " " << this->var_name << " = " << this->var_value << "\n";
}