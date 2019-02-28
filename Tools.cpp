#include "Tools.h"

using namespace std;

int Tools::toInt(const string& strToInt) {
	int toConvert;
	toConvert = atoi(strToInt.c_str());
	if ((0<toConvert || strToInt == "0") && isdigit(strToInt.c_str()[0])) {
		return toConvert;
	}
	return 0;
}

string Tools::strToCapLet(const string& toConvert) {
	string capitalLetter(toConvert);

	for (unsigned int i(0); i<toConvert.length(); i++) {
		capitalLetter[i] = toupper(toConvert[i]);
	}
	return capitalLetter;
}

bool Tools::toValidEmail(const string& toValid) {
	int counterArroba(0), counterDut(0);
	char mailX[100];
	bool arrobaFound(false);
	//strcpy(mailX,toValid.c_str());
	string newStr(toValid);

	for (unsigned int j = 0; j<(toValid.length()); j++) {
		mailX[j] = toValid[j];
	}

	if (toValid.length() >= 50) {
		cout << "Error, tu correo no puede superar los 60 caracteres" << endl;
		return false;
	}
	if (("A" <= newStr && newStr <= "Z") || ("a" <= newStr && newStr <= "z")) {

	}
	else {
		cout << "El correo debe iniciar con una letra" << endl;
		return false;
	}
	//cout <<strlen(mailX)<<endl;
	//cout <<toValid.length()<< endl;
	for (unsigned int i(0); i<toValid.length(); i++) {
		//cout <<"Letra: "<<mailX[i]<< endl;
		if (mailX[i] == ' ') {
			return false;
		}
		if (('a' <= mailX[i] && mailX[i] <= 'z') || mailX[i] == '@' ||
			mailX[i] == '-' || mailX[i] == '_' || mailX[i] == '.' || ('1' <= mailX[i] && mailX[i] <= '9')) {
		}
		else {
			cout << "No se admiten caracteres especiales" << endl;
			return false;
		}
		if (mailX[i] == '@' && counterArroba <= 1) {
			counterArroba++;
			arrobaFound = true;
			if (mailX[i - 1] == '-' || mailX[i - 1] == '_' || mailX[i - 1] == '.') {
				cout << "El correo no puede tener caracteres especiales antes del '@'" << endl;
				return false;
			}
			if (mailX[i + 1] == '-' || mailX[i + 1] == '_' || mailX[i + 1] == '.' || ('1' <= mailX[i + 1] && mailX[i + 1] <= '9')) {
				cout << "El correo no puede tener caracteres especiales después del '@'" << endl;
				return false;
			}
			if (mailX[i + 2] == ' ') {
				cout << "Dominio incorrecto" << endl;
				return false;
			}
		}
		if (counterArroba == 2) {
			cout << "Error, tu correo no puede tener más de 1 @" << endl;
			return false;
		}

		if (mailX[strlen(mailX)] == '.') {
			cout << "Erro de dominio" << endl;
			return false;
		}

		if (arrobaFound && mailX[i] == '.') {
			if (mailX[i + 1] == '.' || mailX[i + 2] == '.' || mailX[i + 5] == '.' || (mailX[i + 2] == mailX[toValid.length()])) {
				cout << "Dominio inválido." << endl;
				return false;
			}
		}


		if (arrobaFound && mailX[i] == '.' && counterDut <= 2) {
			counterDut++;
			if (counterDut == 1) {
				if (mailX[i + 5] == '.' || mailX[i + 2] == '.') {
					cout << "El dominio sólo puede ser de 2 o 3 caracteres" << endl;
					return false;
					break;
				}
				if (mailX[i + 2] == '.' || mailX[i + 2] == '\0') {
					cout << "El dominio debe tener mínimo 2 caracteres" << endl;
					return false;
					break;
				}
			}
			if (counterDut == 2) {
				if (mailX[i + 4] == '.') {
					cout << "Dominio incorrecto" << endl;
					return false;
					break;
				}
				/*
				if(mailX[i+1] != ' '){
				cout <<"1Dominio incorrecto"<< endl;
				return false;
				break;
				}*/
			}
			if (mailX[i - 1] == '-' || mailX[i - 1] == '_' || mailX[i - 1] == '.' || mailX[i - 1] == '@' || ('1' <= mailX[i + 1] && mailX[i + 1] <= '9')) {
				cout << "Tu dominio es incorrecto" << endl;
				return false;
				break;
			}
			if (mailX[i + 1] == '-' || mailX[i + 1] == '_' || mailX[i + 1] == '.' || mailX[i + 1] == '@' || ('1' <= mailX[i + 1] && mailX[i + 1] <= '9')) {
				cout << "Tu dominio es incorrecto" << endl;
				return false;
				break;
			}
		}

		//dominioCounter()

		//if(dominioFound && mailX[i])
	}
	if (counterArroba == 0) {
		cout << "Tu correo debe llevar un '@'" << endl;
		return false;
	}
	if (counterDut == 0) {
		cout << "Ingrese un dominio" << endl;
		return false;
	}
	return true;
}