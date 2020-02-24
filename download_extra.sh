#!/bin/bash
#
# Download diário do log de batch do CEMG
# para ser utilizado em BD para a projeção e médias.
#
#
#
#
#
#
#
#
#
#

#	int intDias=StringToInt(dias);
#	const int Constante=17165;
#	int numero=intDias-Constante;
#	string sufixo="G"+IntToString(numero)+"V00";

#haumasemana=$(date -d "-7 days" +"%Y-%m-%d")
#enderecos="ptstf093@pt.ibm.com, ALF.PT.MFO@stefanini.com, Patricia.Parames@stefanini.com"
enderecos="ptstf093@pt.ibm.com" #,ptstf031@pt.ibm.com,ptstf026@pt.ibm.com"
#echo "$haumasemana"
#exit 0
hoje=$(date +"%Y-%m-%d")
now=$(date +"%T")



##############
#    ALTERAR AQUI O dias E NA LINHA SEGUINTE A pass E data
dias=18308
/home/cgdibm02/git/repository/syslogdDownload/Debug/syslogdDownload xibmsi 'KU8AGX#X' 2020-02-16 $dias
#
##############




#data em número de dias desde EPOCH=1970
numero=$(( dias - 17165 ))
sufixo="G${numero}V00"
ficheiro="SYP.D.STSY902G.SYSLOGD.DIAG.$sufixo"
pasta="/home/cgdibm02/SYSLOGD_DIAG/"
#touch "$pasta$ficheiro"
#/home/cgdibm02/syslogdDownload xibmsi  $hoje $dias


ls -lh $pasta

echo "Criado $ficheiro, está no posto K2 na pasta home/SYSLOGD_DIAG"|mutt -a "$pasta$ficheiro" -s "SYSLOGD DIAG de $hoje" -- $enderecos

exit 0
