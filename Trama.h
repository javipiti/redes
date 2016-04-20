/*
 * Trama.h
 *
 *  Created on: 29 de feb. de 2016
 *      Author: Usuario
 */

#ifndef TRAMA_H_
#define TRAMA_H_
#include "windows.h"
#include "PuertoSerie.h"
#include <iostream>
using namespace std;

class Trama {
protected:
	unsigned char Sinc;
	unsigned char Dir;
	unsigned char Cont;
	unsigned char Num;
public:
	Trama();
	virtual ~Trama();
	virtual void enviarTrama(HANDLE PuertoCOM) =0;
};

 /* namespace std */

#endif /* TRAMA_H_ */
