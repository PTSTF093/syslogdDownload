/* 
 * File:   CMenu.cpp
 * Author: marlon.simoni
 * 
 * Created on 8 de Abril de 2014, 15:58
 */

#include "CMenu.h"

CMenu::CMenu() {
	//cout << PIPESPASS << endl;
	time(&Antes);
	time(&Agora);
	sair = false;
	LogedNetView = false; //esta logado no netview
	LoopRec = false;
	tso = lpar = "";
	//DBStart(); //arrancar o mysql
	//Cftsm::mysql = CGrt::mysql = CGspi::mysql = Ccutoffs::mysql = CGprtAnom::mysql = CGprtRec::mysql = mysql;
	//mysql.Exec("update run set parar=0");
	//OpenPassword();
}

CMenu::CMenu(string scemg) {
	cout << PIPESPASS << endl;
	time(&Antes);
	time(&Agora);
	sair = false;
	LogedNetView = false; //esta logado no netview
	LoopRec = false;
	tso = lpar = "";

	//Cftsm::mysql = CGrt::mysql = CGspi::mysql = Ccutoffs::mysql = CGprtAnom::mysql = CGprtRec::mysql = mysql;
	//mysql.Exec("update run set parar=0");
	OpenPassword();
}


CMenu::CMenu(const CMenu& orig) {
	time(&Antes);
	time(&Agora);
	sair = false;
	tso = lpar = "";
	LogedNetView = false; //esta logado no netview
	LoopRec = false;
	OpenPassword();
}

CMenu::~CMenu() {
}

void CMenu::PrintMenu() {
	/*
	 despejar um menu com opcoes e recolher num prompt*/
	cout << endl;
	cout << "****       Motor-Opipes  **** TSO:" << tso << " ****" << endl;
	if (!LogedNetView) {
		cout << "    1. Login NetView" << endl;
	} else {
		if (tso == "") {
			cout << "    2. Login TSO SYA1" << endl;
			cout << "    3. Login TSO SYB1" << endl;
			cout << "    4. Login TSO SYB2" << endl;
			cout << "    5. Login TSO MCFA" << endl;
			cout << "    6. Login TSO MCFB" << endl;
			cout << "    7. Login TSO MVS2" << endl;
		} else {
			cout << "    8. Logoff: " << tso << endl;
			//cout << "   10. Start Pipes: "<< tso << endl;
		}
	}
	cout << "    9. ShowScreen(Debug)" << endl;
	cout << "    X. Sair" << endl;
	cout << "opipes > ";
	cin >> comando;
}


void CMenu::LogoutAll() {

}


void CMenu::Logoff() {
	int bkp_exp_timeout = exp_timeout, rc;	//salvaguardar o exp_timeout default
	exp_timeout = 2;
	cout << "...logoff " << lpar << " .";
	Pf3();
	while (1) {
		cout << ".";
//		sleep(1);
//		Home();Eeof();EnterString("=x");Pf3();
//		EnterString("logoff");
//		sleep(1);

		fprintf(fpipe, "ascii\r");
		rc = exp_fexpectl(fpipe, exp_glob,
				" READY                                 ", 1, exp_glob,
				"CAIXA GERAL DE DEPOSITOS, S.A.", 2, exp_glob, "IKJ56247I", 3,
				exp_glob, "Specify Disposition of Log Data Set", 4, exp_glob,
				"UNSUPPORTED FUNCTION", 5, exp_glob,
				"PONER CODIGO DE APLICACION Y PULSAR INTRO", 6, exp_glob,
				"IKJ56247I", 7, exp_glob,
				"C A I X A   E C O N O M I C A   M O N T E P I O   G E R A L",
				8,
				exp_glob,"FUNCAO NAO SUPORTADA",9,
				exp_glob,"Specify Disposition of Log Data Set",10,

				/******/
				exp_end);
		switch (rc) {
		//case 0:

		case 1:

			cout << "apareceu READY" << endl;
			EnterString("logoff");
			cout << "dado logoff" << endl;
			continue;
			break;
		case 9:
		case 5:
			Clear();
			continue;
			break;
		case 6:
		case 2:
		case 8:
			//sucesso
			cout << "logoff com sucesso" << endl;
			exp_timeout = bkp_exp_timeout;
			cout << ".";
			sleep(5);
			Stop();
			sleep(5);
			cout << ".feito stop";

			return;
			break;
		case 3:
			Enter();
			continue;
			break;
		case 4:
		case 7:
			Enter();
			continue;
			break;
		case 10:
			EnterString("2");
			continue;
			break;
		case EXP_TIMEOUT:
			//cout<<"timeout, continuar loop"<<endl;
			Home();
			Eeof();
			Pf3();
			//cout<<"dado Home();Eeof();Pf3();"<<endl;
			continue;
			break;
		}


	}

}


