#include <iostream>
#include <string>

#include "Lexicon.h"

int main() {

   Lexicon *l = new Lexicon("Automaton.txt", "Palavras_Reservadas.txt", 97, 97);
   l->process_lexicon("inputFile.txt", "outPutHashFile.txt", "tokens.txt");
   delete l;

   return 0;
}

