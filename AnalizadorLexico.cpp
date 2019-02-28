#include <cstring>

#include "AnalizadorLexico.h"
#include "regex"

#define FILENAME "Database/lexico.txt"

using namespace std;

AnalizadorLexico::AnalizadorLexico() {
    listData = new Collection<Lexico>;
    listError = new Collection<std::string>;
    readFromDisk();
    }

std::string AnalizadorLexico::getStringToAnalize() {
    return stringToAnalize;
    }

void AnalizadorLexico::setStringToAnalize(std::string& aux) {
    stringToAnalize = aux;
    }

void AnalizadorLexico::receiveString() {
    }
string report = "";
void AnalizadorLexico::analizeString() {
    Lexico lexico;
    Tools* tool;
    string aux = stringToAnalize;

    string sentence = "";

    string auxSubst;
    int i(0);
    int qActual(0);
    bool startSintactico(false);
    bool exit(false);
    bool validateCharacters  = (aux[i] == '"' || aux[i] == '/' || aux[i] == ';' || aux[i] == ',' ||
                                aux[i] == '(' || aux[i] == ')' || aux[i] == '{' || aux[i] == '}' ||
                                aux[i] == '!' || aux[i] == '=' || aux[i] == '+' || aux[i] == '*' ||
                                aux[i] == '-' || aux[i] == '$' || aux[i] == '#' || aux[i] == '<' ||
                                aux[i] == '>' || aux[i] == '|' || aux[i] == '&');
    auxSintactico += aux = tool->strToCapLet(aux);

    do {
        //cout <<"Charac: "<<(int)aux[i]<< endl;
        switch(qActual) {
            case 0:
                if(('A' <= aux[i] && aux[i] <= 'Z') || ('0' <= aux[i] && aux[i] <= '9')) {
                    qActual = 1;
                    sentence+=aux[i];
                    }
                else if(aux[i] == '"' || aux[i] == '/' || aux[i] == ';' || aux[i] == ',' ||
                        aux[i] == '(' || aux[i] == ')' || aux[i] == '{' || aux[i] == '}' ||
                        aux[i] == '!' || aux[i] == '=' || aux[i] == '+' || aux[i] == '*' ||
                        aux[i] == '-' || aux[i] == '$' || aux[i] == '#' || aux[i] == '<' ||
                        aux[i] == '>' || aux[i] == '|' || aux[i] == '&') {
                    sentence = aux[i];
                    qActual = 2;
                    }
                else if(aux[i] == '\0') {
                    qActual = 3;
                    }
                else if(aux[i] == ' ') {
                    qActual = 0;
                    }
                else {
                    qActual = 5;
                    }
                break;
            case 1:
                //cout <<"estado 1"<< endl;
                if(('A' <= aux[i] && aux[i] <= 'Z') || ('0' <= aux[i] && aux[i] <= '9')) {
                    qActual = 1;
                    sentence+=aux[i];
                    }
                else if (aux[i] == '"' || aux[i] == '/' || aux[i] == ';' || aux[i] == ',' ||
                         aux[i] == '(' || aux[i] == ')' || aux[i] == '{' || aux[i] == '}' ||
                         aux[i] == '!' || aux[i] == '=' || aux[i] == '+' || aux[i] == '*' ||
                         aux[i] == '-' || aux[i] == '$' || aux[i] == '#' || aux[i] == '<' ||
                         aux[i] == '>' || aux[i] == '|' || aux[i] == '&') {
                    qActual = 2;
                    i--;
                    }
                else if(aux[i] == ' ') {
                    qActual = 2;
                    }
                else if(aux[i] == '\0') {
                    qActual = 3;
                    }
                else {
                    qActual = 5;
                    }
                break;
            case 2:
                //auxSubst = aux.substr(0,i-1);
                //cout <<"Cadena validar : '" + sentence + "'\n";
                report+="Cadena validar : '" + sentence + "' - ";
                lexico.setName(sentence);
                if(listData->findData(lexico) != nullptr) {
                    lexico = listData->getData(listData->findData(lexico));
                    report+=lexico.getDescription()+"\n";
                    //cout <<lexico.getDescription()+"\n";
                    }
                else if(sentence == "END") {
                    startSintactico = true;
                    }
                else {
                    //cout <<"Es una variable"<< endl;
                    report +="Es una variable\n";
                    //qActual = 3;
                    //exit = true;
                    }
                sentence="";
                qActual=0;
                i--;
                break;
            case 3:
                //cout <<"Cadena validar : '" + sentence + "'\n";
                report +="Cadena validar : '" + sentence + "' - ";
                lexico.setName(sentence);
                if(listData->findData(lexico) != nullptr) {
                    lexico = listData->getData(listData->findData(lexico));
                    //cout <<lexico.getDescription() << endl;
                    report+=lexico.getDescription()+"\n";
                    qActual = 0;
                    }
                else {
                    if(sentence.length() > 0) {
                        report+="Es una variable\n";
                        cout <<"Es una variable"<< endl;;
                        qActual = 3;
                        }
                    }
                //isCorrect = true;
                exit = true;
                break;
            case 5:
                //cout <<"Error, esto no existe en la gramática"<< endl;
                report+="Error, esto no existe en la gramática\n";
                listError->insertData("Error, esto no existe en la gramática\n");
                isCorrect = false;
                qActual = 0;
                break;
            default:
                cout <<"Error"<< endl;
                //exit = true;
			}
        i++;
        }
    while(!exit);
    //cout << report;
    //cout <<auxSintactico<< endl;
    if(startSintactico && isCorrect) {
        isCorrect = true;
        cout <<report<<endl<<endl;
        cout <<"--------------------------------------- Sintaxis ---------------------------------------"<< endl;
        analizeGeneral(auxSintactico);

        }
    if(startSintactico && !isCorrect) {
        cout <<report<< endl;
        }
    }

