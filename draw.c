#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "drawfield.h"
#include "shm_data.h"
#define zeilen 15
#define spalten 40

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



      if(shm_pointer->field[0][0]==1){
        field[0][0]='#';
        field[0][1]='-';
      }if (shm_pointer->field[0][0]==2){
        field[0][0]='@';
        field[0][1]='-';
      }
      if(shm_pointer->field[0][1]==1){
        field[0][19]='#';
        field[0][20]='-';
      }if (shm_pointer->field[0][1]==2){
        field[0][19]='@';
        field[0][20]='-';
      }
      if(shm_pointer->field[0][2]==1){
        field[0][38]='-';
        field[0][39]='#';
      }if (shm_pointer->field[0][2]==2){
         field[0][38]='-';
        field[0][39]='@';
      }
      if(shm_pointer->field[0][3]==1){
        field[7][38]='-';
        field[7][39]='#';
      }if (shm_pointer->field[0][3]==2){
        field[7][38]='-';
        field[7][39]='@';
      }
      if(shm_pointer->field[0][4]==1){
        field[14][38]='-';
        field[14][39]='#';
      }if (shm_pointer->field[0][4]==2){
        field[14][38]='-';
        field[14][39]='@';

      }
      if(shm_pointer->field[0][5]==1){
        field[14][19]='#';
        field[14][20]='-';
      }if (shm_pointer->field[0][5]==2){
        field[14][19]='@';
        field[14][20]='-';
      }
      if(shm_pointer->field[0][6]==1){
        field[14][0]='#';
        field[14][1]='-';
      }if (shm_pointer->field[0][6]==2){
        field[14][0]='@';
        field[14][1]='-';
      } if(shm_pointer->field[0][7]==1){
        field[7][0]='#';
        field[7][1]='-';
      }if (shm_pointer->field[0][7]==2){
        field[7][0]='@';
        field[7][1]='-';
      }
      if(shm_pointer->field[1][0]==1){
        field[2][7]='#';
        field[2][8]='-';
      }if (shm_pointer->field[1][0]==2){
        field[2][7]='@';
        field[2][8]='-';
      }
      if(shm_pointer->field[1][1]==1){
        field[2][19]='#';
        field[2][20]='-';
      }if (shm_pointer->field[1][1]==2){
        field[2][19]='@';
        field[2][20]='-';
      }
      if(shm_pointer->field[1][2]==1){
        field[2][30]='-';
        field[2][31]='#';
      }if (shm_pointer->field[1][2]==2){
         field[2][30]='-';
        field[2][31]='@';
      }if(shm_pointer->field[1][3]==1){
        field[7][30]='-';
        field[7][31]='#';
      }if (shm_pointer->field[1][3]==2){
        field[7][30]='-';
        field[7][31]='@';
      }
      if(shm_pointer->field[1][4]==1){
        field[12][30]='-';
        field[12][31]='#';
      }if (shm_pointer->field[1][4]==2){
        field[12][30]='-';
        field[12][31]='@';
      }if(shm_pointer->field[1][5]==1){
        field[12][19]='#';
        field[12][20]='-';
      }if (shm_pointer->field[1][5]==2){
        field[12][19]='@';
        field[12][20]='-';
      }
      if(shm_pointer->field[1][6]==1){
        field[12][7]='#';
        field[12][8]='-';
      }if (shm_pointer->field[1][6]==2){
        field[12][7]='@';
        field[12][8]='-';
      }
      if(shm_pointer->field[1][7]==1){
        field[7][7]='#';
        field[7][8]='-';
      }if (shm_pointer->field[1][7]==2){
        field[7][7]='@';
        field[7][8]='-';
      }
      if(shm_pointer->field[2][0]==1){
        field[5][12]='#';
        field[5][13]='-';
      }if (shm_pointer->field[2][0]==2){
        field[5][12]='@';
        field[5][13]='-';
      }
      if(shm_pointer->field[2][1]==1){
        field[5][19]='#';
        field[5][20]='-';
      }if (shm_pointer->field[2][1]==2){
        field[5][19]='@';
        field[5][20]='-';
      }
      if(shm_pointer->field[2][2]==1){
        field[5][25]='-';
        field[5][26]='#';
      }if (shm_pointer->field[2][2]==2){
        field[5][25]='-';
        field[5][26]='@';
      }
      if(shm_pointer->field[2][3]==1){
        field[7][25]='-';
        field[7][26]='#';
      }if (shm_pointer->field[2][3]==2){
        field[7][25]='-';
        field[7][26]='@';
      }
      if(shm_pointer->field[2][4]==1){
        field[9][25]='#';
        field[9][26]='-';
      }if (shm_pointer->field[2][4]==2){
        field[9][25]='@';
        field[9][26]='-';
      }
      if(shm_pointer->field[2][5]==1){
        field[9][19]='#';
        field[9][20]='-';
      }if (shm_pointer->field[2][5]==2){
        field[9][19]='@';
        field[9][20]='-';
      }
      if(shm_pointer->field[2][6]==1){
        field[9][12]='#';
        field[9][13]='-';
      }if (shm_pointer->field[2][6]==2){
        field[9][12]='@';
        field[9][13]='-';
      }

      if(shm_pointer->field[2][7]==1){
        field[7][12]='#';
        field[7][13]='-';
      }if (shm_pointer->field[2][7]==2){
        field[7][12]='@';
        field[7][13]='-';
      }

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
