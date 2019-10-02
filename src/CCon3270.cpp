/*  
 * File:   CCon3270.cpp
 * Author: marlon.simoni
 * 
 * Created on 16 de Março de 2014, 13:32
 */

#include "CCon3270.h"

//namespace CCon3270{

/*
 *
 {
 interp = Tcl_CreateInterp();
 Expect_Init(interp);
 Tcl_InitStubs(interp, 0, 0);
 fpipe = NULL;
 started = false;
 }
 */
CCon3270::CCon3270() {
	interp = Tcl_CreateInterp();
	Expect_Init(interp);
	Tcl_InitStubs(interp, 0, 0);
	fpipe = NULL;
	started = false;
}

CCon3270::CCon3270(const CCon3270& orig) {
	interp = Tcl_CreateInterp();
	Expect_Init(interp);
	Tcl_InitStubs(interp, 0, 0);
	fpipe = NULL;
	started = false;
}

CCon3270::~CCon3270() {
	fclose(fpipe); //.close();
	int status;
	waitpid(exp_pid, &status, 0);
	cout << "programa terminou com status=" << status << endl;
}

string CCon3270::Capturar(string linha, string coluna, string n) {
	/*
	 * capturar n bytes nessas coordenadas no ecrã
	 */
	//salvaguardar coordenadas para depuramento
	coordenadas = "linha=" + linha + " coluna=" + coluna + " lenght=" + n;
	string str, retorna;
	size_t pos, sn;
	int intlinha, intcoluna, intn;
	intlinha = StringToInt(linha);
	intcoluna = StringToInt(coluna);
	intn = StringToInt(n);

	sn = (size_t) StringToInt(n);
	if (intlinha < 0) {
		linha = "0";
	}
	if (intlinha > 23) {
		linha = "23";
	}
	if (intcoluna < 0) {
		coluna = "0";
	}
	if (intcoluna > 79) {
		coluna = "79";
	}

	//verificar se o size n não ultrapassa fora do ecra.
	if (intcoluna + intn > 79) {
		Informar(
				"Captura fora do ecra ASCII(" + linha + "," + coluna + ",1," + n
						+ ")");
		return "";
	}

	//TODO LimparBuffer();

	Executar3270("ascii(" + linha + "," + coluna + ",1," + n + ")");
	str = exp_buffer;
	pos = str.find("data:");
	//cout<<exp_buffer<<endl;
	retorna = str.substr(pos + 6, sn);
	if (str == "")
		throw 100;
	return retorna;
}

string CCon3270::Capturar(int linha, int coluna, int n) {
	/*
	 Captura com parametros a int, a começar em (1 x 1)
	 */
	//if(linha==0) ++linha;
	//if(coluna==0) ++coluna;
	return this->Capturar(IntToString(--linha), IntToString(--coluna),
			IntToString(n));
}

int CCon3270::StringToInt(string s) {
	/*
	 * converter numero em string para int
	 */
	int Result;
	//char Text[16];
	//Text=s.c_str();

	//proxy += ip;
	//proxy = proxy.c_str();
	char *c = new char[s.size() + 1];
	c[s.size()] = 0; //terminador null
	memcpy(c, s.c_str(), s.size());

	sscanf(c, "%d", &Result);
	return Result;
//    istringstream convert(s);
//    convert >> i; //converter string para int
//    return i;
}

string CCon3270::IntToString(int i) {
	string s;
	char Result[16];
	sprintf(Result, "%d", i);
	s = Result;
	return s;
//    ostringstream out;
//    out << i;
//    return out.str();
}