void AnalizadorLexico::analizeGeneral(std::string& aux) {
    Collection<char>* stackCharacter = new Collection<char>;
    Collection<Variable>* stackVariable = new Collection<Variable>;
    Variable variable;
    string sentence;
    bool exit = false;
    bool isCorrect = true;
    int qActual = 0;
    int i = 0;
    stackCharacter->insertData('°');
    sentence ="°";
    variable.setName(sentence);
    stackVariable->push(variable);
    sentence = "";
    cout <<"Analizando sintácticamente: '"<<aux<<"'"<< endl;
    do {
        switch(qActual) {
            case 0:
                cout <<"Case 0 con '"<<aux[i]<<"' '"<<(int)aux[i]<<"' llegando"<< endl;
                if('A' <= aux[i] && aux[i] <= 'Z') {
                    qActual = 0;
                    sentence+=aux[i];
                    }
                else if(aux[i] == ';') {
                    qActual = 1;
                    }
                else {
                    qActual = 0;
                    }
                break;
            case 1:
                cout <<"Case 1 con '"<<aux[i]<<"' '"<<(int)aux[i]<<"' llegando"<< endl;
                cout <<"Sentences = '"<<sentence<<"' "<< endl;
                if(sentence == "INICIO") {
                    qActual = 2;
                    estadoActual = 2;
                    i--;
                    }
                else {
                    qActual = 0;
                    i--;
                    }
                sentence = "";
                break;
            case 2:
                cout <<"Case 2 con '"<<aux[i]<<"' '"<<(int)aux[i]<<"' llegando"<< endl;
                if(aux[i] == '$') {
                    qActual = 3;
                    }
                else if('A' <= aux[i] && aux[i] <= 'Z') {
                    qActual = 5;
                    sentence = aux[i];
                    }
                else if(aux[i] == '}') {
                    qActual = 2;
                    if(stackCharacter->getTop() == '{') {
                        stackCharacter->pop();
                        }
                    else {
                        cout <<"Setting false"<< endl;
                        isCorrect = false;
                        }
                    }
                else if(aux[i] == '\0') {
                    exit = true;
                    qActual = 2;
                    }
                else {
                    cout <<"Error, sintáxis incorrecta en: '"<<aux[i]<<"'"<< endl;
                    }
                break;
            case 3:
                cout <<"Case 3 con '"<<aux[i]<<"' '"<<(int)aux[i]<<"' llegando"<< endl;
                if('A' <= aux[i] && aux[i] <= 'Z') {
                    qActual = 3;
                    sentence+=aux[i];
                    }
                else if(aux[i] == ';') {
                    qActual = 4;
                    }
                else {
                    cout <<"Error sintáxis"<< endl;
                    }
                break;
            case 4:
                cout <<"Case 4 con '"<<aux[i]<<"' '"<<(int)aux[i]<<"' llegando"<< endl;
                qActual = 2;
                //aux = tool->strToCapLet(sentence);
                variable.setName(sentence);
                stackVariable->push(variable);
                sentence = "";
                i--;
                break;
            case 5:
                cout <<"Case 5 con '"<<aux[i]<<"' '"<<(int)aux[i]<<"' llegando"<< endl;
                if('A' <= aux[i] && aux[i] <= 'Z') {
                    qActual = 5;
                    sentence+=aux[i];
                    }
                else {
                    variable.setName(sentence);
                    cout <<"En el tpe variable hay: "<<stackVariable->getTop().getName()<< endl;
                    cout <<"De sentencia hay: "<<sentence<< endl;
                    if(sentence == "IF") {
                        i=analizeIF(aux,i);
                        if(aux[i] == '{') {
                            stackCharacter->push('{');
                            }
                        else {
                            listError->insertData("Error, falta un '{' después del IF");
                            isCorrect = false;
                            i--;
                            }
                        cout <<"Después del if el I está en: '"<<aux[i]<<"' '"<<(int)aux[i]<<"' i="<<i<< endl;;

                        qActual = 2;
                        }
                    else if(sentence == "WHILE") {
                        i=analizeWHILE(aux,i);
                        if(aux[i] == '{') {
                            stackCharacter->push('{');
                            }
                        else {
                            listError->insertData("Error, falta un '{' después del WHILE");
                            isCorrect = false;
                            i--;
                            }
                        qActual = 2;
                        }
                    else if(sentence == "FOR") {
                        i=analizeFOR(aux,i);
                        if(aux[i] == '{') {
                            stackCharacter->push('{');
                            }
                        else {
                            listError->insertData("Error, falta un '{' después del FOR");
                            isCorrect = false;
                            i--;
                            }
                        qActual = 2;
                        }
                    else if(sentence == "END") {
                        estadoActual = 2;
                        cout <<"Tope character = "<<stackCharacter->getTop()<< endl;
                        if(stackCharacter->getTop() != '°') {
                            isCorrect = false;

                            }
                        exit = true;
                        }
                    else if(sentence == "AOUT") {
                        qActual = 6;
                        i--;
                        }
					else if(sentence == "AIN") {
						qActual = 10;
						i--;
					}
                    else if(stackVariable->findData(variable) == nullptr) {
                        listError->insertData("Error, la variable : '"+variable.getName()+"' no ha sido declarada");
                        qActual = 2;
                        }
                    else {
                        //listError->insertData("Error, esto: '"+variable.getName()+"' no existe");
                        //cout <<"Error, esto no existe"<< endl;
                        qActual = 15;
                        }
                    sentence = "";
                    //cout <<"Yendo a "<<qActual<<" desde 5"<< endl;
                    }
                break;
            case 6:
                cout <<"Case 6 con '"<<aux[i]<<"' '"<<(int)aux[i]<<"' llegando"<< endl;
                if(aux[i] != '<') {
                    listError->insertData("Error, se esperaba un '<' después de 'AOUT'");
                    }
                qActual = 7;
                break;
            case 7:
                cout <<"Case 7 con '"<<aux[i]<<"' '"<<(int)aux[i]<<"' llegando"<< endl;
                if(aux[i] != '"') {
                    listError->insertData("Error, se esperaba un '\"' en 'AOUT'");
                    }
                qActual = 8;
                break;
            case 8:
                cout <<"Case 8 con '"<<aux[i]<<"' '"<<(int)aux[i]<<"' llegando"<< endl;
                if(aux[i] == '\0'){
                    listError->insertData("Error, se esperaba un '\"' en 'AOUT'");
                    exit = true;
                }
                if(aux[i] != '"') {
                    qActual = 8;
                    }
                else{
                    qActual = 9;
                }
                break;
            case 9:
                cout <<"Case 9 con '"<<aux[i]<<"' '"<<(int)aux[i]<<"' llegando"<< endl;
                if(aux[i] != ';') {
                    listError->insertData("Error, se esperaba un ';' en 'AOUT'");
                    }
                qActual = 2;
                break;
			case 10:  /// /////////// Validando el > en AIN //////////////////
				cout <<"Case 10 con '"<<aux[i]<<"' '"<<(int)aux[i]<<"' llegando"<< endl;
				if(aux[i] != '>'){
					listError->insertData("Error, se esperaba un '>' en 'AIN'");
				}
				qActual = 11;
				break;
			case 11:
				cout <<"Case 11 con '"<<aux[i]<<"' '"<<(int)aux[i]<<"' llegando"<< endl;
				if(aux[i] == '"'){
                    qActual = 12;
				}
				else if(aux[i] == '\''){
					qActual = 13;
				}
				else{
					listError->insertData("Error, se esperana un '\'' o un '\"' en 'AIN'");
					qActual = 14;
				}
				break;
			case 12:
				cout <<"Case 12 con '"<<aux[i]<<"' '"<<(int)aux[i]<<"' llegando"<< endl;
				if(aux[i] != '"'){
					sentence+=aux[i];
					qActual = 12;
				}
				else{
					variable.setName(sentence);
					if(stackVariable->findData(variable) == nullptr){
						listError->insertData("Error, la variable '"+variable.getName()+"' no ha sido previamente declarada");
					}
					sentence="";
                    qActual = 14;
				}
				break;
			case 13:
				cout <<"Case 13 con '"<<aux[i]<<"' '"<<(int)aux[i]<<"' llegando"<< endl;
				if(aux[i] != '\''){
					qActual = 13;
				}
				else{
					variable.setName(sentence);
					if(stackVariable->findData(variable) == nullptr){
						listError->insertData("Error, la variable '"+variable.getName()+"' no ha sido previamente declarada");
					}
					sentence="";
					qActual = 14;
				}
				break;
			case 14:
                cout <<"Case 14 con '"<<aux[i]<<"' '"<<(int)aux[i]<<"' llegando"<< endl;
                if(aux[i] != ';'){
					listError->insertData("Error, se esperaba un ';' en 'AIN'");
                }
                qActual = 2;
                break;
			case 15:
				cout <<"Case 15 con '"<<aux[i]<<"' '"<<(int)aux[i]<<"' llegando"<< endl;
				if(aux[i] != '='){
					qActual=15;
					sentence+=aux[i];
				}
				else{
					qActual = 16;
					variable.setName(sentence);
					if(stackVariable->findData(variable) == nullptr){
						listError->insertData("Error, la variable '"+variable.getName()+"' no ha sido previamente declarada");
					}
					sentence="";
				}
				break;
			case 16:
				cout <<"Case 16 con '"<<aux[i]<<"' '"<<(int)aux[i]<<"' llegando"<< endl;
				if(aux[i] != '+' && aux[i] != '-' && aux[i] != '/' && aux[i] != '*'){
					qActual=16;
					sentence+=aux[i];
				}
				else{
					qActual = 17;
					variable.setName(sentence);
					if(stackVariable->findData(variable) == nullptr){
						listError->insertData("Error, la variable '"+variable.getName()+"' no ha sido previamente declarada");
					}
					sentence="";
				}
				break;
			case 17:
				cout <<"Case 17 con '"<<aux[i]<<"' '"<<(int)aux[i]<<"' llegando"<< endl;
				if(aux[i] != ';'){
					qActual=17;
					sentence+=aux[i];
				}
				else{
					qActual = 2;
					variable.setName(sentence);
					if(stackVariable->findData(variable) == nullptr){
						listError->insertData("Error, la variable '"+variable.getName()+"' no ha sido previamente declarada");
					}
					sentence="";
				}
			case 20:
                cout <<"Case 20 con '"<<aux[i]<<"' llegando"<< endl;
                exit = true;
                cout <<"Error, : "<< endl;
                break;
            default:
                break;
            }
        i++;
        }
    while(!exit);
    cout <<endl<<endl<<"Errores: "<< endl;
    //cout <<"Siguiente estado = : "<<estadoActual<< endl;
    if(!listError->isEmpty()) {
        cout <<"-------------Sintáxis inválida----------------"<< endl;
        while(!listError->isEmpty()) {
			cout <<"Error: "<<listError->dequeue()<< endl;
			}
        }
    else {
        cout <<"-------------Sintáxis válida----------------"<< endl;
        }
    }

