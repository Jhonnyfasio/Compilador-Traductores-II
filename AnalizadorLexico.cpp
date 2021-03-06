#include <cstring>

#include "AnalizadorLexico.h"
#include "regex"

#define FILENAME "Database/lexico.txt"

using namespace std;

typedef enum {Q0=0,Q1,Q2,Q3,Q4,Q5,Q6,Q7,Q8,Q9,Q10,Q11,Q12,Q13,Q14,Q15,Q16,Q17,Q18,Q19,Q20,Q21,Q22,
              Q23,Q24,Q25,Q26} QAnalize;
typedef enum {NULO=0, STRING, CHAR, INT} VariablesTypes;

AnalizadorLexico::AnalizadorLexico() {
    listData = new Collection<Lexico>;
    listError = new Collection<std::string>;
    listWarning = new Collection<std::string>;
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
    Tools* tool = new Tools();
    string aux = stringToAnalize;

    string sentence = "";

    string auxSubst;
    int i(0);
    int qActual(0);
    bool startSintactico(false);
    bool exit(false);
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
                //cout <<"Error, esto no existe en la gram�tica"<< endl;
                report+="Error, esto no existe en la gram�tica\n";
                listError->insertData("Error, esto no existe en la gram�tica\n");
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
    //cout <<"-------------------------------"<< endl;
    }