bool CCon3270::Apareceu(string str, int tentativas) {
	//cout<<"aparece"<<endl;
	//esperar que a string str apareça no ecra até um timeout de secs segundos
	//salvaguardar timeout global do expect.
	int rc; //,timeoutbak=exp_timeout;
	//exp_timeout=secs;
	//exp_timeout = 2; //  implementar loop com try baseado no este_timeout
	//enviar o comando ascii
	//cout<<"antes de limpar buffer"<<endl;
	int bkp_exp_timeout = exp_timeout;
	exp_timeout = 2; //  implementar loop com try baseado no este_timeout
	int conttry = 0;

	string comando = "ascii\r";

	//informa("antes de limpar buffer");
	//TODO LimparBuffer();
	//informa("depois de limpar buffer");
	//sleep(depois);//esperar segundos antes de executar
	while (true) {
		//informa("antes de limpar buffer");
		//TODO LIMPARBUFFER()
		//LimparBuffer();
		//informa("depois de limpar buffer");
		//sleep(depois);//esperar segundos antes de executar
		fprintf(fpipe, "%s", comando.c_str());
		//		if(aparece("ISPF system data set allocation error - press Enter to continue.",0))
		//enter();

		//pause script até que o ecrã seja actualizado
		//devolvento o conteúdo em snap(ascii)
		//WaitUpd();

		rc = exp_fexpectl(fpipe, exp_glob, str.c_str(), 0, exp_glob,
				"ISPF system data set allocation error", 1, exp_glob,
				"IEA705I ERROR DURING GETMAIN", 2, exp_glob,
				"Unrecoverable Error", 3, exp_end);
		switch (rc) {
		case 0:
			exp_timeout = bkp_exp_timeout;
			//informa("apareceu "+str);
			Ok(); //retirar restantes chars até ao ok do s3270
			return true;
			break; //inutil
		case 1:
			Ok(); //"erro ispf"
			ShowThisScreen();
			Informar("inexplicavel erro no ispf!!?!?!? ... retry... ");
			Informar("inexplicavel erro no ispf!!?!?!? ... retry... ");
			Informar("inexplicavel erro no ispf!!?!?!? ... retry... ");
			Informar("inexplicavel erro no ispf!!?!?!? ... retry... ");
			Enter();
			Home();
			Eeof();
			exp_timeout = 2;
			//continue;
			break;
		case 2:
		case 3:
			//abend getmain
			Ok(); // erro getmain
			ShowThisScreen();
			Informar("erro getmain, vou fazer relogin no tso ");
			//RELAUNCH=false;
			Pa2();
			return false;
			break;
		case EXP_TIMEOUT:
			Ok();
			//comando=(comando=="ascii\r")?"printtext(ascii)\r":"ascii\r";
			//fichlog"a espera que aparecesse "<<str<<" "<<esta_lpar<<" "<<esta_op<<endl;
			//continue;
			//return false;

			Informar("Pipes - timeout a espera que aparecesse " + str);
			break;
		default:
			break;
		}
		Home();			//TODO
		Enter();
//		sleep(1);
		++conttry;
		if (conttry > tentativas) {
			exp_timeout = bkp_exp_timeout;
			Informar(
					"timeout aparece " + str + " tentativas "
							+ IntToString(tentativas));
			return false;
		}
		exp_timeout = 2;
	}
	exp_timeout = bkp_exp_timeout;
	return false;
}

bool CCon3270::Apareceu(string str, int linha, int coluna) {
	//cout<<"aparece"<<endl;
	//esperar que a string str apareça no ecra até um timeout de secs segundos
	//salvaguardar timeout global do expect.
	int rc; //,timeoutbak=exp_timeout;
	//exp_timeout=secs;
	//exp_timeout = 2; //  implementar loop com try baseado no este_timeout
	//enviar o comando ascii
	//cout<<"antes de limpar buffer"<<endl;
	--linha;
	--coluna;
	linha = linha < 0 ? 0 : linha;
	coluna = coluna < 0 ? 0 : coluna;

	string slinha = IntToString(linha);
	string scoluna = IntToString(coluna);
	int len = str.size();
	string slen = IntToString(len);

	string comando = "ascii(" + slinha + "," + scoluna + "," + slen + ")\r";

	//informa("antes de limpar buffer");

	//TODO LIMPARBUFFER LimparBuffer();
	//informa("depois de limpar buffer");
	//sleep(depois);//esperar segundos antes de executar
	fprintf(fpipe, "%s", comando.c_str());
	//		if(aparece("ISPF system data set allocation error - press Enter to continue.",0))
	//enter();

	//pause script até que o ecrã seja actualizado
	//devolvento o conteúdo em snap(ascii)
	//WaitUpd();

	rc = exp_fexpectl(fpipe, exp_glob, str.c_str(), 0, exp_glob,
			"Unrecoverable Error", 1, exp_end);
	switch (rc) {
	case 0:
		//informa("apareceu "+str);
		Ok(); //retirar restantes chars até ao ok do s3270
		return true;
		break; //inutil
	case 1:
	case 2:
	case 3:
		break;
	case EXP_TIMEOUT:
		//Ok();
		Informar(
				"timeout: nao apareceu=(" + str + ")L\'" + slen + " na posição "
						+ slinha + " X " + scoluna);
		return false;
		break;
	default:
		return false;
		break;
	}
	return false;
}

