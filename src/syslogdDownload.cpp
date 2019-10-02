/*
 * File:   main.cpp
 * Author: Marlon
 *
 * Created on 16 de Março de 2014, 21:31
 */
//g++ gprtrec.cpp -o gprtrec -l:libexpect5.45.dll.a -ltcl8.5 -ltclstub8.5 -lm
#include <iostream>
#include <fstream>


//#include <ios>
//#include <cstdlib>
//#include <ctype.h>
//#include <algorithm>
//
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

//#include "CCon3270.h"
#include "CMenu.h"

int main(int argc, char**argv) {
    //CCon3270 conex;

    CMenu menu;

    //menu.LoginNetView("SYB1");
    try {
    	//user password data
    	cout<<"Arrancar MG1P"<<endl;
    	menu.ArrancarMG1P(argv[1],argv[2],argv[3],argv[4]);
	} catch (const std::out_of_range & e) {
		for(int i=0;i<25;++i){
			cout<<"Opipes SYB1 CAIU !!!!! "<<e.what()<<endl;
		}
	}

/*
    for(;!menu.SolicitadoSair();){
        menu.PrintMenu();
        menu.EnterComando();
    }
*/
    //cout << endl << endl << "Concluido..." << endl;
    //*fichout.close();
    return 0;
}