void AnalizadorLexico::analizeGeneral(std::string& aux) {
    Collection<char>* stackCharacter = new Collection<char>;
    stackVariable = new Collection<Variable>;
    listVariableType = new Collection<int>;
    Variable variable;
    string sentence;
    string auxVariable;
    bool exit = false;
    bool isCorrect = false;
    int qActual = 0;
    int i = 0;
    int variableType = 0;
    int firstVariableType = 0, secondVariableType = 0;
    stackCharacter->insertData('�');
    sentence ="�";
    variable.setName(sentence);
    variable.setIsUsed(true);
    stackVariable->push(variable);
    sentence = "";
    cout <<"Analizando sint�cticamente: '"<<aux<<"'"<< endl;
    do {
        switch(qActual) {
            case Q0: /// Estado inicial, ignora todo hasta la llegada de un ;
                cout <<"Case 0 con '"<<aux[i]<<"' '"<<(int)aux[i]<<"' llegando"<< endl;
                if('A' <= aux[i] && aux[i] <= 'Z') {
                    qActual = 0;
                    sentence+=aux[i];
                    }
                else if(aux[i] == ';') {
                    qActual = Q1;
                    }
                else {
                    qActual = 0;
                    }
                break;
            case Q1: /// Valida si lo ingresado es un INICIO; indicando la lectura del programa
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
            case Q2: /// Estado base, aqu� revisa todo lo que llegue para su sintaxis general
                cout <<"Case 2 con '"<<aux[i]<<"' '"<<(int)aux[i]<<"' llegando"<< endl;
                if(aux[i] == '$') {
                    qActual = 23;
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
                        //cout <<"Setting false"<< endl;
                        //isCorrect = false;
                        listError->insertData("Error, falta un '}'");
                        }
                    }
                else if(aux[i] == '\0') {
                    exit = true;
                    qActual = 2;
                    }
                else {
                    cout <<"Error, sint�xis incorrecta en: '"<<aux[i]<<"'"<< endl;
                    }
                break;
            case Q23:
            	cout <<"Case 23 con '"<<aux[i]<<"' '"<<(int)aux[i]<<"' llegando"<< endl;
                if(aux[i] == '"'){
                    variableType = 1;
                }
                else if(aux[i] == '\''){
                    variableType = 2;
                }
                else if(('A' <= aux[i] && aux[i] <= 'Z') || ('0' <= aux[i] && aux[i] <= '9')) {
                    variableType = 3;
                    sentence+=aux[i];
                    }
                qActual = 3;
                break;
            case Q3: /// Empieza a revisar la declaraci�n de las variables
                cout <<"Case 3 con '"<<aux[i]<<"' '"<<(int)aux[i]<<"' llegando"<< endl;
                if(('A' <= aux[i] && aux[i] <= 'Z') || ('0' <= aux[i] && aux[i] <= '9')) {
                    qActual = 3;
                    sentence+=aux[i];
                    }
                else if(aux[i] == ';') {
                    qActual = 4;
                    }
                else {
                    listError->insertData("Falta un ';'");
                    }
                break;
            case Q4: /// valida e inserta la variable
                cout <<"Case 4 con '"<<aux[i]<<"' '"<<(int)aux[i]<<"' llegando"<< endl;
                qActual = 2;
                //aux = tool->strToCapLet(sentence);
                if(isVariable(sentence)) {
                    if(!existVariable(sentence)) {
                        variable.setName(sentence);
                        variable.setIsUsed(false);
                        variable.setType(variableType);
                        stackVariable->push(variable);
                        cout <<"Variable: "<<sentence<<" con valor: "<<variableType<< endl;
                        }
                    else {
                        listError->insertData("Error, esta variable ya has ido previamente declarada");
                        }
                    }
                else {
                    listError->insertData("Error, sint�xis de variable err�nea");
                    }

                sentence = "";
                i--;
                break;
            case Q5: /// Valida si es un FOR, WHILE, IF, END, AOUT, AIN, variableX
                cout <<"Case 5 con '"<<aux[i]<<"' '"<<(int)aux[i]<<"' llegando"<< endl;
                if(('A' <= aux[i] && aux[i] <= 'Z') || ('0' <= aux[i] && aux[i] <= '9')) {
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
                            listError->insertData("Error, falta un '{' despu�s del IF");
                            isCorrect = false;
                            i--;
                            }
                        cout <<"Despu�s del if el I est� en: '"<<aux[i]<<"' '"<<(int)aux[i]<<"' i="<<i<< endl;;

                        qActual = 2;
                        }
                    else if(sentence == "WHILE") {
                        i=analizeWHILE(aux,i);
                        if(aux[i] == '{') {
                            stackCharacter->push('{');
                            }
                        else {
                            listError->insertData("Error, falta un '{' despu�s del WHILE");
                            isCorrect = false;
                            i--;
                            }
                        qActual = 2;
                        }
                    else if(sentence == "FOR") {
                        i=analizeFOR(aux,i);
                        cout <<"Despu�s del FOR el I est� en: '"<<aux[i]<<"' '"<<(int)aux[i]<<"' i="<<i<< endl;
                        if(aux[i] == '{') {
                            stackCharacter->push('{');
                            }
                        else {
                            listError->insertData("Error, falta un '{' despu�s del FOR");
                            isCorrect = false;
                            i--;
                            }

                        qActual = 2;
                        }
                    else if(sentence == "END") {
                        estadoActual = 2;
                        cout <<"Tope character = "<<stackCharacter->getTop()<< endl;
                        if(stackCharacter->getTop() != '�') {
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
                        auxVariable=variable.getName();
                        /*if(isVariable(auxVariable)) { /// Esto posiblemente pueda ser omitido (?)
                            listError->insertData("Error, la palabra : '"+variable.getName()+"' no es una variable");
                            }*/
                        listError->insertData("Error, la variable : '"+variable.getName()+"' no ha sido declarada");
                        qActual = 2;
                        }
                    else {
                        //listError->insertData("Error, esto: '"+variable.getName()+"' no existe");
                        //cout <<"Error, esto no existe"<< endl;
                        cout <<"Variable: "<<variable.getName()<<" con valor: "<<variable.getType()<<" insertando en list variable"<< endl;
                        listVariableType->insertData(variable.getType());
                        qActual = 15;
                        i--;
                        }
                    sentence = "";
                    //cout <<"Yendo a "<<qActual<<" desde 5"<< endl;
                    }
                break;
            case Q6: /// Valida el < despu�s de aout
                cout <<"Case 6 con '"<<aux[i]<<"' '"<<(int)aux[i]<<"' llegando"<< endl;
                if(aux[i] != '<') {
                    listError->insertData("Error, se esperaba un '<' despu�s de 'AOUT'");
                    }
                qActual = 7;
                break;
            case Q7: /// valida que lleguen un " o variable para salida de texto
                cout <<"Case 7 con '"<<aux[i]<<"' '"<<(int)aux[i]<<"' llegando"<< endl;
                if(aux[i] == '"') {
                    qActual = 8;
                    }
                else if(isdigit((int)aux[i]) || isalpha((int)aux[i])) {
                    qActual = 21;
                    sentence=aux[i];
                    }
                else {
                    listError->insertData("Error, se esperaba un '\"' en 'AOUT'");
                    }
                qActual = 8;
                break;
            case Q8: /// Valida puro texto de salida
                cout <<"Case 8 con '"<<aux[i]<<"' '"<<(int)aux[i]<<"' llegando"<< endl;
                if(aux[i] == '\0') {
                    listError->insertData("Error, se esperaba un '\"' en 'AOUT'");
                    exit = true;
                    }
                if(aux[i] != '"') {
                    sentence+=aux[i];
                    qActual = 8;
                    }
                else {
                    qActual = 9;
                    }
                break;
            case Q21:
                if(isdigit((int)aux[i]) || isalpha((int)aux[i])) {
                    sentence+=aux[i];
                    qActual = 21;
                    }
                else if(aux[i] == ';') {
                    qActual = 2;
                    if(existVariable(sentence)) {
                        variable.setName(sentence);
                        stackVariable->retrieveData(stackVariable->findData(variable)).setIsUsed(true);
                        }
                    else{
                        listError->insertData("Error, la variable '"+sentence+"' no ha sido previamente declarada");
                    }
                    sentence = "";
					}
                else {
                    listError->insertData("Error, se esperaba un ';' en AOUT / AIN");
                    qActual = 2;
                    }
                break;
            case Q9: /// Valida que llegue un ; en AOUT
                cout <<"Case 9 con '"<<aux[i]<<"' '"<<(int)aux[i]<<"' llegando"<< endl;
                if(aux[i] != ';') {
                    listError->insertData("Error, se esperaba un ';' en 'AOUT'");
                    }
                qActual = 2;
                break;
            case Q10:  /// /////////// Validando el > en AIN //////////////////
                cout <<"Case 10 con '"<<aux[i]<<"' '"<<(int)aux[i]<<"' llegando"<< endl;
                if(aux[i] != '>') {
                    listError->insertData("Error, se esperaba un '>' en 'AIN'");
                    }
                qActual = 11;
                break;
            case Q11: /// Valida la variable de entrada
                cout <<"Case 11 con '"<<aux[i]<<"' '"<<(int)aux[i]<<"' llegando"<< endl;
				if(isdigit((int)aux[i] || isalpha((int)aux[i]))) {
                    qActual = 11;
                    sentence = aux[i];
                    }
				else if(aux[i] == ';'){
					qActual = 14;
					i--;
					}
                else {
                    listError->insertData("Error, variable '"+sentence+"' incorrecta en 'AIN'");
                    qActual = 14;
                    }
                break;
            case Q12: /// recolecta todo lo referente a la variable
                break;
            case Q13:
                break;
            case Q14:
                cout <<"Case 14 con '"<<aux[i]<<"' '"<<(int)aux[i]<<"' llegando"<< endl;
                if(aux[i] != ';') {
                    listError->insertData("Error, se esperaba un ';' en 'AIN'");
                    }
                qActual = 2;
                break;
            case Q22:
                /*cout <<"Case 22 con '"<<aux[i]<<"' '"<<(int)aux[i]<<"' llegando"<< endl;
                if(isalpha((int)aux[i]) || isdigit((int)aux[i])) {
                    sentence+=aux[i];
                    qActual = 22;
                    }
                else {
                    variable.setName(sentence);
                    if(stackVariable->findData(variable) != nullptr) {
                        stackVariable->retrieveData(stackVariable->findData(variable)).setIsUsed(true);
                        listError->insertData("Error, la variable '"+variable.getName()+"' no ha sido previamente declarada");
                        }
                    sentence="";
                    qActual = 14;
                    }*/
                break;
            case Q15:
                cout <<"Case 15 con '"<<aux[i]<<"' '"<<(int)aux[i]<<"' llegando"<< endl;
                if(aux[i] != '=') {
                    listError->insertData("Error inesperado en una variable");
                    }
				else{

				}
				qActual=16;
                break;
            case Q16:
                cout <<"Case 16 con '"<<aux[i]<<"' '"<<(int)aux[i]<<"' llegando"<< endl;
                if(isalpha((int)aux[i]) || isdigit((int) aux[i])){
                    sentence+=aux[i];
                    qActual = 16;
                }
                else if(aux[i] == '+' || aux[i] == '-' || aux[i] == '/' || aux[i] == '*' || aux[i] == '%' ){
                    qActual = 16;
                    variable.setName(sentence);
                    if(stackVariable->findData(variable) == nullptr) {
                        listError->insertData("Error, la variable '"+variable.getName()+"' no ha sido previamente declarada");
                        }
					else{
						stackVariable->retrieveData(stackVariable->findData(variable)).setIsUsed(true);
						cout <<"Variable: "<<variable.getName()<<" con valor: "<<variable.getType()<<" insertando en list variable"<< endl;
						listVariableType->insertData(variable.getType());
					}
                    sentence = "";
                }
                else if(aux[i] == ';'){
                    qActual = 2;
                    variable.setName(sentence);
                    sentence="";
                    if(stackVariable->findData(variable) == nullptr) {
                        listError->insertData("Error, la variable '"+variable.getName()+"' no ha sido previamente declarada");
                        }
					else{
						stackVariable->retrieveData(stackVariable->findData(variable)).setIsUsed(true);
						cout <<"Variable: "<<variable.getName()<<" con valor: "<<variable.getType()<<" insertando en list variable"<< endl;
						listVariableType->insertData(variable.getType());
					}
					int auxInt;
					if(!listVariableType->isEmpty()){
						auxInt = listVariableType->pop();
						cout <<"Agarr�: "<<auxInt<< endl;
					}

					while(!listVariableType->isEmpty()){
						cout <<" Tope: "<<listVariableType->getTop()<< endl;
						if(auxInt != listVariableType->pop()){
							listError->insertData("Error, incoherencia de relaci�n de variables '"+variable.getName()+"'");
							}
						}
					listVariableType->deleteAll();
					}
                else{
					qActual=16;
					auxVariable=aux[i];
                    listError->insertData("Error, '"+ auxVariable + "' no se esperaba");
                }
                break;
            case Q17:
                /*cout <<"Case 17 con '"<<aux[i]<<"' '"<<(int)aux[i]<<"' llegando"<< endl;
                if(aux[i] != ';') {
                    qActual=17;
                    sentence+=aux[i];
                    }
                else {
                    qActual = 2;
                    variable.setName(sentence);
                    if(stackVariable->findData(variable) == nullptr) {
                        listError->insertData("Error, la variable '"+variable.getName()+"' no ha sido previamente declarada");
                        }
                    sentence="";
                    }*/
                    break;
            case Q20:  /// De error
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
    cout <<endl<<endl<<"Resultado: "<< endl;
    //cout <<"Siguiente estado = : "<<estadoActual<< endl;
    /*if(!stackCharacter->isEmpty()){
        listError->insertData("Error, falta un '}'");
    }*/

    while(!stackVariable->isEmpty()){
		variable=stackVariable->pop();
		if(variable.getIsUsed() == false){
			listWarning->insertData("Warning, variable '"+variable.getName()+"' no utilizada");
		}
    }

    if(!listError->isEmpty()) {
        cout <<"-------------Sint�xis inv�lida----------------"<< endl;
        while(!listError->isEmpty()) {
            cout <<"Error: "<<listError->dequeue()<< endl;
            }
        }
    else {
        cout <<"-------------Sint�xis v�lida----------------"<< endl;
        }

	while(!listWarning->isEmpty()){
		cout <<listWarning->dequeue()<< endl;
		}
    }

