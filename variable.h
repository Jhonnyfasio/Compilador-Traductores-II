#ifndef VARIABLE_H_INCLUDED
#define VARIABLE_H_INCLUDED

#include <iostream>
#include <cstring>

class Variable {
    private:
        int id;
        std::string name;
        std::string description;
        int type = 0;
        bool isUsed = false;

    public:
        int getId();
        std::string getName();
        std::string getDescription();
        int getType();
        bool getIsUsed();

        void setId(const int& idL);
        void setName(std::string& nameL);
        void setDescription(std::string& descriptionL);
        void setType(const int& typeX);
        void setIsUsed(const bool isUsedX);

        std::string toString();
};

#endif // VARIABLE_H_INCLUDED
