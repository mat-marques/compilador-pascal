#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "Item.h"

using namespace std;

struct Node
{
  Item *data;
  struct Node *next;
};	


class List {
    private:
      struct Node *head, *tail;
      int size;
      
    public:
      List();
      bool addStart(Item *data);
      bool addFinal(Item *data);
      Item * remove(Item data);
      Item * search(Item data);
      void showItens(ofstream & hFile, string nameList);
      int getSize();
      void removeList();
};

#endif