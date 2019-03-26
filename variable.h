#ifndef VARIABLE_H_INCLUDED
#define VARIABLE_H_INCLUDED

#include <iostream>
#include <cstring>

class Variable {
    private:
        int id;
        std::string name;
        std::string description;
        int type;
        
    public:
        int getId();
        std::string getName();
        std::string getDescription();
        int getType();
        
        void setId(const int& idL);
        void setName(std::string& nameL);
        void setDescription(std::string& descriptionL);
        void setType(const int& typeX);
        
        std::string toString();
};

#endif // VARIABLE_H_INCLUDED
