/*
 * TramaDatos.cpp
 *
 *  Created on: 29 de feb. de 2016
 *      Author: Usuario
 */

#include "TramaDatos.h"

using namespace std;

TramaDatos::TramaDatos() :
		Trama() {
	Cont = 2;
	longitud = 0;
	BCE = 0;
}

TramaDatos::~TramaDatos() {
}

unsigned char TramaDatos::getBce() const {
	return BCE;
}

void TramaDatos::setBce(unsigned char bce) {
	BCE = bce;
}

const char* TramaDatos::getDatos() const {
	return Datos;
}

void TramaDatos::enviarTrama(HANDLE PuertoCOM) {
	setBce(calcularBCE());
	EnviarCaracter(PuertoCOM, Sinc);
	EnviarCaracter(PuertoCOM, Dir);
	EnviarCaracter(PuertoCOM, Cont);
	EnviarCaracter(PuertoCOM, Num);
	EnviarCaracter(PuertoCOM, BCE);
	EnviarCaracter(PuertoCOM, (char)longitud);
	EnviarCadena(PuertoCOM, Datos, longitud);
	//TODO ¿enviarCaracter o enviarCadena? Parece ser enviarCadena

}

void TramaDatos::setDato(char Dato) {

	Datos[longitud] = Dato;
	if (Dato != '\0')
		longitud++;
}

char TramaDatos::calcularBCE() {
	char bce = Datos[0];

	for (int i = 1; i<longitud;i++){
		bce = bce ^ Datos[i];
	}
	return bce;
}

/* namespace std */