bool CMenu::setTso(string stso) {

	tso = stso;
	/*
	 * implementar o processo todo de entrar no tso pe: TSOSYA1, pelo nome não pelo número
	 */

	return true;
}

bool CMenu::OpenPassword() {
	CTxtCrptDecript *ficheiro;
	vector<string> regs;
	ficheiro = new CTxtCrptDecript(PIPESPASS);
	regs = ficheiro->VirtualDecript();
	for (size_t i = 0; i < regs.size(); ++i) {
		switch (i) {
		case 0:
			usercgd = regs.at(i);
			Informar("usercgd=" + usercgd);
			break;
		case 1:
			passcgd = regs.at(i);
			break;
		case 2:
			usermcf = regs.at(i);
			Informar("usermcf=" + usermcf);
			break;
		case 3:
			passmcf = regs.at(i);
			break;
		case 4:
			userbcg = regs.at(i);
			Informar("userbcg=" + userbcg);
			break;
		case 5:
			passbcg = regs.at(i);
			break;
		}
	}
	ficheiro = new CTxtCrptDecript(PIPESPASSCEMG);
	regs = ficheiro->VirtualDecript();
	for (size_t i = 0; i < regs.size(); ++i) {
		switch (i) {
		case 0:
			usertsop = regs.at(i);
			Informar("usertsop=" + usertsop);
			break;
		case 1:
			passtsop = regs.at(i);
			break;
		case 2:
			usertsoc = regs.at(i);
			Informar("usertsoc=" + usertsoc);
			break;
		case 3:
			passtsoc = regs.at(i);
			break;
		case 4:
			usertsod = regs.at(i);
			Informar("usertsod=" + usertsod);
			break;
		case 5:
			passtsod = regs.at(i);
			break;
		}
	}
	return true;
}

bool CMenu::ElapsedTime(time_t t) {
	//devolver TRUE se t for maior que o difftime entre Agora e Antes
	time(&Agora);
	if (difftime(Agora, Antes) > t) {
		//reset ao timer
		Antes = Agora;
		return true;
	}
	return false;

}

bool CMenu::ArrancarMG1P(string user,string pass,string data, string dias) {
	//AADFS();
	//hoje=$(date +"%Y-%m-%d")

	lpar = "MG1P";

	Start(lpar);
	setTso("TSOP");
	LoginTso(tso, user, pass);



	Home();
	Eeof();
	EnterString("start 6");

	/*
	 * Transferir o ficheiro.
	 * #	"HostFile=pcsyspl.tds.mgtdsj09.pln"
	 * #	"LocalFile=/home/cgdibm02/tws/pcsyspl.tds.mgtdsj09.pln.$hoje.txt"
	 */
	/*
	 * #data em número de dias desde EPOCH=1970
		segundos=`date +"%s"`
		dias=$(( $segundos / 3600 / 24 ))
	 *	18171 => 1006
	 * Constante a subtrair é 17165
	 */
	int intDias=StringToInt(dias);
	const int Constante=17165;
	int numero=intDias-Constante;
	string sufixo="G"+IntToString(numero)+"V00";
	string HostFile="SYP.D.STSY902G.SYSLOGD.DIAG."+sufixo;
	string LocalFile="/home/cgdibm02/SYSLOGD_DIAG/"+HostFile;

	Executar3270("Transfer HostFile="+HostFile+" LocalFile="+LocalFile);


	return true;
}


