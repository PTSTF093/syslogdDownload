/* 
 * File:   CMenu.h
 * Author: marlon.simoni
 *
 * Created on 8 de Abril de 2014, 15:58
 */

#ifndef CMENU_H
#define	CMENU_H


#include <fstream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string>
#include "CCon3270.h"

#ifndef YYIBM01
#define SLEEPTIME 1*60 //segundo
#define RELOGINTIMER 1*30*60
#else
#define SLEEPTIME 2*60
#define RELOGINTIMER  1*30*60//*15
#endif


class CMenu : public CCon3270  {
public:
    CMenu();
    CMenu(string);//para connectar a db do cemg e não do pipes.
    CMenu(const CMenu& orig);
    virtual ~CMenu();
    
    bool EnterComando();
    void PrintMenu();
    bool SolicitadoSair(){ return sair; }
    bool LoginNetView(string l);
    bool ArrancarSYA1();
    bool ArrancarMG1P(string,string,string,string);
    bool ArrancarSYB2();
    bool ArrancarMCFA();
    bool ArrancarMCFB();
    bool ArrancarMVS2();
    bool ArrancarMG1D();//com opc1 MP_DES
    bool ArrancarMG1P();
    bool ArrancarMG1C();
    bool ArrancarMG1T();
    bool ArrancarMG1P_BCRS();//para drp porto
    bool SessaoCaiuLocked(string,string);//verifica se está locked ou se a sessão caiu fora do tso
    bool VerificarChecklistLoper();
    bool LoginTso(string,string,string);

    /*
     * RTGS
     */
    bool ArrancarSYA1RTGS(string,string);//é passado user e password
    bool LogonApplid(string,string,string);//applid,user,pass

private:
    time_t Agora;
    time_t Antes;
    string usercgd,usermcf,userbcg,usercemg;
            string passcgd,passmcf,passbcg,passcemg;
    string usertsop,usertsoc,usertsod,
			passtsop,passtsoc,passtsod;
    string tso,lpar;
    

    /*Controle de sessao no host*/
    bool OpenPassword();//abrir password encriptada.
    bool LoginNetView();

    bool setTso(string);
    void LogoutAll();
    void Logoff();//desconectar tso
    void Logoff(int);//logoff com parametro de timout s3270
    
    /*opcoes*/
    void Cutoffs();
    void AADFS();
    void GprtAnom();
    void GprtRec(int loops=0,int insist=0);
    void Gspi();
    void Grt();
    void Ftsm();
    void FtsmParam();
    
    void QueyQueuePedidos();
    
    string comando;
    bool sair;
    bool LogedNetView;//esta logado no netview
    bool LoopRec;
    
    bool solicitado_stop();
    bool QueryLoopRec();
    
    bool ElapsedTime(time_t t);//
};

#endif	/* CMENU_H */