void CCon3270::WaitInputReady() {
	//Executar3270("wait(10,inputfield)");
	int rc;
	//LimparBuffer();
	fprintf(fpipe, "wait(10,inputfield)\r");
	rc = exp_fexpectl(fpipe, exp_glob, "\r\nok\r\n", 0, exp_glob,
			"\r\nerror\r\n", 1, exp_end);
	if (rc == 0) {
		//LimparBuffer();
	} else if (rc == 1) {
		Informar("timeout wait(10,inputfield)");
		Ok();
	}
}
bool CCon3270::WaitOutput() {
	return true;
	//fprintf(fpipe, "wait(output)\r");
	//Executar3270("wait(output,10)");
	int rc;
	//LimparBuffer();
	fprintf(fpipe, "wait(10,output)\r");
	rc = exp_fexpectl(fpipe, exp_glob, "\r\nok\r\n", 0, exp_glob,
			"\r\nerror\r\n", 1, exp_end);
	if (rc == 0) {
		//LimparBuffer();
		return true;
	} else if (rc == 1) {
		Informar("timeout wait(10,output)");

		//executar disconnect e connect porque
		//perdeu-se a sessão e nada a fará voltar sem
		//se exeecutar isto.

		//Home();
		//Enter();
		//Ok();
		return false;
	}
	return true;
}
void CCon3270::Home() {
	Executar3270("Home");
	//WaitOutput();
}
void CCon3270::Clear() {
	Executar3270("Clear");
	//WaitOutput();
}
void CCon3270::Pf8() {
	Executar3270("pf(8)");
	//WaitOutput();
}
void CCon3270::Pf9() {
	Executar3270("pf(9)");
	//WaitOutput();
}
void CCon3270::Pf7() {
	Executar3270("pf(7)");
	//WaitOutput();
}
void CCon3270::Pf3() {
	Executar3270("pf(3)");
	//WaitOutput();
}
void CCon3270::Pf10() {
	Executar3270("pf(10)");
	//WaitOutput();
}
void CCon3270::Pf11() {
	Executar3270("pf(11)");
	//WaitOutput();
}
void CCon3270::Pf24() {
	Executar3270("pf(24)");
	//WaitOutput();
}
void CCon3270::Pa2() {
	Executar3270("pa(2)");
	//WaitOutput();
}
void CCon3270::NewLine() {
	Executar3270("NewLine");
}
void CCon3270::NewLine(int times) {
	for (int i = 0; i < times; ++i) {
		Executar3270("NewLine");
	}
}
void CCon3270::Eeof() {
	Executar3270("EraseEof");
}
void CCon3270::EraseInput() {
	Executar3270("EraseInput");
}
void CCon3270::Tab() {
	Executar3270("Tab");
}
void CCon3270::Enter() {
	//WaitInputReady();
	Executar3270("enter");
	//WaitOutput();
}
void CCon3270::Mpf8() { // m + pf(8)
	Home();
	PutString("m");
	Pf8();
}
void CCon3270::Mpf7() { // m + pf(7)
	Home();
	PutString("m");
	sleep(1);
	Pf7();
}
/*
 * Mover o cursor para as coordenadas linha,coluna que começam a zero
 */