bool CMenu::SessaoCaiuLocked(string u, string p) {
	int bk = exp_timeout;
	exp_timeout = 2;
	fprintf(fpipe, "ascii\r");
	int rc = exp_fexpectl(fpipe, exp_glob, "INDIQUE A OPÇÃO PRETENDIDA", 0,
			exp_glob, "INVALID COMMAND SINTAX", 1, exp_glob,
			"COMMAND UNRECOGNIZED", 2, exp_glob, "IVALID SWAP COMMAND SINTAX",
			3, exp_glob, "UNSUPORTED FUNCTION", 4, exp_glob,
			"COMANDO SWAP NO RECONOCIDO", 5, exp_glob,
			"C A I X A   E C O N O M I C A   M O N T E P I O   G E R A L", 6,


			exp_glob,"FUNCAO NAO SUPORTADA",9,
			exp_glob,"Specify Disposition of Log Data Set",10,
			exp_glob,"ISPF processor ended abnormally",11,
//				exp_glob,"",1,
//				exp_glob,"",1,
//				exp_glob,"",1,


			exp_end);
	if( rc == 10){
		EnterString("2");
	}
	if (rc == EXP_TIMEOUT) {
		Ok();
	}


	exp_timeout = bk;
	return false;
}

void CMenu::Logoff(int int1) {
	int bkp_exp_timeout = exp_timeout, rc;	//salvaguardar o exp_timeout default
	exp_timeout = int1;
	cout << endl << "...logoff " << lpar << " .";
	Pf3();
	LimparBuffer();
	while (1) {
		cout << ".";
//		sleep(1);
//		Home();Eeof();EnterString("=x");Pf3();
//		EnterString("logoff");
//		sleep(1);
		LimparBuffer();
		Pf3();
		fprintf(fpipe, "ascii\r");
		rc = exp_fexpectl(fpipe, exp_glob,
				" READY                                 ", 1, exp_glob,
				"CAIXA GERAL DE DEPOSITOS, S.A.", 2, exp_glob, "IKJ56247I", 3,
				exp_glob, "Specify Disposition of Log Data Set", 4, exp_glob,
				"UNSUPPORTED FUNCTION", 5, exp_glob,
				"PONER CODIGO DE APLICACION Y PULSAR INTRO", 6, exp_glob,
				"IKJ56247I", 7, exp_glob, "SDSF DA MVS2", 8, exp_glob,
				"SDSF PRIMARY OPTION MENU", 9, exp_glob,
				"ISPF Primary Option Menu", 10, exp_glob,
				"BROWSING OPERATIONS ENDED IN ERROR", 11, exp_glob,
				"SPECIFYING ENDED IN ERROR LIST CRITERIA", 12, exp_glob,
				"TIVOLI WORKLOAD SCHEDULER FOR Z/OS", 13, exp_glob,
				"CO Control-D", 14, exp_glob, "BROWSING ACTIVE CRITICAL JOBS",
				15, exp_glob, "DEFINING TWSz PARAMETERS AND OPTIONS", 16,
				exp_glob, "TWSz CONTROLLERS AND SERVER LU NAMES", 17, exp_glob,
				"CURRENT PLAN AND STATUS INQUIRY", 18,
//				exp_glob,"",19,
//				exp_glob,"",20,
//				exp_glob,"",21,
//				exp_glob,"",22,
//				exp_glob,"",23,
//				exp_glob,"",24,
//				exp_glob,"",25,
//				exp_glob,"",26,
//				exp_glob,"",27,
//				exp_glob,"",28,
//				exp_glob,"",29,
//				exp_glob,"",30,
//				exp_glob,"",31,
//				exp_glob,"",32,

				exp_end);
		switch (rc) {
		//case 0:

		case 1:

			cout << "apareceu READY" << endl;
			EnterString("logoff");
			cout << "dado logoff" << endl;
			continue;
			break;
		case 5:
			Clear();
			continue;
			break;
		case 6:
		case 2:
			//sucesso
			cout << "logoff com sucesso" << endl;
			exp_timeout = bkp_exp_timeout;
			cout << ".";
			sleep(5);
			Stop();
			sleep(5);
			cout << ".feito stop";


			return;
			break;
		case 3:
			Enter();
			continue;
			break;
		case 4:
		case 7:
			Enter();
			continue;
			break;
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
		case 17:
		case 18:
		case EXP_TIMEOUT:
			//cout<<"timeout, continuar loop"<<endl;
			cout << ".";
			Home();
			Eeof();
			Pf3();
			//cout<<"dado Home();Eeof();Pf3();"<<endl;
			continue;
			break;
		}

	}

}
//void CMenu::Ftsm() {
//    //
//    if (solicitado_stop()) return;
//    cout<<endl<<".FTSM.";
//    Cftsm::EstadoOn();cout << ".";
//    Cftsm::LimpaTabTemp();cout << ".";
//    Cftsm::GetCics();cout << ".";
//    Cftsm::ActTempIntoOrig();cout << ".";
//    Cftsm::EstadoOff();cout << ".";
//}


