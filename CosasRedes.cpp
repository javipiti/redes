/*
 *
 * CosasRedes.cpp
 *
 *  Created on: 24 de feb. de 2016
 *      Author: Usuario
 */

#include "CosasRedes.h"

CosasRedes::CosasRedes() {
	carRec = 0;
	car = 0;
	cuantos = 0;
	modoFichero = false;
}

bool CosasRedes::ejecutar(HANDLE PuertoCOM) {
	PuertoCOM = AbrirPuerto("COM1", 9600, 8, 0, 0); //Abrimos el puerto COM1 (en la sala siempre abrimos el COM1)
	if (PuertoCOM == NULL) {
		printf("Error al abrir el puerto\n");
		getch();
		return false;
	}
	bool salir = false;
	while (salir == false) {
		recibirAlgo(PuertoCOM);
		if (kbhit())
			salir = enviarAlgo(PuertoCOM);
	}
	CerrarPuerto(PuertoCOM);
	return true;
}

bool CosasRedes::enviarAlgo(HANDLE PuertoCOM) {
	car = getch();
	int queTrama = 0;
	if (cuantos < 600 || car == '\0' || car == 8 || car == 27) { //Comprobacion de tamaño max
		switch (car) { //4
		case 27:
			return true;
			break;
		case '\0':
			car = getch();
			if (car == 59) {
				TramaDatos *datosEnv = new TramaDatos();
				for (int i = 0; i < cuantos; i++) {
					if (i % 254 == 0) //Comienzo nueva trama
							{
						delete datosEnv;
						datosEnv = new TramaDatos();
					}
					datosEnv->setDato(cadenaEnviar[i]);
					if (i % 254 == 253 || i == cuantos - 1)	//Fin trama o fin cadena
							{
						datosEnv->setDato('\0');
						datosEnv->enviarTrama(PuertoCOM);
						recibirAlgo(PuertoCOM);
					}
				}
				cuantos = 0;
				cout << endl;
			}
			if (car == 60) {

				cout << "Introduzca el tipo de trama de control a enviar:"
						<< endl;
				cout << "1. Trama ENQ" << endl;
				cout << "2. Trama EOT" << endl;
				cout << "3. Trama ACK" << endl;
				cout << "4. Trama NACK" << endl;

				TramaControl *tramaEnv;
				do {
					cin >> queTrama;

					switch (queTrama) {

					case 1:
						tramaEnv = new TramaControl(5);
						cout << "Se ha enviado una trama ENQ" << endl;
						break;
					case 2:
						tramaEnv = new TramaControl(4);
						cout << "Se ha enviado una trama EOT" << endl;
						break;
					case 3:
						tramaEnv = new TramaControl(6);
						cout << "Se ha enviado una trama ACK" << endl;
						break;
					case 4:
						tramaEnv = new TramaControl(21);
						cout << "Se ha enviado una trama NACK" << endl;
						break;
					default:
						cout
								<< "Valor no válido. Por favor, introduzca un valor entre 1 y 4."
								<< endl;
						break;
					}
				} while (queTrama > 4 || queTrama < 1);

				tramaEnv->enviarTrama(PuertoCOM);
			}
			if (car == 61) {
				EnviarFichero(PuertoCOM);
			}
			break;
		case 13:
			cout << endl;
			cadenaEnviar[cuantos] = '\n';
			cuantos++;
			break;
		case 8:
			cout << "\b \b";
			cuantos--;
			cadenaEnviar[cuantos] = 0;
			break;
		default:
			// COMPROBACION DE SOLO ALFANUMERICOS Y ESPACIOS
			if (car >= 32 && car <= 126) {	     //6
				cout << car;
				cadenaEnviar[cuantos] = car;
				cuantos++;
			}
			break;

		}
	}
	return false;
}
void CosasRedes::EnviarFichero(HANDLE PuertoCOM) {
	//TODO : enviar inicio de fichero
	ifstream lectura;
	lectura.open("frc.txt");
	char lect[255];
	int contador = 0;
	TramaDatos *tramaLec = new TramaDatos();
	//Al abrir el fichero enviamos el caracter suprimir para indicarlo
	EnviarCaracter(PuertoCOM, 127);
	if (lectura.is_open() == true) {
		while (lectura.eof() == false) {
			recibirAlgo(PuertoCOM);
			lectura.read(lect, 254);
			contador = lectura.gcount();
			for (int i = 0; i < contador; i++) {
				tramaLec->setDato(lect[0]);
			}
			lect[contador] = '\0';
			tramaLec->enviarTrama(PuertoCOM);
			delete tramaLec;
			tramaLec = new TramaDatos();
			contador = 0;
		}
	}
	//Cuando el fichero acaba enviamos el caracter retroceso para indicarlo
	EnviarCaracter(PuertoCOM, 8);
	lectura.close();
}

void CosasRedes::recibirAlgo(HANDLE PuertoCOM) {
	unsigned char longitud;
	carRec = RecibirCaracter(PuertoCOM);
	unsigned char BCE;
	unsigned char BCEt; //Para control
	if (carRec == 22) //ESTAMOS RECIBIENDO TRAMA
			{
		unsigned char tipo;
		carRec = 0;
		while (carRec == 0) {
			carRec = RecibirCaracter(PuertoCOM); //Direccion
		}
		if (carRec != 'T')
			cout << "ERROR en Dir";
		carRec = 0;
		while (carRec == 0) {
			carRec = RecibirCaracter(PuertoCOM);
		}
		tipo = carRec; //Control
		carRec = 0;

		while (carRec == 0) {
			carRec = RecibirCaracter(PuertoCOM);
		}
		if (carRec != '0')
			cout << "Error en Num";
		carRec = 0;

		switch (tipo) {
		case 2:
			while (carRec == 0) {
				carRec = RecibirCaracter(PuertoCOM);
			}
			BCE = carRec;
			carRec = 0;
			while (carRec == 0) {
				carRec = RecibirCaracter(PuertoCOM);
			}
			longitud = carRec;
			char Datos[255];
			RecibirCadena(PuertoCOM, Datos, (int) longitud);
			BCEt = Datos[0];
			for (int i = 1; i < longitud; i++) {
				BCEt = BCEt ^ Datos[i];

			}

			if (BCE == BCEt) {
				if (modoFichero == false) {
					cout
							<< "Se ha recibido una trama de datos. Su contenido es:"
							<< endl;
					for (int i = 0; i < longitud; i++) {
						cout << Datos[i];
					}
				} else
					ficheroSal.write(Datos, longitud);
			} else
				cout << "La trama no es correcta" << endl;
			break;
		case 4:
			cout << "Se ha recibido una trama EOT" << endl;
			break;
		case 5:
			cout << "Se ha recibido una trama ENQ" << endl;
			break;
		case 6:
			cout << "Se ha recibido una trama ACK" << endl;
			break;
		case 21:
			cout << "Se ha recibido una trama NACK" << endl;
			break;
		default:
			cout << "Error al recibir tipo de trama" << endl;
			break;
		}
	} else if (carRec == 8) {
		cout<<"Fichero enviado."<<endl;
		modoFichero = false;
		ficheroSal.close();
	} else if (carRec == 127) {
		cout<<"Enviando fichero..."<<endl;
		modoFichero = true;
		ficheroSal.open("frcR.txt");
	} else
		cout << carRec;
}

CosasRedes::~CosasRedes() {

}