bool CCon3270::MoverCursor(int linha, int coluna) { //mover o cursor no 3270
	//string l,c;
	char l[50], c[50];
	string lin, col;
	sprintf(l, "%d", linha);
	sprintf(c, "%d", coluna);
	lin = l;
	col = c;
	//itoa(linha,l,10);
	//itoa(coluna,c,10);
	//cout<<"lin="<<lin<<" col="<<col<<endl;
	Executar3270("movecursor(" + lin + "," + col + ")");
	/*
	 * Verificar se o cursor ficou na posição pretendida.
	 * U F U C(192.168.90.17) I 2 24 80 15 71 0x88000b4 -
	 *  1 2 3                4 5 6  7  8  9
	 * partir o prompt em espaços. as coordenadas
	 * ficam depois do espaço 8 (X) e 9 (Y)
	 */
	string cursor = GetStrPromtState();
	string testlin, testcol;
	int contar_espacos = 0;
	for (unsigned i = 0; i < cursor.size(); ++i) {
		if (cursor.at(i) == ' ') {
			contar_espacos++;
		}
		if (contar_espacos == 8) {
			++i;
			while (cursor.at(i) != ' ') {
				testlin.push_back(cursor.at(i));
				++i;
			}
			++i;	//saltar o espaço ' '
			while (cursor.at(i) != ' ') {
				testcol.push_back(cursor.at(i));
				++i;
			}
			break;
		}
	}
	//cout << "prompt=" << cursor << endl;
	//cout << "lin:" << lin << " testlin:" << testlin << " col:" << col
	//		<< " testcol:" << testcol << endl;
	return (lin == testlin && col == testcol);
}
void CCon3270::SetUserPass(string u, string p) {
	//user = u.substr(0,8);
	//pass = p.substr(0,8);
	//char c;
	user.clear();
	pass.clear();

	for (int i = 0; i < 8 && i < (int) u.size(); ++i) {
		if (u.at(i) != '\r') {
			user.push_back(u.at(i));
		}
	}
	for (int i = 0; i < 8 && i < (int) p.size(); ++i) {
		if (p.at(i) != '\r') {
			pass.push_back(p.at(i));
		}
	}
}
void CCon3270::LimparBuffer() {
	//Ok();
	int bk = exp_timeout;
	exp_timeout = 1;
	//exp_match
	int rc = exp_fexpectl(fpipe, exp_glob, "*\r\nok\r\n", 0, exp_end);
	//limpar buffer
	int contador = 0;
	//cout << endl << endl << endl;
	while (true) {
		switch (rc) {
		case EXP_EOF:
			cout << lpar << " Limpexa do buffer EXP_EOF" << endl;
			exp_timeout = bk;
			return;
			break;
		case EXP_TIMEOUT:
			exp_timeout = bk;
			if (contador > 0) {
				cout << lpar << " Limpeza do buffer match EXP_TIMEOUT contagem="
						<< contador << endl;
			}
			return;
			break;
		case 0:
			rc = exp_fexpectl(fpipe, exp_glob, "*\r\nok\r\n", 0, exp_end);
			if (contador > 0) {
				cout << lpar << " Limpeza do buffer match ok contagem="
						<< contador << endl;
			}
			contador++;
		}
	}

	/*
	 * Código obsleto
	 */
	exp_timeout = bk;
	cout << endl << endl << endl;
	if (exp_buffer == '\0')
		return;
	//cout<<"exp_buffer="<<exp_buffer<<endl;
	//cout<<"&exp_buffer="<<&exp_buffer<<endl;
	//cout<<"*exp_buffer="<<*exp_buffer<<endl;

	//cout<<"exp_buffer sujo:"<<endl<<exp_buffer<<":"<<endl;
	char *ch;
	ch = exp_buffer;
	while (*ch != '\0') {
		*ch = ' ';
		ch++;
	}
	*ch = '\0';

	//exp_buffer=bf;
	//cout<<"exp_buffer limpo:"<<endl<<exp_buffer<<":"<<endl;
	//cout<<"buffer limpo"<<endl;
}
string CCon3270::CharClear(string str) {
	string limpa;
	//cout<<endl<<"antes :"<<str<<endl;
	char ch;
	int c = 0;

	for (size_t i = 0; i < str.size(); ++i) {
		ch = str.at(i);
		if (ch < 32 || ch > 125) {
			++c;
			if (c == 2) //porque os caracteres vêm em utf e fora dessa range de ascci são 2 bytes!!
					{
				c = 0;
				limpa.push_back('_');
			}
		} else
			limpa.push_back(ch);
	}
	//cout<<endl<<"depois:"<<limpa<<endl;
	for (size_t i = 0; i < limpa.size(); ++i) {
		ch = limpa.at(i);
		if (ch == '\'')
			limpa.at(i) = ' ';
	}
	return limpa;
}
void CCon3270::PrintTimestamp() {
	string ts;
	time_t ltime; //calendar time
	char buffer[80];

	ltime = time(NULL); //get current calendar time
	//ts=asctime(localtime(&ltime));
	//
	strftime(buffer, 80, "%d-%m-%Y %X: ", localtime(&ltime));
	cout << buffer;

	return;
}
string CCon3270::GetStrTimestamp() {
	string ts;
	time_t ltime; //calendar time
	char buffer[80];

	ltime = time(NULL); //get current calendar time
	//ts=asctime(localtime(&ltime));
	//
	strftime(buffer, 80, "%d-%m-%Y %X: ", localtime(&ltime));
	//cout << buffer;

	return buffer;
}
string CCon3270::GetStrTimestamp_Maria() {
	string ts;
	time_t ltime; //calendar time
	char buffer[80];

	ltime = time(NULL); //get current calendar time
	//ts=asctime(localtime(&ltime));
	//
	strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", localtime(&ltime));
	//cout << buffer;

	return buffer;
}

