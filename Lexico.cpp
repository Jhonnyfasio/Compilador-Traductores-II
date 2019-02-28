#include "lexico.h"

using namespace std;
 
int Lexico::getId() {
    return id;
    }

std::string Lexico::getName() {
    return name;
}

string Lexico::getDescription() {
    return description;
    }

void Lexico::setId(const int& idL) {
    id = idL;
    }

void Lexico::setName(std::string& nameL) {
    name = nameL;
}

void Lexico::setDescription(string& descriptionL) {
    description = descriptionL;
    }

string Lexico::toString() {
    /// Pendiente
    }
