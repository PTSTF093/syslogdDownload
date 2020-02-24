#!/bin/bash

#
#	PRIMEIRO, FAZER DOWNLOAD VIA FILEZILLA DOS FICHEIROS G08*V* PARA HOME FOLDER.
#	DEPOIS REMOVER GALL.csv
#
#
cd /home/MarlonSimoni/SYSLOGD_DIAG/
#rm GALL.csv

# 
# 
#	CONCATENAR OS FICHEIROS G0* EM GALL.csv 
# 
#cat G*V* | tr '.' ':' > GALL.csv
cat SYP.D.STSY902G.SYSLOGD.DIAG* | tr '.' ':' > GALL.csv

#
#
#	SUBSTITUIR '.' POR ':' PARA AS HORAS SEREM LEGÍVEIS NA BASE DE DADOS NO FORMATO TIME
#
#sed -i 's/\./\:/g' GALL.csv

#
#
#	Limpar a tabela temporária
#
echo "última linha antes de atualizar, dados do último dia carregado:"
mysql -u root -popenpipes -h 127.0.0.1 auditoria -e "select * from batch_tempo_execucao_temp order by id desc limit 1"
mysql -u root -popenpipes -h 127.0.0.1 auditoria -e "TRUNCATE batch_tempo_execucao_temp;"

#
#
#	Importar o ficheiro GALL.csv para esta mesma tabela
#
mysql -u root -popenpipes -h 127.0.0.1 auditoria -e "LOAD DATA LOCAL INFILE '/home/MarlonSimoni/SYSLOGD_DIAG/GALL.csv'  INTO TABLE batch_tempo_execucao_temp FIELDS TERMINATED BY '  ' ENCLOSED BY '' LINES TERMINATED BY '\n' (job,data,hora_inicio,hora_fim,elapsed);"

#
#
#	Atualizar dias fmes, domingo e quinta
#
#
mysql -u root -popenpipes -h 127.0.0.1 auditoria -e "update batch_tempo_execucao_temp set fmes=1 where dayofmonth(data)=1;"
mysql -u root -popenpipes -h 127.0.0.1 auditoria -e "update batch_tempo_execucao_temp set quinta=1 where dayofweek(data)=5;"
mysql -u root -popenpipes -h 127.0.0.1 auditoria -e "update batch_tempo_execucao_temp set domingo=1 where dayofweek(data)=1;"

#
#
#	Verificar o estado da tabela temporária antes de prosseguir com o insert na tabela real
#
echo "Verificar agora o estado de auditoria.batch_tempo_execucao_temp."
echo "primeira linha:"
mysql -u root -popenpipes -h 127.0.0.1 auditoria -e "select * from batch_tempo_execucao_temp order by id asc limit 1"
echo "última linha:"
mysql -u root -popenpipes -h 127.0.0.1 auditoria -e "select * from batch_tempo_execucao_temp order by id desc limit 1"
echo "Prima enter para continuar....."
read
echo "....iniciando..."

#
#
#	Atualizar dias fmes, domingo e quinta
#
#
echo "==========Manipulando datas localmente"
mysql -u root -popenpipes -h 127.0.0.1 auditoria -e "update batch_tempo_execucao set fmes=1 where dayofmonth(data)=1;"
mysql -u root -popenpipes -h 127.0.0.1 auditoria -e "update batch_tempo_execucao set quinta=1 where dayofweek(data)=5;"
mysql -u root -popenpipes -h 127.0.0.1 auditoria -e "update batch_tempo_execucao set domingo=1 where dayofweek(data)=1;"

#
#
#	Fazer o insert
#
echo "==========Inserir na tabela localmente"
mysql -u root -popenpipes -h 127.0.0.1 auditoria -e "insert into  batch_tempo_execucao(job,data,hora_inicio,hora_fim,elapsed,fmes,domingo,quinta) SELECT job,data,hora_inicio,hora_fim,elapsed,fmes,domingo,quinta FROM batch_tempo_execucao_temp;"

#
#
#	Exportar a tabela
#
echo "==========Export dump localmente para batch_tempo_execucao.sql"
mysqldump -u root -popenpipes auditoria batch_tempo_execucao > /home/MarlonSimoni/batch_tempo_execucao.sql

#
#
#	Fazer upload da tabela para o N3
#
echo "==========Upload para N3 do ficheiro sql"
ssh ptuser@9.172.81.97 'rm /home/ptuser/batch_tempo_execucao.sql'
rsync -I --progress /home/MarlonSimoni/batch_tempo_execucao.sql ptuser@9.172.81.97:/home/ptuser/

#
#
#	Fazer upload da tabela para o 92
#
echo "==========Upload para N2 do ficheiro sql"
ssh cgdibm02@9.172.85.92 'rm /home/cgdibm02/batch_tempo_execucao.sql'
rsync -I --progress /home/MarlonSimoni/batch_tempo_execucao.sql cgdibm02@9.172.85.92:/home/cgdibm02/

#
#
#	Remotamente fazer o import da tabela exportada, dentro da máquina N3
#
echo "==========Invocar script na máquina destino N3 utilizando o ficheiro sql no destino"
USERNAME=ptuser
HOSTS="9.172.81.97"
SCRIPT="mysql -u root -popenpipes -h 127.0.0.1 auditoria < /home/ptuser/batch_tempo_execucao.sql;"

for HOSTNAME in ${HOSTS} ; do
    ssh -l ${USERNAME} ${HOSTNAME} "${SCRIPT}"
done

#
#
#	Remotamente fazer o import da tabela exportada, dentro da máquina 92
#
echo "==========Invocar script na máquina destino N2 utilizando o ficheiro sql no destino"
USERNAME=cgdibm02
HOSTS="9.172.85.92"
SCRIPT="mysql -u root -popenpipes -h 127.0.0.1 auditoria < /home/cgdibm02/batch_tempo_execucao.sql;"

for HOSTNAME in ${HOSTS} ; do
    ssh -l ${USERNAME} ${HOSTNAME} "${SCRIPT}"
done

#
#
#	Terminar
#
echo "==========Eliminar localmente os ficheiro importados do mainframe recebidos diariamente via mail"
rm -f SYP.D.STSY902G.SYSLOGD.DIAG*
echo "conteúdo da pasta:"
ls -lh 
echo "------------------------------"
echo "terminado... prima enter para terminar o script!"
read
exit 0
