/*
 * TramaDatos.h
 *
 *  Created on: 29 de feb. de 2016
 *      Author: Usuario
 */

#ifndef TRAMADATOS_H_
#define TRAMADATOS_H_

#include "Trama.h"
using namespace std;

class TramaDatos: public Trama {
protected:
	char Datos[255];
	unsigned char BCE;
	unsigned int longitud;
public:
	TramaDatos();
	virtual ~TramaDatos();
	unsigned char getBce() const;
	void setBce(unsigned char bce);
	const char* getDatos() const;
	void enviarTrama(HANDLE PuertoCOM);
	void setDato(char dato);
	void eliminarDato();
	char calcularBCE();
};

 /* namespace std */

#endif /* TRAMADATOS_H_ */
