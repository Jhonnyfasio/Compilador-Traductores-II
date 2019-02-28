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

void Variable::setId(const int& idL) {
    id = idL;
    }

void Variable::setName(std::string& nameL) {
    name = nameL;
}

void Variable::setDescription(string& descriptionL) {
    description = descriptionL;
    }

string Variable::toString() {
    /// Pendiente
    }