int AnalizadorLexico::analizeIF(const std::string& aux, const int& auxInt) {
    Collection<char>* listCharacter = new Collection<char>;
    Variable variable;
    string auxError, sentence;
    bool exit = false;
    int qActual = 0;
    int i = auxInt;
    listCharacter->push('�');
    do {
        switch(qActual) {
            case 0:
                if(aux[i] == '(') {
                    qActual = 1;
                    listCharacter->push(aux[i]);
                    }
                else {
                    listError->insertData("Error, falta un '(' en IF");
                    qActual = 10;
                    }
                break;
            case 1:
                if(aux[i] == '(') {
                    qActual = 1;
                    listCharacter->push(aux[i]);
                    }
                else if(isalnum(aux[i])) {
                    qActual = 2;
                    sentence=aux[i];
                    }
                else if(aux[i] == '!') {
                    qActual = 9;
                    sentence=aux[i];
                    }
                else {
                    qActual = 10;
                    auxError = aux[i];
                    listError->insertData("Error, caracter '"+auxError+"' no v�lido");
                    }
                break;
            case 2: // /////////////// Valida los n�meros antes del operador ////////
                //cout <<"q2"<< endl;
                if(isalnum(aux[i])) {
                    qActual = 2;
                    sentence+=aux[i];
                    }
                else if(aux[i] == '<' || aux[i] == '>' || aux[i] == '!' || aux[i]=='&' || aux[i] == '=') {
                    qActual = 4;
					variable.setName(sentence);
					sentence = "";
                     if(stackVariable->findData(variable) == nullptr) {
						if(!validOnlyNumber(variable.getName())){
							listError->insertData("Error, la variable '"+variable.getName()+"' no ha sido previamente declarada");
							}
                        }
					else{
						stackVariable->retrieveData(stackVariable->findData(variable)).setIsUsed(true);
						listVariableType->insertData(variable.getType());
                    }
                }
                else if(aux[i] == ')') {
                    qActual = 6;
                    variable.setName(sentence);
					sentence = "";
                     if(stackVariable->findData(variable) == nullptr) {
						if(!validOnlyNumber(variable.getName())){
							listError->insertData("Error, la variable '"+variable.getName()+"' no ha sido previamente declarada");
							}
                        }
					else{
						stackVariable->retrieveData(stackVariable->findData(variable)).setIsUsed(true);
						listVariableType->insertData(variable.getType());
                    }
                    }
                else {
                    qActual = 10;
					listError->insertData("Operando inv�lido, 2");
                    }
                break;
            case 4: /// ///////////// Valida las letras despues del operador////////////
                if(isalnum(aux[i])) {
                    qActual = 7;
                    sentence=aux[i];
                    }
                else {
                    qActual = 10;
                    listError->insertData("Operando inv�lido, 4");
                    }
                break;
            case 6:
                //cout <<"IF V�lido"<< endl;
                exit = true;
                break;
            case 7:
                if(isalnum(aux[i])) {
                    qActual = 7;
                    sentence+=aux[i];
                    }
                else if(aux[i] == ')') {
					cout <<"Entry estatus 7 if: "<<sentence<< endl;
                    qActual = 6;
                    variable.setName(sentence);
					sentence = "";
                     if(stackVariable->findData(variable) == nullptr) {
                        if(!validOnlyNumber(variable.getName())){
							listError->insertData("Error, la variable: '"+variable.getName()+"' no ha sido previamente declarada");
							}
                        }
					else{
						stackVariable->retrieveData(stackVariable->findData(variable)).setIsUsed(true);
						listVariableType->insertData(variable.getType());
						}
                    }
                else {
                    qActual = 10;
                    listError->insertData("Operando inv�lido, 7");
                    }
                break;
            case 9: // ///////// Valida en caso de ! //////////
                if(isalnum(aux[i])) {
                    qActual = 7;
                    sentence+=aux[i];
                    }
                else if(aux[i] == ')') {
                    qActual = 6;
                    variable.setName(sentence);
					sentence = "";
                     if(stackVariable->findData(variable) == nullptr) {
                        if(!validOnlyNumber(variable.getName())){
							listError->insertData("Error, la variable '"+variable.getName()+"' no ha sido previamente declarada");
							}
                        }
					else{
						stackVariable->retrieveData(stackVariable->findData(variable)).setIsUsed(true);
						listVariableType->insertData(variable.getType());
                    }
                    }
                else {
                    qActual = 10;
                    listError->insertData("Operando inv�lido, 9");
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
    /*else {
        while(!listError->isEmpty()) {
            cout <<listError->pop()<< endl;
            }
        }*/
	listVariableType->deleteAll();
    return --i;
    }

int AnalizadorLexico::analizeFOR(const std::string& aux, const int& auxInt) {
    Collection<char>* listCharacter = new Collection<char>;
    std::string auxError;
    std::string auxVariable = "";
    bool exit = false;
    int qActual = 0;
    bool bandera = false;
    int i = auxInt;
    listCharacter->push('�');
    do {
        switch(qActual) {
            case 0: /// Iniciar FOR con (
                if(aux[i] == '(') {
                    qActual = 1;
                    listCharacter->push(aux[i]);
                    }
                else {
                    auxError = "Falta un '('";
                    qActual = 7;
                    }
                break;
            case 1: /// Variable inicia con $.
                if(aux[i] == '$') {
                    listCharacter->push(aux[i]);
                    qActual = 2;
                    }
                else {
                    auxError = "La variable debe iniciar con $";
                    qActual = 7;
                    }
                break;
            case 2: /// Validar nombre de variable
                if(('A' <= aux[i] && aux[i] <= 'Z') || ('0' <= aux[i] && aux[i] <= '9')) {
                    listCharacter->push(aux[i]);
                    auxVariable += aux[i];
                    qActual = 2;
                    bandera = true;
                    }
                else if(aux[i] == '=' && bandera) { /// isVariable(auxVariable)
                    listCharacter->push(aux[i]);
                    qActual = 3;
                    auxVariable = "";
                    bandera = false;
                    }
                else {
                    qActual = 7;
                    auxError = "Operando invalido, 2";
                    }
                break;
            case 3:
                if('0' <= aux[i] && aux[i] <= '9') {
                    listCharacter->push(aux[i]);
                    qActual = 3;
                    bandera = true;
                    }
                else if(aux[i] == ';' && bandera) {
                    listCharacter->push(aux[i]);
                    qActual = 4;
                    bandera = false;
                    }
                else {
                    qActual = 7;
                    auxError = "Operando invalido, 3";
                    }
                break;
            case 4: // ///////////// Valida las letras despues del operador////////////
                if(('A' <= aux[i] && aux[i] <= 'Z') || ('0' <= aux[i] && aux[i] <= '9')) {
                    listCharacter->push(aux[i]);
                    auxVariable += aux[i];
                    qActual = 4;
                    bandera = true;
                    }
                else if((aux[i] == '<' || aux[i] == '>' || aux[i] == '&' || aux[i] == '!' || aux[i] == '=' || aux[i] == '|') && (bandera)) {
                    listCharacter->push(aux[i]);
                    qActual = 5;
                    auxVariable = "";
                    bandera = false;
                    }
                else {
                    qActual = 7;
                    auxError = "Operando invalido, 4";
                    }
                break;
            case 5: // /////////////// Valida los numeros despues del operador /////////
                if(('A' <= aux[i] && aux[i] <= 'Z') || ('0' <= aux[i] && aux[i] <= '9')) {
                    listCharacter->push(aux[i]);
                    qActual = 5;
                    bandera = true;
                    }
                else if(aux[i] == ')' && bandera) {
                    listCharacter->push(aux[i]);
                    auxVariable = "";
                    qActual = 6;
                    }
                else {
                    qActual = 7;
                    auxError = "Operando invalido, 5";
                    }

                break;
            case 6:
                cout <<"FOR V�lido"<< endl;
                exit = true;
                break;
            case 7:
                exit = true;
                listError->insertData("Error, sentencia FOR mal, " + auxError);
                break;
            default:
                break;
            }
        i++;
        }
    while(!exit);
    while(!listError->isEmpty()) {
        cout <<listError->pop()<< endl;
        }
    return --i;
    }

int AnalizadorLexico::analizeWHILE(const std::string& aux, const int& auxInt) {
    Collection<char>* listCharacter = new Collection<char>;
    Variable variable;
    string auxError, sentence;
    bool exit = false;
    int qActual = 0;
    int i = auxInt;
    listCharacter->push('�');
    do {
        switch(qActual) {
            case 0:
                if(aux[i] == '(') {
                    qActual = 1;
                    listCharacter->push(aux[i]);
                    }
                else {
                    listError->insertData("Error, falta un '(' en IF");
                    qActual = 10;
                    }
                break;
            case 1:
                if(aux[i] == '(') {
                    qActual = 1;
                    listCharacter->push(aux[i]);
                    }
                else if(isalnum(aux[i])) {
                    qActual = 2;
                    sentence=aux[i];
                    }
                else if(aux[i] == '!') {
                    qActual = 9;
                    sentence=aux[i];
                    }
                else {
                    qActual = 10;
                    auxError = aux[i];
                    listError->insertData("Error, caracter '"+auxError+"' no v�lido");
                    }
                break;
            case 2: // /////////////// Valida los n�meros antes del operador ////////
                //cout <<"q2"<< endl;
                if(isalnum(aux[i])) {
                    qActual = 2;
                    sentence+=aux[i];
                    }
                else if(aux[i] == '<' || aux[i] == '>' || aux[i] == '!' || aux[i]=='&' || aux[i] == '=') {
                    qActual = 4;
					variable.setName(sentence);
					sentence = "";
                     if(stackVariable->findData(variable) == nullptr) {
						if(!validOnlyNumber(variable.getName())){
							listError->insertData("Error, la variable '"+variable.getName()+"' no ha sido previamente declarada");
							}
                        }
					else{
						stackVariable->retrieveData(stackVariable->findData(variable)).setIsUsed(true);
						listVariableType->insertData(variable.getType());
                    }
                }
                else if(aux[i] == ')') {
                    qActual = 6;
                    variable.setName(sentence);
					sentence = "";
                     if(stackVariable->findData(variable) == nullptr) {
						if(!validOnlyNumber(variable.getName())){
							listError->insertData("Error, la variable '"+variable.getName()+"' no ha sido previamente declarada");
							}
                        }
					else{
						stackVariable->retrieveData(stackVariable->findData(variable)).setIsUsed(true);
						listVariableType->insertData(variable.getType());
                    }
                    }
                else {
                    qActual = 10;
					listError->insertData("Operando inv�lido, 2");
                    }
                break;
            case 4: /// ///////////// Valida las letras despues del operador////////////
                if(isalnum(aux[i])) {
                    qActual = 7;
                    sentence=aux[i];
                    }
                else {
                    qActual = 10;
                    listError->insertData("Operando inv�lido, 4");
                    }
                break;
            case 6:
                //cout <<"IF V�lido"<< endl;
                exit = true;
                break;
            case 7:
                if(isalnum(aux[i])) {
                    qActual = 7;
                    sentence+=aux[i];
                    }
                else if(aux[i] == ')') {
					cout <<"Entry estatus 7 if: "<<sentence<< endl;
                    qActual = 6;
                    variable.setName(sentence);
					sentence = "";
                     if(stackVariable->findData(variable) == nullptr) {
                        if(!validOnlyNumber(variable.getName())){
							listError->insertData("Error, la variable: '"+variable.getName()+"' no ha sido previamente declarada");
							}
                        }
					else{
						stackVariable->retrieveData(stackVariable->findData(variable)).setIsUsed(true);
						listVariableType->insertData(variable.getType());
						}
                    }
                else {
                    qActual = 10;
                    listError->insertData("Operando inv�lido, 7");
                    }
                break;
            case 9: // ///////// Valida en caso de ! //////////
                if(isalnum(aux[i])) {
                    qActual = 7;
                    sentence+=aux[i];
                    }
                else if(aux[i] == ')') {
                    qActual = 6;
                    variable.setName(sentence);
					sentence = "";
                     if(stackVariable->findData(variable) == nullptr) {
                        if(!validOnlyNumber(variable.getName())){
							listError->insertData("Error, la variable '"+variable.getName()+"' no ha sido previamente declarada");
							}
                        }
					else{
						stackVariable->retrieveData(stackVariable->findData(variable)).setIsUsed(true);
						listVariableType->insertData(variable.getType());
                    }
                    }
                else {
                    qActual = 10;
                    listError->insertData("Operando inv�lido, 9");
                    }
                break;
            case 10:
                exit = true;
                listError->insertData("Error, sentencia WHILE mal, " + auxError);
                break;
            default:
                break;
            }
        i++;
        }
    while(!exit);
    if(!listError->isEmpty()) {
        cout <<"WHILE Correcto"<< endl;
        }
    /*else {
        while(!listError->isEmpty()) {
            cout <<listError->pop()<< endl;
            }
        }*/
    return --i;
    }

bool AnalizadorLexico::existVariable(std::string& aux) {
    Variable variable;
    variable.setName(aux);
    if(stackVariable->findData(variable) != nullptr) {
        return true;
        }
    return false;
    }

bool AnalizadorLexico::isVariable(std::string& aux ) {
    bool bandera = false;
    for(int i = 0; i < aux.size(); i++) {
        if(isalnum(aux[i])) {
            bandera = true;
            }
        else{
            return false;
            }
        }
    return bandera;
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
        getline(readerFile,str1,'�');
        getline(readerFile,str2,'�');
        getline(readerFile,str3,'�');
        if(!readerFile.eof()) {
            lexico.setId(atoi(str1.c_str()));
            lexico.setName(str2);
            lexico.setDescription(str3);
            listData->insertData(lexico);
            }
        cout <<lexico.getName()<< endl;

        }
    }

bool AnalizadorLexico::validOnlyNumber(const std::string& aux) {
    bool isNumber = true;
    for(int i(0); i < aux.length(); i++){
		if(!isdigit(aux[i])){
			isNumber = false;
		}
    }
    return isNumber;
}