vector<string> CCon3270::GrabScreen() {
	vector<string> regs;
	string reg, tudo;
	char ch;

	reg.clear();

	//TODO LIMPARBUFFER()
	//LimparBuffer();
	/*	fprintf(fpipe, "%s", "\r");	//enter no s3270 somente para produzir o promt
	 //esperar o ok do s3270
	 //rc = exp_fexpectl(fpipe, exp_glob, "\r\nok\r\n", 0, exp_end);
	 int	rc = exp_fexpectl(fpipe, exp_glob, "\r\nok\r\n", 0, exp_end);*/
	//fprintf(fpipe,"printtext string\r");
	if (!Executar3270("ascii")) {
		return regs;
	}
	//sleep(1);
	//recolher o buffer exp_buffer
	tudo = exp_buffer;

	//cout<<endl<<"tudo:"<<endl<<tudo<<endl<<"fim"<<endl;

	//remover os char='\r'
	int j = tudo.find("\r");

	//cout<<"j="<<j<<endl;
	while (j > 0 && j < (int) tudo.size()) {
		tudo = tudo.replace(j, 1, "");
		//tudo=tudo.erase(j);
		j = tudo.find("\r");
		//cout<<"j="<<j<<endl;
	}

	//tudo=tudo.find('\r');
	reg.clear();
	for (size_t n = 0; n < tudo.size(); ++n) //dividir por quebras de linha=(\r\n)
			{
		ch = tudo.at(n);
		if (ch == '\0')
			break;
		if (ch == '\n') {
			regs.push_back(reg);
			//regs.append(reg);
			reg.clear();
			continue;
		} else
			reg.push_back(ch);
	}

	//cout<<"1_regs.size()="<<regs.size()<<endl;
	vector<string> temp;
	for (int j = 0; j < (int) regs.size(); j++) {
		size_t k = regs.at(j).find("data:");
		if (k > 0 || k < regs.at(j).size())
			temp.push_back(regs.at(j));
	}

	regs.clear();
	regs = temp;

	//	for(int i=0;i<(int)regs.size();++i)
	//		cout<<"regs:"<<regs.at(i)<<endl;

	//delete temp;
	if (!regs.empty())
		regs.pop_back(); //retirar os dois ultimos elementos do vetor que são info s3270 e não ecrã
	if (!regs.empty())
		regs.pop_back();
	//regs.pop_back();
	//regs.erase(regs.begin()); //retirar o ascii do vector

	if (!regs.empty())
		regs.erase(regs.begin()); //retirar o ascii do vector
	else
		return regs;

	//informa("grab_screen");
	//cout<<"2_regs.size()="<<regs.size()<<endl;

	size_t pos;

	for (int i = 0; i < (int) regs.size(); ++i) {
		reg.clear();
		reg = regs.at(i);
		pos = reg.find("data:");
		if (pos < 0 || pos > reg.size())
			continue;
		pos += 6;
		regs.at(i) = reg.substr(pos);
		//cout<<i<<"\t:|"<<regs.at(i)<<"|"<<endl;
	}

	//limpar buffer
	//rc=exp_fexpectl(fpipe,exp_glob,"\r\nok\r\n",0,exp_end);
	//ok();
	for (int i = 0; i < (int) regs.size(); ++i) {
		regs.at(i) = CharClear(regs.at(i));
	}

	return regs;
}
void CCon3270::ShowThisScreen() {
	vector<string> scr = GrabScreen();
	Informar("Ecra actual:");
	for (size_t i = 0; i < scr.size(); ++i) {
		cout << (int) i << "\t:" << scr.at(i) << ":" << endl;

	}
}
string CCon3270::GetStrThisScreen() {
	vector<string> scr = GrabScreen();
	//Informar("Ecra actual:");
	string ecra;
	for (size_t i = 0; i < scr.size(); ++i) {
		ecra += IntToString(i) + "\t:" + scr.at(i) + "\n";

		//cout << (int) i << "\t:" << scr.at(i) << ":" << endl;

	}
	return ecra;
}
string CCon3270::GetTimestamp() {
	string ts;
	time_t ltime; //calendar time
	char buffer[80];

	ltime = time(NULL); //get current calendar time
	//ts=asctime(localtime(&ltime));
	//
	strftime(buffer, 80, "%d-%m-%Y %X: ", localtime(&ltime));
	ts = buffer;

	return ts;
}
string CCon3270::Informar() {
	string info;
	info = GetTimestamp();
	info += " (t=" + IntToString(exp_timeout) + "): ";

	return info;
}
bool CCon3270::Ok() {
	/*
	 * limpar até ao OK
	 * Actualizado 19-09-2016 Marlon.
	 * --ir até ao ultimo ok do s3270 até dar EXP_TIMEOUT
	 * que é o que se pretende.
	 */
	//SetDebug(true);
	//int bk = exp_timeout;
	//exp_timeout = 1;
	int rc = exp_fexpectl(fpipe, exp_glob, "*\r\nok\r\n", 0, exp_end);
	//limpar buffer
	switch (rc) {
	case 0:
		return true;
		break;
	case EXP_EOF:
		cout << "Ok() EXP_EOF" << endl;
		return false;
		break;
	case EXP_TIMEOUT:
		cout << "Ok() EXP_TIMEOUT" << endl;
		return false;
		break;
	}
//	int contador = 0;
//	while (true) {
//
//		if (rc == EXP_EOF) {
//			cout << "Ok() EXP_EOF" << endl;
//			break;
//		}
//		if (rc == EXP_TIMEOUT) {
//			cout << "Ok() EXP_TIMEOUT" << endl;
//			break;
//		}
//		//ir até ao último ok do s3270 que está no buffer.
//		rc = exp_fexpectl(fpipe, exp_glob, "*\r\nok\r\n", 0, exp_end);
//		cout << "match ok cont=" << contador << endl;
//		contador++;
//	}
//	exp_timeout = bk;
//	SetDebug(false);
	return true;
}
void CCon3270::Informa() {
	PrintTimestamp();
	cout << " (t=" << exp_timeout << "): " << endl;

}
void CCon3270::Informar(string info) {
	PrintTimestamp();
	cout << lpar << " (t=" << exp_timeout << "): " << info << endl;

}
bool CCon3270::PutString(string str) {
	//enviar string para s3270
	//cout<<"xoks str="<<str<<endl;
	int rc;

	string pre = "string \"";
	pre += str;
	pre += "\"\r";

	//cout<<"string=<"<<pre<<">"<<endl;
	fprintf(fpipe, "%s", pre.c_str());
	//esperar o ok do s3270
	rc = exp_fexpectl(fpipe, exp_glob, "\r\nok\r\n", 0, exp_end);
	if (rc == 0)
		return true;
	if (rc == EXP_TIMEOUT) {
		ShowThisScreen();
		Informar("PutString timeout em xoks com " + str);
		//fichlog"ocorreu timeout em xoks com "+str<<esta_lpar<<" "<<esta_op<<endl;
		return false;
	}
	return false;
}
bool CCon3270::EnterString(string str) {
	//enviar string para s3270
	//cout<<"xoks str="<<str<<endl;
	int rc;

	string pre = "string \"";
	pre += str;
	pre += "\\n\"\r";

	//cout<<"string=<"<<pre<<">"<<endl;
	fprintf(fpipe, "%s", pre.c_str());
	//esperar o ok do s3270
	rc = exp_fexpectl(fpipe, exp_glob, "\r\nok\r\n", 0, exp_end);
	if (rc == 0)
		return true;
	if (rc == EXP_TIMEOUT) {
		ShowThisScreen();
		Informar("EnterString timeout em xoks com " + str);
		//fichlog"ocorreu timeout em xoks com "+str<<esta_lpar<<" "<<esta_op<<endl;
		return false;
	}
	return false;
}
bool CCon3270::Executar3270(string str) {
	/*
	 * VERIFICAR SE A SESSÃO ESTÁ BLOQUEADA!
	 */
	//SetDebug(true);
	string prompt = GetStrPromtState();
	if (prompt.substr(0, 1) != "U") {
		// se não está unlocked U
		//cout << endl << endl << "prompt=«««««««««" << prompt << "»»»»»»»»»»»"
		//		<< endl;

		return false;
	}

	int rc;

	str += "\r";
	fprintf(fpipe, "%s", str.c_str());
	//esperar o ok do s3270
	//rc = exp_fexpectl(fpipe, exp_glob, "\r\nok\r\n", 0, exp_end);

	//SetDebug(false);

	rc = exp_fexpectl(fpipe, exp_glob, "\r\nok\r\n", 0, exp_end);
	if (rc == 0)
		return true;
	if (rc == EXP_TIMEOUT) {
		ShowThisScreen();
		Informar("Executar3270 timeout em ok com " + str);
		////fichlog"ocorreu timeout em xoks com "+str<<esta_lpar<<" "<<esta_op<<endl;
		return false;
	}
	return false;
}
/*
 void CCon3270::OpenPassword(){
 //	ifstream fpass;
 //	string reg;
 //	int count = 0;

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
 return;

 //	if (fpass.is_open())
 //	{
 //		while (fpass.good())
 //		{
 //			getline(fpass, reg);
 //			switch (count)
 //			{
 //			case 0:
 //				usercgd = reg;
 //				Informar("usercgd=" + usercgd);
 //				break;
 //			case 1:
 //				passcgd = reg;
 //				break;
 //			case 2:
 //				usermcf = reg;
 //				Informar("usermcf=" + usermcf);
 //				break;
 //			case 3:
 //				passmcf = reg;
 //				break;
 //			case 4:
 //				userbcg = reg;
 //				Informar("userbcg=" + userbcg);
 //				break;
 //			case 5:
 //				passbcg = reg;
 //				break;
 //			}
 //			++count;
 //
 //		}
 //		fpass.close();
 //	}
 //	else
 //		Informar(
 //				"problemas ao tentar abrir o ficheiro de passwords (pipespass)");
 }
 */
