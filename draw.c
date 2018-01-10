#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define zeilen 15
#define spalten 40
#include "debugging.h"

//Gibt ein Mühlespielfeld auf der Konsole aus
//TODO: übersichtlicher schreiben und Koordinaten speichern wo man Spielsteine ablegen kann
//int player=0;
char field[zeilen][spalten];
  int i;
  int j;


void drawField(){
  

  //Feld mit Leerzeichen initialisieren
  for(i = 0; i<zeilen; i++){
    for(j = 0; j<spalten; j++){
      field[i][j] = ' ';
    }
  }

  //Äußeres Quadrat
  for(j=1; j<zeilen-1; j++){
    for(i = 0; i<spalten; i++){
      field[0][i] = '-';
      field[zeilen-1][i] = '-';
      field[j][0] = '|';
      field[j][spalten-1] = '|';
    }
  }

  //Mittleres Quadrat
  for(j = 3; j<12; j++){
    for(i = 7; i<32; i++){
      field[2][i] = '-';
      field[12][i] = '-';
      field[j][7] = '|';
      field[j][31] ='|';
    }
  }

  //Inneres Quadrat
  for(j = 6; j<9; j++){
    for(i = 12; i<27; i++){
      field[5][i] = '-';
      field[9][i] = '-';
      field[j][12] = '|';
      field[j][26] = '|';
    }
  }

  //Mittellinie von oben nach unten
  for(j = 1; j<14; j++){
    if(j<5 || j>9){
      field[j][19] = '|';
    }
  }

  //Mittellinie von links nach rechts
  for(j=1; j<39; j++){
    if(j<12 || j>27){
      field[7][j] = '-';
    }
  }
  field[0][0]='A';
  field[0][1]='0';
  field[0][19]='A';
  field[0][20]='1';
  field[0][38]='A';
  field[0][39]='2';

  field[2][7]='B';
  field[2][8]='0';
  field[2][19]='B';
  field[2][20]='1';
  field[2][30]='B';
  field[2][31]='2';

  field[5][12]='C';
  field[5][13]='0';
  field[5][19]='C';
  field[5][20]='1';
  field[5][25]='C';
  field[5][26]='2';

  field[7][0]='A';
  field[7][1]='7';
  field[7][7]='B';
  field[7][8]='7';
  field[7][12]='C';
  field[7][13]='7';

  field[7][25]='C';
  field[7][26]='3';
  field[7][30]='B';
  field[7][31]='3';
  field[7][38]='A';
  field[7][39]='3';

  field[9][12]='C';
  field[9][13]='6';
  field[9][19]='C';
  field[9][20]='5';
  field[9][25]='C';
  field[9][26]='4';

  field[12][7]='B';
  field[12][8]='6';
  field[12][19]='B';
  field[12][20]='5';
  field[12][30]='B';
  field[12][31]='4';

  field[14][0]='A';
  field[14][1]='6';
  field[14][19]='A';
  field[14][20]='5';
  field[14][38]='A';
  field[14][39]='4';








  for(i = 0; i<15; i++){
    for(j = 0; j<40; j++){
      if(j%40 == 0){ // ???
        printf("\n");
      }
    printf("%c", field[i][j]);
    }
  }
printf("\n");
}