bool CMenu::LogonApplid(string applid, string u, string p) {
	int bkp_exp_timeout = exp_timeout, rc;	//salvaguardar o exp_timeout default

	exp_timeout = 30;
	//Pa2();
	//MoverCursor(22, 13);
	//Eeof();
	sleep(2);//translation table
	//tsoname = applid;
	EnterString("logon applid(" + applid + ")");
	sleep(1);
	//IKJ56700A ENTER USERID - meter user
	cout << endl << ".logon applid()." << endl;

	//a partir daqui existem diversas possibilidades
	/*
	 * login puro, entrando com user e password até cair no painel do ispf
	 * login parcial onde falta introduzir o user / ou password / ou dar enter no ***
	 * login já completo mas com ecrã inexperado no tso
	 */
	while (1) {
		sleep(1);
		fprintf(fpipe, "ascii\r");
		sleep(1);
		rc =
				exp_fexpectl(fpipe, exp_glob, "EMS1334E",
						0, //mau, refazer logon
						exp_glob, "IKJ56710I", 1, exp_glob,
						"IKJ56703A REENTER THIS OPERAND", 2, exp_glob,
						"EMS1112E",
						3, //relogin refresh screen
						exp_glob,
						"IKJ56421I PASSWORD NOT AUTHORIZED FOR USERID", 4,
						exp_glob, "\r\ndata:  \\*\\*\\*                 ", 5,
						exp_glob, "ISPF Primary Option Menu",
						6, //boa, ja ca estamos.
						exp_glob, "IKJ56700A ENTER USERID",
						8, //enviar userid
						exp_glob, "Enter LOGON parameters below:", 9, exp_glob,
						"Application Selection", 10, exp_glob,
						"HANDLING OPERATIONS ENDED IN ERROR", 11, exp_glob,
						"SPECIFYING ENDED IN ERROR LIST", 12, exp_glob,
						"COMMAND INPUT ===>", 13, exp_glob, "READY LIST", 14,
						exp_glob, "IKJ56951I NO BROADCAST MESSAGES", 15,
						exp_glob, "OPERATOR INSTRUCTION", 16, exp_glob,
						"IKT00300I LOGON RECONNECT SUCCESSFUL", 17, exp_glob,
						"BROWSING OPERATIONS", 18, exp_glob,
						"HO006DA ----------------- SELECTING ALL DEPENDENCIES",
						19, exp_glob,
						"HO006DA -------- CONFIRMING INVOCATION OF THE ALL", 20,
						exp_glob,
						"HO006DA -------- ALL DEPENDENCIES OF AN OPERATION", 21,
						exp_glob, "IKJ56420I Userid", 22, exp_glob, "EMSP01",
						23, exp_glob, "IKJ56714A Enter current password", 24,
						exp_glob, "ISPF processor ended abnormally", 25,
						exp_glob, "BROWSING ACTIVE CRITICAL JOBS", 27, exp_glob,
						"CURRENT PLAN AND STATUS INQUIRY", 28,
						//EMSP00
						exp_glob, "EMSP00", 26, exp_glob,
						"already logged on to system", 30, exp_glob,
						"INDIQUE A OPÇÃO PRETENDIDA", 31, exp_glob,
						"COMMAND UNRECOGNIZED", 32, exp_glob,
						"dos quais seja titular o responsavel por este sistema de informacao",
						33, exp_glob, "IKJ56703A REENTER THIS OPERAND", 34,
						exp_glob,"INVALID SWAP COMMAND SYNTAX",35,
						exp_glob,"- PCICST01 -",36,
						exp_glob,"Type your userid and password",37,
						exp_glob,"DFHCE3549 SIGN-ON IS COMPLETE",38,
						exp_end);
		switch (rc) {
		case 25:
			Informar("     ISPF processor ended abnormally");
			Informar("		System abend code        106");
			Informar("            Reason code 0C");
			/* no break */
		case 0:
		case 1:
		case 2:
			Ok();
			//Pa2();
			//MoverCursor(22, 13);
			//Eeof();

			//EnterString("lf " + applid);
			EnterString("logon applid(" + applid + ")");
			continue;
			break;
		case 3:
			Ok();
			//Pa2(); //xok("pa(2)");
			EnterString("lf " + applid);
			EnterString(applid);
			continue;
			break;
		case 4:
			Informar("user nao autorizado.. abortando");
			//fichlog"user nao autorizado... abortando "<<esta_lpar<<" "<<applid<<endl;
			//RELAUNCH = false;
			return false;
			break;
		case 5:
			Ok();
			Enter(); //xok("enter");
			continue;
			break;
		case 6:
			Ok();
			return true;
			break;
		case 7:
			break;
		case 8:
			Informar("IKJ56700A ENTER USERID vou entrar com user " + u);
			Ok();
//			home();
//			newline();
//			eeof();
			EnterString(u);

			continue;
			break;
		case 9:
			Ok();
			if(!MoverCursor(7, 19)){
				continue; //xok("movecursor(7,19)");
			}
			EnterString(p);
			continue;
			break;

		case 10:
			Ok();
			continue;
			break;
		case 28:
			/* no break */
		case 27:
			//Ok();
			Home();
			Eeof();
			Pf10();
			/* no break */
		case 11:
			Ok();
			Mpf7();
			Enter(); //xok("enter");
			return true;
			break;
		case 12:
			Ok();
			Enter(); //xok("enter");
			Home(); //xok("home");
			return true;
			break;
		case 13:
		case 14:
			Ok();
			Mpf7();
			Enter(); //xok("enter");
			return true;
			break;
		case 15:
			//Entrada excepcional para o MVS2 (R)
			if (lpar == "MVS2") {
				Ok();
				Enter();
				//sleep(1);
				Enter();
				continue;
				break;
			}
			Ok();
			Enter();
			continue;
			break;
		case 16:
			Ok();
			Home(); //xok("home");
			Eeof(); //xok("EraseEof");
			Pf3(); //xok("pf(3)");
			Home(); //xok("home");
			Enter(); //xok("enter");
			Mpf7();
			Enter(); //xok("enter");
			return true;
			break;
		case 17:
			Ok();
			Enter(); //xok("enter");
			continue;
			break;
		case 18:
			Ok();
			Mpf7();
			Enter(); //xok("enter");
			return true;
			break;
		case 19:
			Ok();
			Pf3(); //xok("pf(3)");
			Home(); //xok("home");
			Mpf7();
			Enter(); //xok("enter");
			return true;
			break;
		case 20:
			Ok();
			Mpf7();
			EnterString("n");
			Pf3(); //xok("pf(3)");
			Home(); //xok("home");
			Mpf7();
			Enter(); //xok("enter");
			return true;
			break;
		case 21:
			Ok();
			Home(); //xok("home");
			Pf3(); //xok("pf(3)");
			Pf3(); //xok("pf(3)");
			Home(); //xok("home");
			Mpf7();
			Enter(); //xok("enter");
			return true;
			break;
		case 22:
			Ok();
			Home(); //xok("home");
			Eeof(); //xok("eraseeof");
			EnterString(u);
			continue;
			break;
		case 23:
			Ok();
			if(!MoverCursor(22, 13)){
				continue;; //xok("movecursor(22,13)");
			}
			Eeof(); //xok("EraseEOF");
			EnterString(applid);
			continue;
			break;
		case 24:
			Ok();
			EnterString(p);
			continue;
			break;
		case 26:
			Ok();
			if(!MoverCursor(15, 71)){
				continue;
			}
			Eeof();
			EnterString("exit");
			//LoginMira();
			Clear();
			EnterString("a");
			PutString(usercgd);
			NewLine();
			PutString(passcgd);
			Enter();
			continue;
			break;
		case 30:
			for (int llll = 0; llll < 30; ++llll) {
				Informar(
						"USER JÁ LOGADO no " + lpar
								+ " Cancele o YYIBM01 neste lpar e aguarde");
				//sleep(1);
			}
			sleep(10);
			EnterString("LOGOFF");
			continue;
			break;
		case 31:
			Home();
			Eeof();
			EnterString("logon applid(" + applid + ")");
			continue;
			break;
		case 32:
			Clear();
			Home();
			EnterString("logon applid(" + applid + ")");
			continue;
			break;
		case 33:
			LimparBuffer();
			Enter();
			continue;
			break;
		case 34:
			Clear();
			Executar3270("Attn()");
			Clear();
			Home();
			Eeof();
			EnterString("logoff");
			continue;
			break;
		case 35:
			Clear();
			Home();
			EnterString("logon applid(" + applid + ")");
			continue;
			break;
		case 36:
			Clear();
			EnterString("cesn");
			continue;
			break;
		case 37:
			PutString(u);
			MoverCursor(10,25);
			EnterString(p);
			continue;
			break;
		case 38:
			Ok();
			Clear();
			return true;
			break;
		case EXP_TIMEOUT:
			Ok();
			ShowThisScreen();
			Informar("Login TSO - ecra inexperado no tso, gerou timeout");
			Informar("Vou executar lf e relogin neste tso n(" + applid + ")");
			//Pa2();
			sleep(1);
			Stop();
			Start(lpar);
			//EnterString("lf " + applid);
			sleep(1);
			EnterString("logon applid(" + applid + ")");
			continue;
			//return false;
			break;
		}
		//break;
	}
	exp_timeout = bkp_exp_timeout;
	return true;
}