int AnalizadorLexico::analizeIF(const std::string& aux, const int& auxInt) {
    Collection<char>* listCharacter = new Collection<char>;
    string auxError;
    bool exit = false;
    int qActual = 0;
    int i = auxInt;
    listCharacter->push('°');
    do {
        switch(qActual) {
            case 0:
                if(aux[i] == '(') {
                    qActual = 1;
                    listCharacter->push(aux[i]);
                    }
                else {
                    auxError = "Falta un '('";
                    qActual = 10;
                    }
                break;
            case 1:
                if(aux[i] == '(') {
                    qActual = 1;
                    listCharacter->push(aux[i]);
                    }
                else if('A' <= aux[i] && aux[i] < 'Z') {
                    qActual = 2;
                    }
                else if('0' <= aux[i] && aux[i] <='9') {
                    qActual = 3;
                    }
                else if(aux[i] == '!') {
                    qActual = 9;
                    }
                else {
                    qActual = 10;
                    auxError = "Caracter '"+aux[i];
                    auxError += "' no válido";
                    }
                break;
            case 2: // /////////////// Valida los números antes del operador ////////
                //cout <<"q2"<< endl;
                if('A' <= aux[i] && aux[i] <= 'Z') {
                    qActual = 2;
                    }
                else if(aux[i] == '<' || aux[i] == '>' || aux[i] == '!' || aux[i]=='&' || aux[i] == '=') {
                    qActual = 4;
                    }
                else if(aux[i] == ')') {
                    qActual = 6;
                    }
                else {
                    qActual = 10;
                    auxError = "Operando inválido, 2";
                    }
                break;
            case 3: // /////////// Valida los números antes del operador////////////
                cout <<"q3"<< endl;
                if('0' <= aux[i] && aux[i] <= '9') {
                    qActual = 3;
                    }
                else if(aux[i] == '<' || aux[i] == '>' || aux[i] == '!' || aux[i] == '&' || aux[i] == '=') {
                    qActual = 5;
                    }
                else if(aux[i] == ')') {
                    qActual = 6;
                    }
                else {
                    qActual = 10;
                    auxError = "Operando inválido, 3";
                    }
                break;
            case 4: // ///////////// Valida las letras despues del operador////////////
                if('A' <= aux[i] && aux[i] <= 'Z') {
                    qActual = 7;
                    }
                else if('0' <= aux[i] && aux[i] <= '9') {
                    qActual = 8;
                    }
                else {
                    qActual = 10;
                    auxError = "Operando inválido, 4";
                    }
                break;
            case 5: // /////////////// Valida los numeros despues del operador /////////
                if('0' <= aux[i] && aux[i] <= '9') {
                    qActual = 8;
                    }
                else if('A' <= aux[i] && aux[i] <= 'Z') {
                    qActual = 7;
                    }
                else {
                    qActual = 10;
                    auxError = "Operando inválido, 5";
                    }
                break;
            case 6:
                //cout <<"IF Válido"<< endl;
                exit = true;
                break;
            case 7:
                if('A' <= aux[i] && aux[i] <= 'Z') {
                    qActual = 7;
                    }
                else if(aux[i] == ')') {
                    qActual = 6;
                    }
                else {
                    qActual = 10;
                    auxError = "Operando inválido, 7";
                    }
                break;
            case 8:
                if('0' <= aux[i] && aux[i] <= '9') {
                    qActual = 8;
                    }
                else if(aux[i] == ')') {
                    qActual = 6;
                    }
                else {
                    qActual = 10;
                    auxError = "Operando inválido, 8";
                    }
                break;
            case 9: // ///////// Valida en caso de ! //////////
                if('A' <= aux[i] && aux[i] <= 'Z') {
                    qActual = 7;
                    }
                else if(aux[i] == ')') {
                    qActual = 6;
                    }
                else {
                    qActual = 10;
                    auxError = "Operando inválido, 9";
                    }
                break;
            case 10:
                exit = true;
                listError->insertData("Error, sentencia IF mal, " + auxError);
                break;
            default:
                break;
            }
        i++;
        }
    while(!exit);
    if(!listError->isEmpty()) {
        cout <<"IF Correcto"<< endl;
        }
    else {
        while(!listError->isEmpty()) {
            cout <<listError->pop()<< endl;
            }
        }
    return --i;
    }

