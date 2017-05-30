/*
 - AUTOR: EMANUEL BORGES DA SILVA
 - MATERIA: INTRODUÇÃO A ARQUITETURA DE COMPUTADORES
 - HORARIO: 24N34
 - ABRIL/2017
*/
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <string.h>
#include <iomanip>

using namespace std;

/*
@classe feita para armazenar o opcode e os registradores de cada instrução

*/
class instruction {
public:
  string opcode;
  string regis1;
  string regis2;
  string regis3;
  string dependencias_registrador[2];
  string valor_gerado;
  int dependencia_instrucao;
  int stage;

  instruction() {
  dependencia_instrucao = 99; 
  stage = 6; 
  }
};

/*
 - procedimento que retorna o numero de linhas de um arquivo
*/
int numero_de_linhas(int num_linhas){
    
    string line;
    ifstream myfile ("instrucoes.txt"); // ifstream = padrão ios:in
    if ( myfile.is_open() ){
      while ( !myfile.eof() ) { //enquanto end of file for false continua
          getline ( myfile,line ); // como foi aberto em modo texto(padrão)
          num_linhas++;
      }
      myfile.close();
    }
    return num_linhas;
}

int main(int argc, char * argv[]){
 	
  int i = 0; 
  int j = 0;
  int k = 0;

 	int num_linhas = 0;

 	string linhas[30];
 	string line;

  instruction vetor[numero_de_linhas(num_linhas)]; // - Vetor de objetos para armazenar cada linha do arquivo

	ifstream myfile ("instrucoes.txt");
    if ( myfile.is_open() ){
      while ( !myfile.eof() ) { 
        getline ( myfile,linhas[i] );
        i++;
      }
      myfile.close();
    }
    i = 0;
    string armazenar_informacoes[4] = {"0" "0" "0" "0"};
 /*
- Parte do codigo feita para pegar a linha e separa os opcode e os registraodres e colocar no vetor de string
para colocar no vetor de objetos instruction.
*/
    for(k = 0 ; k < numero_de_linhas(num_linhas) ; k++){
      line = linhas[k];
      string aux = "";  //Variável auxiliar para armazenar a palavra
      for (i = 0; i < line.length() ; i++){
        if (line[i] == ' '){
         armazenar_informacoes[j] = aux;
          aux = "";  //Limpa a variável auxiliar para armazena a próxima palavra
          j++;
        }
        if(line[i] == ',' or line[i] == '(' or line[i] == ')' or line[i] == ':'){}

        else if(line[i] == '$'){
          aux.clear();
          aux = aux + '$';
        }
        else if(line[i] == ' '){
        }
        else
          aux = aux + line[i]; //Armazena o caracter na variável auxiliar
      }
      if(line[i] == ',' or line[i] == '(' or line[i] == ')' or line[i] == ':'){

      }
      armazenar_informacoes[j] = aux;

      vetor[k].opcode =armazenar_informacoes[0];
      vetor[k].regis1 =armazenar_informacoes[1];
      vetor[k].regis2 =armazenar_informacoes[2];
      vetor[k].regis3 =armazenar_informacoes[3];
      j = 0;
      line.clear();
     armazenar_informacoes[0].clear();
     armazenar_informacoes[1].clear();
     armazenar_informacoes[2].clear();
     armazenar_informacoes[3].clear();
  }
//----------------------------------------------------------------------------------------
    /*
    - parte do codigo feita para verificar as depedencias de cada instrução
    */
    for(i = 0 ; i < numero_de_linhas(num_linhas) ; i++){
      if(vetor[i].opcode == "sub" or vetor[i].opcode == "add" ){
        vetor[i].regis1.resize(3);
      	vetor[i].regis2.resize(3);
      	vetor[i].regis3.resize(3);

        vetor[i].dependencias_registrador[0] = vetor[i].regis2;
        vetor[i].dependencias_registrador[1] = vetor[i].regis3;
        vetor[i].valor_gerado = vetor[i].regis1;
      }
      else if(vetor[i].opcode == "lw" ){
        vetor[i].regis2.resize(3);
        vetor[i].dependencias_registrador[0] = vetor[i].regis2;
        vetor[i].dependencias_registrador[1] = vetor[i].regis2;
        
        vetor[i].valor_gerado = vetor[i].regis1;
      }
      else if(vetor[i].opcode == "sw"){
      	vetor[i].regis1.resize(3);
      	vetor[i].regis2.resize(3);

        vetor[i].dependencias_registrador[0] = vetor[i].regis1;
        vetor[i].dependencias_registrador[1] = vetor[i].regis2;
        vetor[i].valor_gerado = "9999" ;  
      }
      else if(vetor[i].opcode == "bne" or vetor[i].opcode == "beq"){
        vetor[i].regis1.resize(3);
      	vetor[i].regis2.resize(3);

        vetor[i].dependencias_registrador[0] = vetor[i].regis1;
        vetor[i].dependencias_registrador[1] = vetor[i].regis2;
        vetor[i].valor_gerado = vetor[i].regis3;
        vetor[i].dependencia_instrucao = 1000;
      }
      else if(vetor[i].opcode == "j"){
        vetor[i].dependencias_registrador[0] = "999";
        vetor[i].dependencias_registrador[1] = "999";
        vetor[i].valor_gerado = "999";
      }
      else{
        vetor[i].dependencias_registrador[0] = "123";
        vetor[i].dependencias_registrador[1] = "123";
        vetor[i].valor_gerado = "123";  
      }
  }
// -----------------------------------------------------------------------------------------
/*
- Parte do codigo que trata dos jumps. foi criado um novo vetor de objetos, e foi colocado neste vetor apenas as instruções
que entram no pipeline
*/
    instruction v_atualizado[50];
    int f = 0;
    int instrucoes_at = 0;
    for(i = 0 ; i < numero_de_linhas(num_linhas) ; i++){

      if(vetor[i].valor_gerado != "123"){
        if(vetor[i].opcode == "bne" or vetor[i].opcode == "beq"){
          v_atualizado[i] = vetor[i];
          for(k = 0 ; k < numero_de_linhas(num_linhas) ; k++){
            if (vetor[i].regis3 == vetor[k].opcode){
              v_atualizado[i+1] = vetor[k+1];
              i = k;
            }
          }
        }
        else if(vetor[i].opcode == "j"){
          for(f = 0 ; f < numero_de_linhas(num_linhas) ; f++){
            if(vetor[i].regis1 == vetor[f].opcode){
              v_atualizado[i] = vetor[f+1];
              i = f+1;
            }
          }
        }
        else{
          v_atualizado[instrucoes_at] = vetor[i];  
        }
        instrucoes_at++;
      }
    }
 /*
  - Parte do codigo que trata da impressao das instruções  
 */
  int aux  = 0;

  for (i = 1 ; i > (-(instrucoes_at-1)) ; i--){
    v_atualizado[aux].stage = i;
    aux++;
  }
  for (i = 0 ; i < instrucoes_at ; i++){
    for(j = 0 ; j < i ; j++ ){
      if(v_atualizado[i].dependencias_registrador[0] == v_atualizado[j].valor_gerado
      or v_atualizado[i].dependencias_registrador[1] == v_atualizado[j].valor_gerado){
        v_atualizado[i].dependencia_instrucao = j;
      }
    }
  }
  int recebe;
  int dif = 0;
  for(i = 1 ; i < instrucoes_at ; i++ ){
      
    if(v_atualizado[i].dependencia_instrucao != 99){
      recebe = v_atualizado[i].dependencia_instrucao;
      dif = v_atualizado[recebe].stage - v_atualizado[i].stage;
      if(dif == 1 ){
        for(j = i ; j < instrucoes_at ; j++ ){
          v_atualizado[j].stage = v_atualizado[j].stage - 3;                
        }
      }
      if(dif == 2){
        for(j = i ; j < instrucoes_at ; j++ ){
          v_atualizado[j].stage = v_atualizado[j].stage - 2;
        }
      }
      if (dif == 3){
        for(j = i ; j < instrucoes_at ; j++ ){
          v_atualizado[j].stage = v_atualizado[j].stage - 1;
        }
      }
    }
  }  
    cout << "QUANTIDADE DE CICLOS TOTAL: " << ((1-v_atualizado[(instrucoes_at-1)].stage) + 5) << endl;
    for(i = 0 ; i < instrucoes_at ; i++ ){
      
      if(v_atualizado[i].dependencia_instrucao == 1000){
      }
      while(v_atualizado[i].stage <= 5 ){
        if (v_atualizado[i].stage == 1){
          cout << left << setw(4) << "IF";
        }
        if(v_atualizado[i].stage == 2){
          cout << left <<  setw(4) << "ID";  
        }
        if(v_atualizado[i].stage == 3){
          cout << left << setw(4) << "EX";
        }
        if(v_atualizado[i].stage == 4){
          cout << left << setw(4) << "MEM";
        }
        if(v_atualizado[i].stage == 5){
          cout << left << setw(4) << "WB";
        } 
        if(v_atualizado[i].stage < 1){
          cout << left << setw(4) << "*";
        }
        v_atualizado[i].stage++;
      }
      cout << endl;
    }
  return 0;
}