bool CMenu::VerificarChecklistLoper() {
	/*
	 * A partir das 04h00 até que o batch listado na LOPER termine,
	 * Verificar o estado de cada execução.
	 * Se algum job ficou executado dentro de hora máxima +1h,
	 * não verificar mais.
	 * Se o job ainda não executou em hora máxima +1h, alertar sonoramente
	 * para se escalar!
	 */
	Home();
	EnterString("start");
	Home();
	EnterString("tso loper");
	EnterString("1");
	/*
	 * Capturar o conteúdo da LOPER
	 */

	/*
	 * sair do painel
	 */
	Pf3();
	Pf3();
	Pf3();
	return true;
}

bool CMenu::LoginTso(string nomTso, string u, string p) {
	int bkp_exp_timeout = exp_timeout, rc;	//salvaguardar o exp_timeout default

	exp_timeout = 30;
	//Pa2();
	//MoverCursor(22, 13);
	//Eeof();
	sleep(2);	//translation table

	EnterString(nomTso);
	Ok();
	sleep(1);
	//IKJ56700A ENTER USERID - meter user
	cout << endl << ".logintso." << endl;

	//a partir daqui existem diversas possibilidades
	/*
	 * login puro, entrando com user e password até cair no painel do ispf
	 * login parcial onde falta introduzir o user / ou password / ou dar enter no ***
	 * login já completo mas com ecrã inexperado no tso
	 */
	while (1) {
		sleep(1);
		fprintf(fpipe, "ascii\r");
		sleep(1);
		rc =
				exp_fexpectl(fpipe, exp_glob, "EMS1334E",
						0, //mau, refazer logon
						exp_glob, "IKJ56710I", 1, exp_glob,
						"IKJ56703A REENTER THIS OPERAND", 2, exp_glob,
						"EMS1112E",
						3, //relogin refresh screen
						exp_glob,
						"IKJ56421I PASSWORD NOT AUTHORIZED FOR USERID", 4,
						exp_glob, "\r\ndata:  \\*\\*\\*                 ", 5,
						exp_glob, "ISPF Primary Option Menu",
						6, //boa, ja ca estamos.
						exp_glob, "IKJ56700A ENTER USERID",
						8, //enviar userid
						exp_glob, "Enter LOGON parameters below:", 9, exp_glob,
						"Application Selection", 10, exp_glob,
						"HANDLING OPERATIONS ENDED IN ERROR", 11, exp_glob,
						"SPECIFYING ENDED IN ERROR LIST", 12, exp_glob,
						"COMMAND INPUT ===>", 13, exp_glob, "READY LIST", 14,
						exp_glob, "IKJ56951I NO BROADCAST MESSAGES", 15,
						exp_glob, "OPERATOR INSTRUCTION", 16, exp_glob,
						"IKT00300I LOGON RECONNECT SUCCESSFUL", 17, exp_glob,
						"BROWSING OPERATIONS", 18, exp_glob,
						"HO006DA ----------------- SELECTING ALL DEPENDENCIES",
						19, exp_glob,
						"HO006DA -------- CONFIRMING INVOCATION OF THE ALL", 20,
						exp_glob,
						"HO006DA -------- ALL DEPENDENCIES OF AN OPERATION", 21,
						exp_glob, "IKJ56420I Userid", 22, exp_glob, "EMSP01",
						23, exp_glob, "IKJ56714A Enter current password", 24,
						exp_glob, "ISPF processor ended abnormally", 25,
						exp_glob, "BROWSING ACTIVE CRITICAL JOBS", 27, exp_glob,
						"CURRENT PLAN AND STATUS INQUIRY", 28,
						//EMSP00
						exp_glob, "EMSP00", 26, exp_glob,
						"already logged on to system", 30, exp_glob,
						"INDIQUE A OPÇÃO PRETENDIDA", 31, exp_glob,
						"COMMAND UNRECOGNIZED", 32, exp_glob,
						"dos quais seja titular o responsavel por este sistema de informacao",
						33, exp_glob, "IKJ56703A REENTER THIS OPERAND", 34,
						exp_glob, "INVALID SWAP COMMAND SYNTAX", 35, exp_end);
		switch (rc) {
		case 25:
			Informar("     ISPF processor ended abnormally");
			Informar("		System abend code        106");
			Informar("            Reason code 0C");
			/* no break */
		case 0:
		case 1:
		case 2:
			Ok();
			//Pa2();
			//MoverCursor(22, 13);
			//Eeof();

			//EnterString("lf " + nomTso);
			EnterString("logon applid(" + nomTso + ")");
			continue;
			break;
		case 3:
			Ok();
			//Pa2(); //xok("pa(2)");
			EnterString("lf " + nomTso);
			EnterString(nomTso);
			continue;
			break;
		case 4:
			Informar("user nao autorizado.. abortando");
			//fichlog"user nao autorizado... abortando "<<esta_lpar<<" "<<nomTso<<endl;
			//RELAUNCH = false;
			return false;
			break;
		case 5:
			Ok();
			Enter(); //xok("enter");
			continue;
			break;
		case 6:
			Ok();
			return true;
			break;
		case 7:
			break;
		case 8:
			Informar("IKJ56700A ENTER USERID vou entrar com user " + u);
			Ok();
//			home();
//			newline();
//			eeof();
			EnterString(u);

			continue;
			break;
		case 9:
			Ok();
			if (!MoverCursor(7, 19)) {
				continue; //xok("movecursor(7,19)");
			}
			EnterString(p);
			continue;
			break;

		case 10:
			Ok();
			continue;
			break;
		case 28:
			/* no break */
		case 27:
			//Ok();
			Home();
			Eeof();
			Pf10();
			/* no break */
		case 11:
			Ok();
			Mpf7();
			Enter(); //xok("enter");
			return true;
			break;
		case 12:
			Ok();
			Enter(); //xok("enter");
			Home(); //xok("home");
			return true;
			break;
		case 13:
		case 14:
			Ok();
			Mpf7();
			Enter(); //xok("enter");
			return true;
			break;
		case 15:
			//Entrada excepcional para o MVS2 (R)
			if (lpar == "MVS2") {
				Ok();
				Enter();
				//sleep(1);
				Enter();
				continue;
				break;
			}
			Ok();
			Enter();
			continue;
			break;
		case 16:
			Ok();
			Home(); //xok("home");
			Eeof(); //xok("EraseEof");
			Pf3(); //xok("pf(3)");
			Home(); //xok("home");
			Enter(); //xok("enter");
			Mpf7();
			Enter(); //xok("enter");
			return true;
			break;
		case 17:
			Ok();
			Enter(); //xok("enter");
			continue;
			break;
		case 18:
			Ok();
			Mpf7();
			Enter(); //xok("enter");
			return true;
			break;
		case 19:
			Ok();
			Pf3(); //xok("pf(3)");
			Home(); //xok("home");
			Mpf7();
			Enter(); //xok("enter");
			return true;
			break;
		case 20:
			Ok();
			Mpf7();
			EnterString("n");
			Pf3(); //xok("pf(3)");
			Home(); //xok("home");
			Mpf7();
			Enter(); //xok("enter");
			return true;
			break;
		case 21:
			Ok();
			Home(); //xok("home");
			Pf3(); //xok("pf(3)");
			Pf3(); //xok("pf(3)");
			Home(); //xok("home");
			Mpf7();
			Enter(); //xok("enter");
			return true;
			break;
		case 22:
			Ok();
			Home(); //xok("home");
			Eeof(); //xok("eraseeof");
			EnterString(u);
			continue;
			break;
		case 23:
			Ok();
			if (!MoverCursor(22, 13)) {
				continue;; //xok("movecursor(22,13)");
			}
			Eeof(); //xok("EraseEOF");
			EnterString(nomTso);
			continue;
			break;
		case 24:
			Ok();
			EnterString(p);
			continue;
			break;
		case 26:
			Ok();
			if (!MoverCursor(15, 71)) {
				continue;
			}
			Eeof();
			EnterString("exit");
			//LoginMira();
			Clear();
			EnterString("a");
			PutString(usercgd);
			NewLine();
			PutString(passcgd);
			Enter();
			continue;
			break;
		case 30:
			for (int llll = 0; llll < 30; ++llll) {
				Informar(
						"USER JÁ LOGADO no " + lpar
								+ " Cancele o YYIBM01 neste lpar e aguarde");
				//sleep(1);
			}
			sleep(10);
			EnterString("LOGOFF");
			continue;
			break;
		case 31:
			Home();
			Eeof();
			EnterString("logon applid(" + nomTso + ")");
			continue;
			break;
		case 32:
			Clear();
			Home();
			EnterString("logon applid(" + nomTso + ")");
			continue;
			break;
		case 33:
			LimparBuffer();
			Enter();
			continue;
			break;
		case 34:
			Clear();
			Executar3270("Attn()");
			Clear();
			Home();
			Eeof();
			EnterString("logoff");
			continue;
			break;
		case 35:
			Clear();
			Home();
			EnterString("logon applid(" + nomTso + ")");
			continue;
			break;
		case EXP_TIMEOUT:
			Ok();
			ShowThisScreen();
			Informar("Login TSO - ecra inexperado no tso, gerou timeout");
			Informar("Vou executar lf e relogin neste tso n(" + nomTso + ")");
			//Pa2();
			sleep(1);
			Stop();
			Start(lpar);
			//EnterString("lf " + nomTso);
			sleep(1);
			EnterString("logon applid(" + nomTso + ")");
			continue;
			//return false;
			break;
		}
		//break;
	}
	exp_timeout = bkp_exp_timeout;
	return true;
}

