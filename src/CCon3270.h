/*  
 * File:   CCon3270.h
 * Author: marlon.simoni
 *
 * Created on 16 de Março de 2014, 13:32
 */

#ifndef CCON3270_H
#define	CCON3270_H

#include <iostream>
#include <iomanip>
#include <sstream>
//#include <fstream>
#include <tcl.h>
//#include <expect_comm.h>
#include <expect_tcl.h>
#include <expect.h> //funcionalidade de interaccao
#include <vector>
#include <string>
#include <algorithm>

#include <cstdlib>
#include <cstring>
#include <ctime>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include <stdexcept>

#include <ctype.h>

#include "CTxtCrptDecript.h"

#define DBPS "openpipes" //password root para mysql
//#define DBCGD "openpipes" //password root para mysql

#define YYIBM01 true


#ifndef YYIBM01 //ptstf093
#define PIPESPASS "/home/ptstf093/pipespass"
#define PIPESPASSCEMG "/home/ptstf093/pipespasscemg"
#else //cgdibm02
#define PIPESPASS "/home/cgdibm02/pipespass"
#define PIPESPASSCEMG "/home/cgdibm02/pipespasscemg"
#endif
//todo definir s3270 ou x3270

#ifndef YYIBM01 //ptstf093
#define PROXY "/usr/bin/s3270 -proxy socks5:socks.lsb.esni.ibm.com:1080 "// 192.168.90.17"
//#define PROXY "/usr/bin/x3270 -script -model 3278-2 -proxy socks5:socks.lsb.esni.ibm.com:1080 " //10.200.101.20"
#else //cgdibm02
#define PROXY "/usr/bin/s3270 -proxy socks5:socks.lsb.esni.ibm.com:1080 " //10.200.101.20"
#endif

//#define PROXY "s3270 -script 172.16.32.253" //mvsa-vs

using namespace std;

//namespace CCon3270{

class CCon3270 {
protected:
	FILE *fpipe;
	Tcl_Interp *interp;
	string user;
	string pass;
	string netview;
	string lpar;
	bool started;

	string usercgd; //igual ao user da mira
	string passcgd; //igual ao user da cgd (a1,b1,b2,etc...)
	string usermcf;
	string passmcf;
	string userbcg;
	string passbcg;
	string coordenadas;//reter as coordenadas para sempre que se desejar depurar saber
	//quais as coordenadas passadas e cujo resultado foi o inexperado.

public:

	CCon3270(const CCon3270& orig);
	virtual ~CCon3270();
public:

	CCon3270() ;

	virtual void Start(string lpar);
	virtual void Start(string lpar, unsigned timeout) ;
	//virtual void OpenPassword();//??? talvez retirar metodo e deixar para outra classe abaixo

	virtual bool Executar3270(string str);//Executar instrucao/comando 3270
	virtual bool EnterString(string str);//escrever string e dar enter
	virtual bool PutString(string str);//por a string sem dar enter
	virtual bool Ok();
	virtual void Informar(string info);//Informar msg passada por string em info
	virtual void Informa();//so o timestamp
	virtual string Informar();//retornar em string o Informar()
	virtual string GetTimestamp();
	virtual void ShowThisScreen();
	virtual string GetStrThisScreen();

	virtual vector<string> GrabScreen() ;

	virtual void PrintTimestamp();
	virtual string GetStrTimestamp();
	virtual string GetStrTimestamp_Maria();

protected:
	string CharClear(string str);

public:

	virtual void LimparBuffer();
	virtual bool Apareceu(string, int, int);//se apareceu @ linha coluna.
	virtual bool Apareceu(string str,int tentativas);//se apareceu no ecrã
	virtual bool SeApareceu(string str);//se apareceu sem dar erro de retorno
	virtual void WaitInputReady();
	virtual bool WaitOutput();

	/*
	 pfkeys e teclas chave de 3270
	 */
	virtual void Home() ;
	virtual void Clear();
	virtual void Pf8() ;
	virtual void Pf9() ;
	virtual void Pf7() ;
	virtual void Pf3() ;
	virtual void Pf10() ;
	virtual void Pf11() ;
	virtual void Pf24() ;
	virtual void Pa2() ;
	virtual void NewLine();
	virtual void NewLine(int times);
	virtual void Eeof();
	virtual void EraseInput();
	virtual void Tab() ;
	virtual void Enter() ;
	virtual void Mpf8() ;
	virtual void Mpf7() ;
	virtual bool MoverCursor(int linha, int coluna);
	//virtual void GetUserPass();
	virtual string GetUser() {
		return user;
	}
	virtual string GetPass() {
		return pass;
	}
	virtual string Capturar(string, string, string);
	virtual string Capturar(int, int, int);
	static int StringToInt(string);
	static string IntToString(int);
	virtual void SetUserPass(string u, string p);

	virtual void Stop();//desligar a emulação
	virtual void LogToFile(string f,string message);

	virtual string GetStrPromtState();//devolver em string algo como U F U C(192.168.90.17) I 4 24 80 4 0 0x0 0.002
	virtual void SetDebug(bool a){exp_loguser=a;};
	virtual bool IsLocked();
};

//}//end namespace CCon3270

#endif	/* CCON3270_H */

