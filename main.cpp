#include <iostream>
#include <string>

#include "Lexicon.h"

int main() {
   printf("Inicio da compilação!\n");

   Lexicon *l = new Lexicon("Automaton.txt", "Palavras_Reservadas.txt", 20, 20);
   l->process_lexicon("inputFile.txt", "outPutHashFile.txt");
   delete l;

   printf("Fim da compilação!\n");
   return 0;
}

