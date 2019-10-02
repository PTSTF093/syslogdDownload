/*
 * CTxtCrptDecript.cpp
 *
 *  Created on: Nov 22, 2012
 *      Author: ptsca012
 */

#include "CTxtCrptDecript.h"

namespace std
{

CTxtCrptDecript::CTxtCrptDecript(string nomeFich)
{
	//criar apontador fp de ficheiro e
	//abrir o ficheiro
	//passado como string no
	//construtor
	ficheiro = nomeFich;
	string linha;
	ifstream fp;
	fp.open(nomeFich.c_str());
	//passar tudo para o vector
	while (fp.good())
	{
		//ler uma linha do ficheiro
		getline(fp, linha);
		registos.push_back(linha + '\n');
	} //a partir daqui trabalhar com o vector
	fp.close();
}

void CTxtCrptDecript::EncryptLine(string& allocator)
{
	unsigned ch;
	for (size_t i = 0; i < allocator.size(); ++i)
	{
		ch = allocator.at(i);
		std::bitset<8> x(ch);
		//cout << "ch=(" << x << ") ";
		if (ch != '\n') //nao mexer no byte='\n'
		{
			if (i % 2 == 0)
				ch = (char) ror(ch, 1);
			else
				ch = (char) rol(ch, 1);
		}
		allocator.at(i) = ch;
		std::bitset<8> y(ch);
		//cout << "ch:(" << y << ")" << endl;
	}
}

void CTxtCrptDecript::DecryptLine(string& allocator)
{
	unsigned ch;
	for (size_t i = 0; i < allocator.size(); ++i)
	{
		ch = allocator.at(i);
		std::bitset<8> x(ch);
		//cout << "ch=(" << x << ") ";
		if (ch != '\n')
		{
			if (i % 2 == 0)
				ch = (char) rol(ch, 1);
			else
				ch = (char) ror(ch, 1);
		}
		allocator.at(i) = ch;
		std::bitset<8> y(ch);
		//cout << "ch:(" << y << ")" << endl;
	}
}
void CTxtCrptDecript::DecryptLineNoNewLine(string& allocator)
{
	unsigned ch;
	for (size_t i = 0; i < allocator.size(); ++i)
	{
		ch = allocator.at(i);
		std::bitset<8> x(ch);
		//cout << "ch=(" << x << ") ";
		if (ch != '\n')
		{
			if (i % 2 == 0)
				ch = (char) rol(ch, 1);
			else
				ch = (char) ror(ch, 1);
			allocator.at(i) = ch;
		}else{
			allocator.erase(i);
		}

		std::bitset<8> y(ch);
		//cout << "ch:(" << y << ")" << endl;
	}
}

CTxtCrptDecript::~CTxtCrptDecript()
{
// TODO Auto-generated destructor stub
}

void CTxtCrptDecript::Encriptar()
{
//percorrer o vector,
//encriptando cada string
//e escrevendo no ficheiro.
	ofstream fp(ficheiro.c_str(), ios::binary);

	for (int i = 0; i < (int) registos.size(); ++i)
	{
		EncryptLine(registos.at(i));
		if (registos.at(i).at(0) != '\n')
			fp << registos.at(i); // << endl;
	}
	fp.close();

}

void CTxtCrptDecript::Decriptar()
{
//percorrer o vector,
//decriptando cada string
//e escrevendo no ficheiro.
	ofstream fp(ficheiro.c_str(), ios::binary);

	for (int i = 0; i < (int) registos.size(); ++i)
	{
		DecryptLine(registos.at(i));
		if (registos.at(i).at(0) != '\n')
			fp << registos.at(i); // << endl;
	}
	fp.close();
}

vector<string> std::CTxtCrptDecript::VirtualDecript()
{
	for (size_t i = 0; i < registos.size(); ++i)
	{
		DecryptLineNoNewLine(registos.at(i));
	}
	return registos;
}




} /* namespace std */
