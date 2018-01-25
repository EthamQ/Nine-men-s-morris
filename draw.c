#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "drawfield.h"
#include "shm_data.h"
#define zeilen 15
#define spalten 40
#define ME "#"
#define OPPONENT "@"
#define ZEILENGRAFIK "-"
#define SPALTENGRAFIK "|"

void drawField(struct SHM_data* shm_pointer){

    char field[zeilen][spalten];
    int i;
    int j;


  //Feld mit Leerzeichen initialisieren
  for(i = 0; i<zeilen; i++){
    for(j = 0; j<spalten; j++){
      field[i][j] = ' ';
    }
  }

  //Äußeres Quadrat
  for(j=1; j<zeilen-1; j++){
    for(i = 0; i<spalten; i++){
      field[0][i] = ZEILENGRAFIK;
      field[zeilen-1][i] = ZEILENGRAFIK;
      field[j][0] = SPALTENGRAFIK;
      field[j][spalten-1] = SPALTENGRAFIK;
    }
  }

  //Mittleres Quadrat
  for(j = 3; j<12; j++){
    for(i = 7; i<32; i++){
      field[2][i] = ZEILENGRAFIK;
      field[12][i] = ZEILENGRAFIK;
      field[j][7] = SPALTENGRAFIK;
      field[j][31] =SPALTENGRAFIK;
    }
  }

  //Inneres Quadrat
  for(j = 6; j<9; j++){
    for(i = 12; i<27; i++){
      field[5][i] = ZEILENGRAFIK;
      field[9][i] = ZEILENGRAFIK;
      field[j][12] = SPALTENGRAFIK;
      field[j][26] = SPALTENGRAFIK;
    }
  }

  //Mittellinie von oben nach unten
  for(j = 1; j<14; j++){
    if(j<5 || j>9){
      field[j][19] = SPALTENGRAFIK;
    }
  }

  //Mittellinie von links nach rechts
  for(j=1; j<39; j++){
    if(j<12 || j>27){
      field[7][j] = ZEILENGRAFIK;
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



      if(shm_pointer->field[0][0]==1){
        field[0][0]=ME;
        field[0][1]=ZEILENGRAFIK;
      }if (shm_pointer->field[0][0]==2){
        field[0][0]=OPPONENT;
        field[0][1]=ZEILENGRAFIK;
      }
      if(shm_pointer->field[0][1]==1){
        field[0][19]=ME;
        field[0][20]=ZEILENGRAFIK;
      }if (shm_pointer->field[0][1]==2){
        field[0][19]=OPPONENT;
        field[0][20]=ZEILENGRAFIK;
      }
      if(shm_pointer->field[0][2]==1){
        field[0][38]=ZEILENGRAFIK;
        field[0][39]=ME;
      }if (shm_pointer->field[0][2]==2){
         field[0][38]=ZEILENGRAFIK;
        field[0][39]=OPPONENT;
      }
      if(shm_pointer->field[0][3]==1){
        field[7][38]=ZEILENGRAFIK;
        field[7][39]=ME;
      }if (shm_pointer->field[0][3]==2){
        field[7][38]=ZEILENGRAFIK;
        field[7][39]=OPPONENT;
      }
      if(shm_pointer->field[0][4]==1){
        field[14][38]=ZEILENGRAFIK;
        field[14][39]=ME;
      }if (shm_pointer->field[0][4]==2){
        field[14][38]=ZEILENGRAFIK;
        field[14][39]=OPPONENT;

      }
      if(shm_pointer->field[0][5]==1){
        field[14][19]=ME;
        field[14][20]=ZEILENGRAFIK;
      }if (shm_pointer->field[0][5]==2){
        field[14][19]=OPPONENT;
        field[14][20]=ZEILENGRAFIK;
      }
      if(shm_pointer->field[0][6]==1){
        field[14][0]=ME;
        field[14][1]=ZEILENGRAFIK;
      }if (shm_pointer->field[0][6]==2){
        field[14][0]=OPPONENT;
        field[14][1]=ZEILENGRAFIK;
      } if(shm_pointer->field[0][7]==1){
        field[7][0]=ME;
        field[7][1]=ZEILENGRAFIK;
      }if (shm_pointer->field[0][7]==2){
        field[7][0]=OPPONENT;
        field[7][1]=ZEILENGRAFIK;
      }
      if(shm_pointer->field[1][0]==1){
        field[2][7]=ME;
        field[2][8]=ZEILENGRAFIK;
      }if (shm_pointer->field[1][0]==2){
        field[2][7]=OPPONENT;
        field[2][8]=ZEILENGRAFIK;
      }
      if(shm_pointer->field[1][1]==1){
        field[2][19]=ME;
        field[2][20]=ZEILENGRAFIK;
      }if (shm_pointer->field[1][1]==2){
        field[2][19]=OPPONENT;
        field[2][20]=ZEILENGRAFIK;
      }
      if(shm_pointer->field[1][2]==1){
        field[2][30]=ZEILENGRAFIK;
        field[2][31]=ME;
      }if (shm_pointer->field[1][2]==2){
         field[2][30]=ZEILENGRAFIK;
        field[2][31]=OPPONENT;
      }if(shm_pointer->field[1][3]==1){
        field[7][30]=ZEILENGRAFIK;
        field[7][31]=ME;
      }if (shm_pointer->field[1][3]==2){
        field[7][30]=ZEILENGRAFIK;
        field[7][31]=OPPONENT;
      }
      if(shm_pointer->field[1][4]==1){
        field[12][30]=ZEILENGRAFIK;
        field[12][31]=ME;
      }if (shm_pointer->field[1][4]==2){
        field[12][30]=ZEILENGRAFIK;
        field[12][31]=OPPONENT;
      }if(shm_pointer->field[1][5]==1){
        field[12][19]=ME;
        field[12][20]=ZEILENGRAFIK;
      }if (shm_pointer->field[1][5]==2){
        field[12][19]=OPPONENT;
        field[12][20]=ZEILENGRAFIK;
      }
      if(shm_pointer->field[1][6]==1){
        field[12][7]=ME;
        field[12][8]=ZEILENGRAFIK;
      }if (shm_pointer->field[1][6]==2){
        field[12][7]=OPPONENT;
        field[12][8]=ZEILENGRAFIK;
      }
      if(shm_pointer->field[1][7]==1){
        field[7][7]=ME;
        field[7][8]=ZEILENGRAFIK;
      }if (shm_pointer->field[1][7]==2){
        field[7][7]=OPPONENT;
        field[7][8]=ZEILENGRAFIK;
      }
      if(shm_pointer->field[2][0]==1){
        field[5][12]=ME;
        field[5][13]=ZEILENGRAFIK;
      }if (shm_pointer->field[2][0]==2){
        field[5][12]=OPPONENT;
        field[5][13]=ZEILENGRAFIK;
      }
      if(shm_pointer->field[2][1]==1){
        field[5][19]=ME;
        field[5][20]=ZEILENGRAFIK;
      }if (shm_pointer->field[2][1]==2){
        field[5][19]=OPPONENT;
        field[5][20]=ZEILENGRAFIK;
      }
      if(shm_pointer->field[2][2]==1){
        field[5][25]=ZEILENGRAFIK;
        field[5][26]=ME;
      }if (shm_pointer->field[2][2]==2){
        field[5][25]=ZEILENGRAFIK;
        field[5][26]=OPPONENT;
      }
      if(shm_pointer->field[2][3]==1){
        field[7][25]=ZEILENGRAFIK;
        field[7][26]=ME;
      }if (shm_pointer->field[2][3]==2){
        field[7][25]=ZEILENGRAFIK;
        field[7][26]=OPPONENT;
      }
      if(shm_pointer->field[2][4]==1){
        field[9][25]=ME;
        field[9][26]=ZEILENGRAFIK;
      }if (shm_pointer->field[2][4]==2){
        field[9][25]=OPPONENT;
        field[9][26]=ZEILENGRAFIK;
      }
      if(shm_pointer->field[2][5]==1){
        field[9][19]=ME;
        field[9][20]=ZEILENGRAFIK;
      }if (shm_pointer->field[2][5]==2){
        field[9][19]=OPPONENT;
        field[9][20]=ZEILENGRAFIK;
      }
      if(shm_pointer->field[2][6]==1){
        field[9][12]=ME;
        field[9][13]=ZEILENGRAFIK;
      }if (shm_pointer->field[2][6]==2){
        field[9][12]=OPPONENT;
        field[9][13]=ZEILENGRAFIK;
      }

      if(shm_pointer->field[2][7]==1){
        field[7][12]=ME;
        field[7][13]=ZEILENGRAFIK;
      }if (shm_pointer->field[2][7]==2){
        field[7][12]=OPPONENT;
        field[7][13]=ZEILENGRAFIK;
      }

  for(i = 0; i<15; i++){
    for(j = 0; j<40; j++){
      if(j%40 == 0){ // ???
        printf("\n");
      }
    printf("%s", field[i][j]);
    }
  }
printf("\n");
}