void parseMove(char str[],int player2){


  char *move=malloc(strlen(str) + 1); 
  strcpy(move,str);
  int player=player2;
  

  char *token;
  char *token2;
  


  /* get the first token */
  token = strtok(move, ":");


  token2 = strtok(NULL, ":");
  printf( " %s\n", token );

     
  printf( " %s\n", token2 );

  if (strcmp(token, "A0") == 0) {
    field[0][0]='A';
    field[0][1]='0';
  }else if(strcmp(token,"A1")==0){
    field[0][19]='A';
    field[0][20]='1';
  }else if(strcmp(token,"A2")==0){
    field[0][38]='A';
    field[0][39]='2';
  }else if(strcmp(token,"B0")==0){
    field[2][7]='B';
    field[2][8]='0';
  }else if(strcmp(token,"B1")==0){
    field[2][19]='B';
    field[2][20]='1';
  }else if(strcmp(token,"B2")==0){
    field[2][30]='B';
    field[2][31]='2';
  }else if(strcmp(token,"C0")==0){
    field[5][12]='C';
    field[5][13]='0';
  }else if(strcmp(token,"C1")==0){
    field[5][19]='C';
    field[5][20]='1';
  }else if(strcmp(token,"C2")==0){
    field[5][25]='C';
    field[5][26]='2';
  }else if(strcmp(token,"A7")==0){
    field[7][0]='A';
    field[7][1]='7';
  }else if(strcmp(token,"B7")==0){
    field[7][7]='B';
    field[7][8]='7';
  }else if(strcmp(token,"C7")==0){
    field[7][12]='C';
    field[7][13]='7';
  }else if(strcmp(token,"C3")==0){
    field[7][25]='C';
    field[7][26]='3';
  }else if(strcmp(token,"B3")==0){
    field[7][30]='B';
    field[7][31]='3';
  }else if(strcmp(token,"A3")==0){
    field[7][38]='A';
    field[7][39]='3';
  }else if(strcmp(token,"C6")==0){
    field[9][12]='C';
    field[9][13]='6';
  }else if(strcmp(token,"C5")==0){
    field[9][19]='C';
    field[9][20]='5';
  }else if(strcmp(token,"C4")==0){
    field[9][25]='C';
    field[9][26]='4';
  }else if(strcmp(token,"B6")==0){
    field[12][7]='B';
    field[12][8]='6';
  }else if(strcmp(token,"B5")==0){
    field[12][19]='B';
    field[12][20]='5';
  }else if(strcmp(token,"B4")==0){
    field[12][30]='B';
    field[12][31]='4';
  }else if(strcmp(token,"A6")==0){
    field[14][0]='A';
    field[14][1]='6';
  }else if(strcmp(token,"A5")==0){
    field[14][19]='A';
    field[14][20]='5';
  }else if(strcmp(token,"A4")==0){
    field[14][38]='A';
    field[14][39]='4';
  }else{
    printf("Kein Gültiger Zug?!?!? draw.c\n");
  }
  

  if (strcmp(token2, "A0") == 0) {
    if(player==0){
      
      field[0][0]='#';
     
    }else{
      field[0][0]='+';
    }
    field[0][1]='-';
  }else if(strcmp(token2,"A1") == 0){
    if(player==0){
      field[0][19]='#';
    }else{
      field[0][19]='+';
    }
    field[0][20]='-';
  }else if(strcmp(token2,"A2")== 0){
     if(player==0){
      field[0][39]='#';
    }else{
      field[0][39]='+';
    }
    field[0][38]='-';
  }else if(strcmp(token2,"B0")== 0){
     if(player==0){
      field[2][7]='#';
    }else{
      field[2][7]='+';
    }
    field[2][8]='-';
  }else if(strcmp(token2,"B1")== 0){
     if(player==0){
      field[0][19]='#';
    }else{
      field[0][19]='+';
    }
    field[0][20]='-';
  }else if(strcmp(token2,"B2")== 0){
     if(player==0){
      field[2][31]='#';
    }else{
      field[2][31]='+';
    }
    field[2][30]='-';
  }else if(strcmp(token2,"C0")== 0){
     if(player==0){
      field[5][12]='#';
    }else{
      field[5][12]='+';
    }
    field[5][13]='-';
  }else if(strcmp(token2,"C1")== 0){
     if(player==0){
      field[5][19]='#';
    }else{
      field[5][19]='+';
    }
    field[5][20]='-';
  }else if(strcmp(token2,"C2")== 0){
     if(player==0){
      field[5][26]='#';
    }else{
      field[5][26]='+';
    }
    field[5][25]='-';
  }else if(strcmp(token2,"A7")== 0){
     if(player==0){
      field[7][0]='#';
    }else{
      field[7][0]='+';
    }
    field[7][1]='-';
  }else if(strcmp(token2,"B7")== 0){
     if(player==0){
      field[7][7]='#';
    }else{
      field[7][7]='+';
    }
    field[7][8]='-';
  }else if(strcmp(token2,"C7")== 0){
     if(player==0){
      field[7][12]='#';
    }else{
      field[7][12]='+';
    }
    field[7][13]=' ';
  }else if(strcmp(token2,"C3")== 0){
     if(player==0){
      field[7][26]='#';
    }else{
      field[7][26]='+';
    }
    field[7][25]=' ';
  }else if(strcmp(token2,"B3")== 0){
     if(player==0){
      field[7][31]='#';
    }else{
      field[7][31]='+';
    }
    field[7][30]='-';
  }else if(strcmp(token2,"A3")== 0){
     if(player==0){
      field[7][39]='#';
    }else{
      field[7][39]='+';
    }
    field[7][38]='-';
  }else if(strcmp(token2,"C6")== 0){
     if(player==0){
      field[9][12]='#';
    }else{
      field[9][12]='+';
    }
    field[9][13]='-';
  }else if(strcmp(token2,"C5")== 0){
     if(player==0){
      field[9][19]='#';
    }else{
      field[9][19]='+';
    }
    field[9][20]='-';
  }else if(strcmp(token2,"C4")== 0){
     if(player==0){
      field[9][26]='#';
    }else{
      field[9][26]='+';
    }
    field[9][25]='-';
  }else if(strcmp(token2,"B6")== 0){
     if(player==0){
      field[12][7]='#';
    }else{
      field[12][7]='+';
    }
    field[12][8]='-';
  }else if(strcmp(token2,"B5")== 0){
     if(player==0){
      field[12][19]='#';
    }else{
      field[12][19]='+';
    }
    field[12][20]='-';
  }else if(strcmp(token2,"B4")== 0){
     if(player==0){
      field[12][31]='#';
    }else{
      field[12][31]='+';
    }
    field[12][30]='-';
  }else if(strcmp(token2,"A6")== 0){
     if(player==0){
      field[14][0]='#';
    }else{
      field[14][0]='+';
    }
    field[14][1]='-';
  }else if(strcmp(token2,"A5")== 0){
     if(player==0){
      field[14][19]='#';
    }else{
      field[14][19]='+';
    }
    field[14][20]='-';
  }else if(strcmp(token2,"A4")== 0){
    if(player==0){
      field[14][39]='#';
    }else{
      field[14][39]='+';
    }
    field[14][38]='-';
  }else{
    printf("Kein Gültiger Zug?!?!? draw.c\n");
  }
  free(move);
  printf("\033[18A");
  printf("\033[39D");



  for(i = 0; i<15; i++){
    for(j = 0; j<40; j++){
      if(j%40 == 0){ // ???
        printf("\n");
      }
    printf("%c", field[i][j]);
    }
  }
printf("\n");
}


void setStone(int x,int y,char c){
  field[x][y]=c;
  field[x][y+1]=' ';
}
void drawField2(){}


