/*
 * CTxtCrptDecript.h
 *
 *  Created on: Nov 22, 2012
 *      Author: ptsca012
 *
 *  instalar bmagic pelo synaptic
 */

#ifndef CTXTCRPTDECRIPT_H_
#define CTXTCRPTDECRIPT_H_

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <bitset>

namespace std
{

class CTxtCrptDecript
{
	string ficheiro;
	//encripta a string passada
	void EncryptLine(string&);
	//decripta a string passada
	void DecryptLine(string&);
	void DecryptLineNoNewLine(string&);

	unsigned ror(unsigned Data, unsigned int Bits)
	{
		if(Data=='\n')
			return '\n';
		unsigned long mascara = 0x000000ff;
		bitset<32> m1(mascara);
		mascara = mascara & Data;
		bitset<32> m2(mascara);
		bitset<8> x(Data);
		bitset<8> y(Data >> Bits);
		bitset<8> z(mascara << (8 - Bits));
		bitset<8> w((Data >> Bits) | (mascara << (8 - Bits)));
//		cout << endl;
//		cout << "mascara1 :"<<m1<<" | ";
//		cout << "mascara2 :"<<m2<<" | ";
//		cout << "Data :" << x << " | ";
//		cout << "Data >> Bits :" << y << " | ";
//		cout << "mascara << (8 - Bits)) :" << z << " | ";
//		cout << "(mascara >> Bits) | (Data << (8-Bits)) :" << w << endl;
		return ((mascara >> Bits) | (Data << (8 - Bits)));
	}

	char rol(char Data, unsigned int Bits)
	{
		if(Data=='\n')
			return '\n';
		//return ((Data << Bits) | (Data >> (8 - Bits)));
		//unsigned long mascara = 33023; //8000h = 1000 0000 0000 0000
		unsigned long mascara = 0x000000ff;
		bitset<32> m1(mascara);
		mascara = mascara & Data;
		bitset<32> m2(mascara);
		bitset<8> x(Data);
		bitset<8> y(Data << Bits);
		bitset<8> z(Data >> (8 - Bits));
		bitset<8> w((Data << Bits) | (mascara >> (8 - Bits)));
//		cout << endl;
//		cout << "mascara1 :"<<m1<<" | ";
//		cout << "mascara2 :"<<m2<<" | ";
//		cout << "Data :" << x << " | ";
//		cout << "Data << Bits :" << y << " | ";
//		cout << "mascara >> (8 - Bits)) :" << z << " | ";
//		cout << "(Data << Bits) | (mascara >> (8-Bits)) :" << w << endl;

		//está a levar 1 ae ao fundo!!! 1000000 -> 11111111
		return (Data << Bits) | (mascara >> (8 - Bits));
	}

	//registos do ficheiro.
	vector<string> registos;
public:
	CTxtCrptDecript(string nomeFich);
	virtual ~CTxtCrptDecript();
	void Encriptar();
	void Decriptar();
	vector<string> VirtualDecript();
};

} /* namespace std */
#endif /* CTXTCRPTDECRIPT_H_ */