void CCon3270::Start(string l, unsigned timeout) {
	//exp
	lpar = l;
	std::transform(lpar.begin(), lpar.end(), lpar.begin(), ::toupper);
	exp_loguser = 1;
	exp_timeout = timeout; //timeout global
	//exp_debug   = 1;
	exp_match_max = 3800;
	//string proxy = "s3270 ";
	//proxy += ip;
	//proxy = proxy.c_str();
	char c[200] = PROXY;
	//char *c = new char[proxy.size() + 1];
	//c[proxy.size()] = 0; //terminador null
	//memcpy(c, proxy.c_str(), proxy.size());


	fpipe = exp_popen(c);
	//cout<<"fpipe:"<<fpipe<<endl;
	started = true;
}
void CCon3270::Start(string l) {
	//exp
	lpar = l;
	std::transform(lpar.begin(), lpar.end(), lpar.begin(), ::toupper);
	exp_loguser = 1;
	exp_timeout = 15; //timeout global
	//exp_debug   = 1;
	exp_match_max = 3800;
	//string proxy = "s3270 ";
	string proxy = PROXY;
	proxy += " ";
	if (lpar == "MG1D") {
#ifdef YYIBM01
		proxy += "-cafile /home/cgdibm02/certificados/CERT.SUBCAMTP.pem -noverifycert 10.200.101.20:992";
#else
		proxy += "-cafile /home/ptstf093/certificados/CERT.SUBCAMTP.pem -noverifycert 10.200.101.20:992";
#endif
	} else if (lpar == "MG1P") {
#ifdef YYIBM01
		proxy += "-cafile /home/cgdibm02/certificados/CERT.SUBCAMTP.pem -noverifycert 10.200.101.30:992";
#else
		proxy += "-cafile /home/ptstf093/certificados/CERT.SUBCAMTP.pem -noverifycert 10.200.101.30:992";
#endif
	} else if (lpar == "MG1C") {
		proxy += "10.200.101.50";
	} else if (lpar == "MG1T") {
		proxy += "10.200.101.40";
	} else if (lpar == "SYA1") {
#ifdef YYIBM01
		proxy += "-cafile /home/cgdibm02/certificados/GrupoCGDRootCA.pem -noverifycert 192.168.90.17:992";
#else
		proxy += "-cafile /home/ptstf093/certificados/GrupoCGDRootCA.pem -noverifycert 192.168.90.17:992";
#endif
	} else if (lpar == "SYB1") {
#ifdef YYIBM01
		proxy += "-cafile /home/cgdibm02/certificados/GrupoCGDRootCA.pem -noverifycert 192.168.90.17:992";
#else
		proxy += "-cafile /home/ptstf093/certificados/GrupoCGDRootCA.pem -noverifycert 192.168.90.17:992";
#endif
	} else if (lpar == "SYB2") {
		proxy += "192.168.90.17";
	} else if (lpar == "MCFA") {
#ifdef YYIBM01
		proxy += "-cafile /home/cgdibm02/certificados/GrupoCGDRootCA.pem -noverifycert 192.168.90.73:992";
#else
		proxy += "-cafile /home/ptstf093/certificados/GrupoCGDRootCA.pem -noverifycert 192.168.90.73:992";
#endif
	} else if (lpar == "MCFB") {
#ifdef YYIBM01
		proxy += "-cafile /home/cgdibm02/certificados/GrupoCGDRootCA.pem -noverifycert 192.168.90.81:992";
#else
		proxy += "-cafile /home/ptstf093/certificados/GrupoCGDRootCA.pem -noverifycert 192.168.90.81:992";
#endif
	} else if (lpar == "MVS2") {
#ifdef YYIBM01
		proxy += "-cafile /home/cgdibm02/certificados/GrupoCGDRootCA.pem -noverifycert 192.168.90.161:992";
#else
		proxy += "-cafile /home/ptstf093/certificados/GrupoCGDRootCA.pem -noverifycert 192.168.90.161:992";
#endif
	}
	//proxy = proxy.c_str();
	//char c[200] = PROXY;
	proxy = proxy.c_str();

	char *c = new char[proxy.size() + 1];
	c[proxy.size()] = 0; //terminador null
	memcpy(c, proxy.c_str(), proxy.size());
	//char *c = new char[proxy.size() + 1];
	//c[proxy.size()] = 0; //terminador null
	//memcpy(c, proxy.c_str(), proxy.size());


	fpipe = exp_popen(c);
	sleep(20);
	//cout<<"fpipe:"<<fpipe<<endl;
	started = true;
}

