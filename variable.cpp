#include "variable.h"

using namespace std;

int Variable::getId() {
    return id;
    }

std::string Variable::getName() {
    return name;
}

string Variable::getDescription() {
    return description;
    }

int Variable::getType(){
    return type;
    }

bool Variable::getIsUsed() {
	return isUsed;
}

void Variable::setId(const int& idL) {
    id = idL;
    }

void Variable::setName(std::string& nameL) {
    name = nameL;
}

void Variable::setDescription(string& descriptionL) {
    description = descriptionL;
    }

void Variable::setType(const int& typeX){
    type = typeX;
    }

void Variable::setIsUsed(const bool isUsedX)
{
	isUsed = isUsedX;
}

string Variable::toString() {
    /// Pendiente
    }
