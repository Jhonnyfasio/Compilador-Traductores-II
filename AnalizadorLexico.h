#ifndef ANALIZADORLEXICO_H_INCLUDED
#define ANALIZADORLEXICO_H_INCLUDED

#include <iostream>
#include <fstream>
#include "collection.h"
#include "Lexico.h"
#include "Tools.h"
#include "variable.h"

class AnalizadorLexico{
    private:
        Tools* tool;
        Collection<Lexico>* listData;
        Collection<std::string>* listError;
        Collection<Variable>* stackVariable;
        std::string stringToAnalize;
        std::string finalString;
        std::string generalReport = "";
        std::string auxSintactico = "";
        int generalQ;
        int estadoActual = 0;
        bool isCorrect = true;
        bool isVariable(std::string& aux);

    public:
        AnalizadorLexico();
        std::string getStringToAnalize();

        void setStringToAnalize(std::string& aux);

        void receiveString();
        void analizeString();
        void analizeSintaticString();
        bool existVariable(std::string& aux);
        void analizeGeneral(std::string& aux);
        int analizeIF(const std::string& aux, const int& auxInt);
        int analizeFOR(const std::string& aux, const int& auxInt);
        int analizeWHILE(const std::string& aux, const int& auxInt);
        void analizeToken(const std::string& aux);

        void readFromDisk();
};

#endif // ANALIZADORLEXICO_H_INCLUDED