void CCon3270::Stop() {
	//int rc;
	int s3270_pid = exp_pid;
	string str = "quit\r";
	fprintf(fpipe, "%s", str.c_str());
	sleep(2);

	cout << "exp_pid:" << s3270_pid << endl;

	Informar("fclose fpipe " + lpar);
	fclose(fpipe);
	int status;
	waitpid(s3270_pid, &status, 0);
	//wait(exp_pid);

	cout << "exp_pid wait done:" << s3270_pid << endl;
}

void CCon3270::LogToFile(string f, string message) {
	ofstream fichlog;
	fichlog.open(f.c_str(), std::fstream::app);	//abrir em MOD!

	if (fichlog.is_open()) {
		fichlog
				<< "************************************************************************"
				<< endl;
		fichlog << "Coordenadas da última captura: " << coordenadas << endl;
		fichlog << GetStrPromtState() << endl;
		fichlog << GetStrTimestamp() << endl;
		fichlog << message << endl;
		fichlog << GetStrThisScreen() << endl << endl << endl;
	}

}

string CCon3270::GetStrPromtState() {
	string strprompt;
	int rc;

	//TODO LIMPARBUFFER()
	//LimparBuffer();
	//fprintf(fpipe, "%s", "\r");	//enter no s3270 somente para produzir o promt
	//esperar o ok do s3270
	//rc = exp_fexpectl(fpipe, exp_glob, "\r\nok\r\n", 0, exp_end);
	//rc = exp_fexpectl(fpipe, exp_glob, "\r\nok\r\n", 0, exp_end);
	fprintf(fpipe, "%s", "\r");	//enter no s3270 somente para produzir o promt
	rc = exp_fexpectl(fpipe, exp_glob, "\r\nok\r\n", 0, exp_end);
	if (rc == 0) {
		strprompt = exp_buffer;
	}
	if (rc == EXP_TIMEOUT) {
		ShowThisScreen();
		Informar("Executar3270 timeout em ok com /\r");
		////fichlog"ocorreu timeout em xoks com "+str<<esta_lpar<<" "<<esta_op<<endl;
		return "ERRO";
	}
	string p;
	for (unsigned i = 0; i < strprompt.size(); ++i) {
		char incognito = strprompt.at(i);
		if (incognito != '.' && incognito != 13 && incognito != 10
				&& incognito != 0) {
			//int ii = incognito;
			//cout << "charprompt=(" << incognito << ")" << ii << endl;
			p.push_back(incognito);
		}
	}

	return p;
}
bool CCon3270::IsLocked() {
	string p = GetStrPromtState();
	string p2;
	unsigned i = 0;
	while (p.at(i) == ' ') {
		++i;
	}

	for (unsigned j = i; j < p.size(); ++j) {
		char incognito = p.at(j);

		if (incognito != 13 && incognito != 10 && incognito != 0 && incognito) {
			//int ii = incognito;
			//cout << "charprompt=(" << incognito << ")" << ii << endl;
			p2.push_back(incognito);
		}
	}
	return p2.substr(0, 1) != "U";

}

bool CCon3270::SeApareceu(string str) {
	/*
	 * Verificar se a string está no ecrã
	 * retornar true ou false
	 * não notificar se não apareceu.
	 */
	int rc; //,timeoutbak=exp_timeout;
	int bkp_exp_timeout = exp_timeout;
	exp_timeout = 1;
	string comando = "ascii\r";
	//TODO LIMPARBUFFER LimparBuffer();
	fprintf(fpipe, "%s", comando.c_str());
	rc = exp_fexpectl(fpipe, exp_glob, str.c_str(), 0, exp_end);
	switch (rc) {
	case 0:
		exp_timeout = bkp_exp_timeout;
		Ok(); //retirar restantes chars até ao ok do s3270
		return true;
		break; //inutil
	case EXP_TIMEOUT:
		Ok();
		break;
	default:
		break;
	}
	exp_timeout = bkp_exp_timeout;
	return false;
}
//}//end namespace CCon3270
