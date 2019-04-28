#include <iostream>
#include <string>

#include "Lexicon.h"

using namespace std;

int main() {

   string input, output, tokens;
   Lexicon *l = new Lexicon("./configuracoes/Automaton.txt", "./configuracoes/Palavras_Reservadas.txt", 97, 97);

   cout << "Digite o nome do arquivo de entrada: ";
   cin >> input;

   cout << "Digite o nome do arquivo de saida para a listagem dos identificadores: ";
   cin >> output;

   cout << "Digite o nome do arquivo de saida dos tokens: ";
   cin >> tokens;
   
   l->process_lexicon("./testes/" + input, "./saida/" + output, "./saida/" + tokens);

   delete l;

   return 0;
}

