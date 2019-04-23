#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>


using namespace std;

#include "Lexicon.h"

/*
	Construtor da classe.
*/
Lexicon::Lexicon(string automatonFileName, string reservedWordsFileName, int idHashSize, int rwHashSize){
	this->hashReservedWords = new HashTable(rwHashSize);
	this->hashIdentifiers = new HashTable(idHashSize);
	this->config_reserved_words(reservedWordsFileName);
	this->config_automaton(automatonFileName);
}

/*
	Faz o slip de uma string atraves do delimitador spliter.
*/
vector<string> *Lexicon::splitString(string input, char spliter) {
	vector<string> *l = new vector<string>();
	int start = 0, size = 0;
	for(int i = 0; i < input.length(); i++){
		if(input[i] == spliter){
			size = start - i;
			if(size < 0) 
				size = size * (-1);
			if(size > 0) 
				l->push_back(input.substr(start, size));
			start = i + 1;
		}
	}
	size = start - input.length();
	if(size < 0) 
		size = size * (-1);
	if(size > 0) 
		l->push_back(input.substr(start, size));

	return l;
}

/*
	Faz o processo de configurar a hash com as palavras reservadas.
*/
void Lexicon::config_reserved_words(string reservedWordsFileName){
	ifstream wordsFile;
	string word;
	wordsFile.open (reservedWordsFileName, ios_base::in);

	if(!wordsFile.is_open( ))
	{
		cout<<"Não foi possível abrir arquivo! Programa será terminado!\n";
		wordsFile.clear( ); //reseta o objeto leitura, para limpar memória do sistema
	}

	//Configuração da matrix e definição das variáveis de inicio
	while(!wordsFile.eof())
	{
		getline(wordsFile, word);
		this->hashReservedWords->insertItem(Item("", word, ""));
	}

	wordsFile.close();
}

/*
	Faz o processo de configurar o léxico.
*/
void Lexicon::config_automaton(string automatonFileName){
	ifstream automatonFile;
	string myString, s1, s2, s3;
	char op;
	vector<string> *l;
	automatonFile.open (automatonFileName, ios_base::in);

	if(!automatonFile.is_open( ))
	{
		cout<<"Não foi possível abrir arquivo! Programa será terminado!\n";
		automatonFile.clear( ); //reseta o objeto leitura, para limpar memória do sistema
	}

	//Configuração da matrix e definição das variáveis de inicio
	while(!automatonFile.eof())
	{
		getline(automatonFile, myString);
		//if para desconsiderar os comentarios no arquivo
		if(!std::regex_match (myString, std::regex("//(.*)")))
		{
			op = myString[0];
			switch(op)
			{
				case 'Q': //Q = Quantidade de estados
					s1 = myString.substr(2, myString.size());
					this->qtdStates = std::stoi(s1);
					matrix = this->getMatrix();
					break;
				case 'T': //T = Transicoes
					l = this->splitString(myString, ' ');
					this->configureMatrix(std::stoi((*l)[1]), std::stoi((*l)[2]), (*l)[3]);
					delete l;
					break;
				case 'I': //I = Estado Inicial
					s2 = myString.substr(2, myString.size());
					this->startState = std::stoi(s2);
					break;
				case 'F': //F = Estados Finais
					l = this->splitString(myString, ' ');
					for(int i = 1; i<l->size(); i++)
					{
						this->finalStates.push_back(std::stoi((*l)[i]));
					}
					delete l;
					break;
				default:
					cout<<"ERRO NAS CONFIGURACOES DO AUTOMATO!!"<<"\n";
					break;
			}
			//cout<<myString<<"\n";
		}
	}
	// cout<<"Q = "<<this->qtdStates<<"\n";
	// cout<<"I = "<<this->startState<<"\n";
	// cout<<"F = ";
	// for(int i = 0; i<this->finalStates.size(); i++)
	// {
	// 	cout<<this->finalStates[i]<<" ";
	// }
	// cout<<"\n";
	automatonFile.close();
}

/*
	Faz o processo de análise das cadeias de entrada.
*/
void Lexicon::process_lexicon(string inputFileName, string outPutHashFile){
	int line = 0;
	ifstream inputFile;
	string myString;
	inputFile.open (inputFileName, ios_base::in); //Arquivo de entrada

	if(!inputFile.is_open())
	{
		cout<<"Não foi possível abrir arquivo! Programa será terminado!\n";
		inputFile.clear(); //reseta o objeto leitura, para limpar memória do sistema
	}

	while(!inputFile.eof())
	{
		getline(inputFile, myString);
		cout << myString << "\n";
		this->checkString(myString, line);
		line++;
	}

	this->hashIdentifiers->show(outPutHashFile);

	this->deleteMatrix();
	this->hashIdentifiers->removeHashTable();
	this->hashReservedWords->removeHashTable();
	inputFile.close();
}

/*
	Cria a matrix de transições
*/
int ** Lexicon::getMatrix(){
	int **new_matrix = NULL;
	if (this->qtdStates > 0)
	{
		new_matrix = new int*[this->qtdStates];
		new_matrix[0] = new int[128];
		for (int i = 1; i < this->qtdStates; ++i)
		{
			new_matrix[i] = new int[128];
		}

		for(int i = 0; i < this->qtdStates; ++i)
		{
			for(int j = 0; j < 128; ++j)
			{
				new_matrix[i][j] = 0;
			}
		}
	}
	return new_matrix;
}

