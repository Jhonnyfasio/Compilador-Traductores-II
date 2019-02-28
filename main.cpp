#include <iostream>
#include "AnalizadorLexico.h"

using namespace std;

int main()
{
    setlocale(LC_CTYPE,"Spanish");
    AnalizadorLexico* analizador = new AnalizadorLexico();
    string str;
    while(1){
        cout <<"Diga la cadena: ";
        getline(cin, str);
        analizador->setStringToAnalize(str);
        analizador->analizeString();
    }
    return 0;
}
