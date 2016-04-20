/*
 * TramaControl.h
 *
 *  Created on: 24 de feb. de 2016
 *      Author: Usuario
 */

#ifndef TRAMACONTROL_H_
#define TRAMACONTROL_H_
#include "Trama.h"

class TramaControl: public Trama {

public:
	TramaControl();
	TramaControl(int Control);
	~TramaControl();

	//Envia la trama
	void enviarTrama(HANDLE PuertoCOM);
	unsigned char getCont() const;
	void setCont(unsigned char cont);
	unsigned char getDir() const;
	void setDir(unsigned char dir);
	unsigned char getNum() const;
	void setNum(unsigned char num);
	unsigned char getSinc() const;
	void setSinc(unsigned char sinc);
};

#endif /* TRAMACONTROL_H_ */