/*
	Deleta uma matrix.
*/
void Lexicon::deleteMatrix(){
	for (int i = 1; i < this->qtdStates; ++i)
	{
		delete[] this->matrix[i];
	}
	delete[] this->matrix;
}

/*
	Configura a matrix de transições.
*/
void Lexicon::configureMatrix(int currentState, int nextState, string myString){
	if(myString == "QC") {
		for(int i = 0; i <= 128; i++) //a-z
		{
			this->matrix[currentState-1][i] = nextState;
		}
	}
	else if(myString == "a-z"){
		for(int i = 'a'; i <= 'z'; i++) //a-z
		{
			this->matrix[currentState-1][i] = nextState;
		}
		for(int i = 'A'; i <= 'Z'; i++) //A-Z
		{
			this->matrix[currentState-1][i] = nextState;
		}
	}
	else if (std::regex_match (myString, std::regex("[a-z]-[a-z]"))) {
		for(int i = myString[0]; i<=myString[2]; i++)
		{
			this->matrix[currentState-1][i] = nextState;
		}
	}
	else if (myString == "0-9") {
		for(int i = myString[0]; i<=myString[2]; i++)
		{
			this->matrix[currentState-1][i] = nextState;
		}
	}
	else 
	{
		this->matrix[currentState-1][myString[0]] = nextState;
	}
}

/*
	Retorna o proximo estado ao ler o caracter c do estado currentState.
*/
int Lexicon::_nextState(int currentState, char c){
	int next, i;
	i = c;
	next = this->matrix[currentState-1][i];
	return next;
}

/*
	Verifica se o estado atual está contido no conjunto de estados finais.
*/
bool Lexicon::_finalState(int currentState){
	for(int i = 0; i<this->finalStates.size(); i++)
	{
		if(this->finalStates[i] == currentState)
		{
			return true;
		}
	}
	return false;
}

/*
	Retorna o token mediante o estado final.
*/
int Lexicon::getToken(int posic){
	if(posic == 2) {
		return 1;
	}
	else if (posic == 3 || posic == 5) {
		cout << "NÚMERO" << "\n";
		return 2;
	}
	else if (posic == 6 || posic == 8 || posic == 10 || posic == 12 || posic == 14 || posic == 16 ||posic == 18 || posic == 20) {
		cout << "SÍMBOLO ESPECIAIS" << "\n";
		return 3;
	}
	else if (posic == 7 || posic == 9 || posic == 11 || posic == 13 || posic == 15 || posic == 17 || posic == 19) {
		cout << "SÍMBOLO ESPECIAIS COMPOSTO" << "\n";
		return 4;
	}
	else {
		return 5;
	}
}

/*
	Retorna o erro léxico mostrando o erro a linha e a coluna.
*/
void Lexicon::error(int state, string error, int line, int column) {
	if(state == 21) {
		cout << "error:line: "<< line << " :column: "<< column << " :símbolo não reconhecido: "<< error << "\n";
	}
	if(state == 22) {
		cout << "error:line: "<< line << " :column: "<< column << " :erro de identificador :" << error << "\n";
	}
}

/*
	Verifica se uma cadeia de caracteres é aceita pelo léxico.
*/
void Lexicon::checkString(string myString, int line){
	int current = this->startState, finalState = 0, last_terminal = 0, index = 0, start = 0, i = 0, token = 0;
	int aux;
	bool tf = false, wt = false;
	while(true)
	{
		//Busca o próximo estado da transição do autômato
		current = _nextState(current, myString[index]);
		//cout<< myString[index] << " " << current <<"\n";

		if(_finalState(current)) //Verifica se a transição ocorreu
		{
			last_terminal = index;
			finalState = current;
		}

		if(current == 0){
			//Calcula o tamanho da string
			aux = start - index;
			if(aux < 0)
				aux = aux * (-1);

			//verifica se não é um erro
			if(finalState != 0){
				token = getToken(finalState);
				if(token == 1){
					//Verifica se é palavra reservada
					//cout << myString.substr(start, aux) << "\n";
					if(this->hashReservedWords->searchItem2(myString.substr(start, aux))){
						cout << "PALAVRA RESERVADA" << "\n";
					} else {
						// Verifica se o id já existe na hashTable
						if(!this->hashIdentifiers->searchItem2(myString.substr(start, aux))){
							cout << "IDENTIFICADOR" << "\n";
							this->hashIdentifiers->insertItem(Item("", myString.substr(start, aux), ""));
						}
					}
				}
				start = last_terminal + 1;
			}
			//verificando se é um erro
			else{
				if(myString[start] != ' ' && myString[start] != '\n' && myString[index] != '\0'){
					this->error(finalState, myString.substr(start, aux), line, index);
				}
				start++;
			}

			current = this->startState;
			finalState = 0;
			index = start-1;
			aux = 0;
		}

		index++;
		if(start == myString.length())
			break;
	}
}