int AnalizadorLexico::analizeFOR(const std::string& aux, const int& auxInt) {

    }

int AnalizadorLexico::analizeWHILE(const std::string& aux, const int& auxInt) {
    Collection<char>* listCharacter = new Collection<char>;
    string auxError;
    bool exit = false;
    int qActual = 0;
    int i = auxInt;
    listCharacter->push('°');
    do {
        switch(qActual) {
            case 0:
                if(aux[i] == '(') {
                    qActual = 1;
                    listCharacter->push(aux[i]);
                    }
                else {
                    auxError = "Falta un '('";
                    qActual = 10;
                    }
                break;
            case 1:
                if(aux[i] == '(') {
                    qActual = 1;
                    listCharacter->push(aux[i]);
                    }
                else if('A' <= aux[i] && aux[i] < 'Z') {
                    qActual = 2;
                    }
                else if('0' <= aux[i] && aux[i] <='9') {
                    qActual = 3;
                    }
                else if(aux[i] == '!') {
                    qActual = 9;
                    }
                else {
                    qActual = 10;
                    auxError = "Caracter '"+aux[i];
                    auxError += "' no válido";
                    }
                break;
            case 2: // /////////////// Valida los números antes del operador ////////
                //cout <<"q2"<< endl;
                if('A' <= aux[i] && aux[i] <= 'Z') {
                    qActual = 2;
                    }
                else if(aux[i] == '<' || aux[i] == '>' || aux[i] == '!' || aux[i]=='&' || aux[i] == '=') {
                    qActual = 4;
                    }
                else if(aux[i] == ')') {
                    qActual = 6;
                    }
                else {
                    qActual = 10;
                    auxError = "Operando inválido, 2";
                    }
                break;
            case 3: // /////////// Valida los números antes del operador////////////
                cout <<"q3"<< endl;
                if('0' <= aux[i] && aux[i] <= '9') {
                    qActual = 3;
                    }
                else if(aux[i] == '<' || aux[i] == '>' || aux[i] == '!' || aux[i] == '&' || aux[i] == '=') {
                    qActual = 5;
                    }
                else if(aux[i] == ')') {
                    qActual = 6;
                    }
                else {
                    qActual = 10;
                    auxError = "Operando inválido, 3";
                    }
                break;
            case 4: // ///////////// Valida las letras despues del operador////////////
                if('A' <= aux[i] && aux[i] <= 'Z') {
                    qActual = 7;
                    }
                else if('0' <= aux[i] && aux[i] <= '9') {
                    qActual = 8;
                    }
                else {
                    qActual = 10;
                    auxError = "Operando inválido, 4";
                    }
                break;
            case 5: // /////////////// Valida los numeros despues del operador /////////
                if('0' <= aux[i] && aux[i] <= '9') {
                    qActual = 8;
                    }
                else if('A' <= aux[i] && aux[i] <= 'Z') {
                    qActual = 7;
                    }
                else {
                    qActual = 10;
                    auxError = "Operando inválido, 5";
                    }
                break;
            case 6:
                //cout <<"IF Válido"<< endl;
                exit = true;
                break;
            case 7:
                if('A' <= aux[i] && aux[i] <= 'Z') {
                    qActual = 7;
                    }
                else if(aux[i] == ')') {
                    qActual = 6;
                    }
                else {
                    qActual = 10;
                    auxError = "Operando inválido, 7";
                    }
                break;
            case 8:
                if('0' <= aux[i] && aux[i] <= '9') {
                    qActual = 8;
                    }
                else if(aux[i] == ')') {
                    qActual = 6;
                    }
                else {
                    qActual = 10;
                    auxError = "Operando inválido, 8";
                    }
                break;
            case 9: // ///////// Valida en caso de ! //////////
                if('A' <= aux[i] && aux[i] <= 'Z') {
                    qActual = 7;
                    }
                else if(aux[i] == ')') {
                    qActual = 6;
                    }
                else {
                    qActual = 10;
                    auxError = "Operando inválido, 9";
                    }
                break;
            case 10:
                exit = true;
                listError->insertData("Error, sentencia IF mal, " + auxError);
                break;
            default:
                break;
            }
        i++;
        }
    while(!exit);
    if(!listError->isEmpty()) {
        cout <<"IF Correcto"<< endl;
        }
    else {
        while(!listError->isEmpty()) {
            cout <<listError->pop()<< endl;
            }
        }
    return --i;
    }

void AnalizadorLexico::readFromDisk() {
    //fstream readerFile;
    Lexico lexico;
    fstream readerFile;
    string str1, str2, str3;
    readerFile.open(FILENAME, ios::in);
    if(!readerFile.is_open()) {
        cout <<"Error, archivo no abierto para lectura TABOP"<< endl;
        return;
        }
    while(!readerFile.eof()) {
        getline(readerFile,str1,'°');
        getline(readerFile,str2,'°');
        getline(readerFile,str3,'°');
        if(!readerFile.eof()) {
            lexico.setId(atoi(str1.c_str()));
            lexico.setName(str2);
            lexico.setDescription(str3);
            listData->insertData(lexico);
            }
        cout <<lexico.getName()<< endl;

        }
    }

