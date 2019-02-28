#ifndef LEXICO_H_INCLUDED
#define LEXICO_H_INCLUDED

#include <iostream>
#include <cstring>

class Lexico {
    private:
        int id;
        std::string name;
        std::string description;
        
    public:
        int getId();
        std::string getName();
        std::string getDescription();
        
        void setId(const int& idL);
        void setName(std::string& nameL);
        void setDescription(std::string& descriptionL);
        
        std::string toString();
};

#endif // LEXICO_H_INCLUDED
