#include <iostream>
#include <string>
#include "Item.h"

Item::Item(string var_type, string var_name, string var_value){
    this->var_type = var_type;
    this->var_name = var_name;
    this->var_value = var_value;
}

void Item::show(){
    cout<< this->var_type << " " << this->var_name << " = " << this->var_value << "\n";
}