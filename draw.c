#include <stdio.h>
#include <stdlib.h>
#define zeilen 15
#define spalten 40
#include "debugging.h"

//Gibt ein Mühlespielfeld auf der Konsole aus
//TODO: übersichtlicher schreiben und Koordinaten speichern wo man Spielsteine ablegen kann
void drawField(){
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

  //Feld zeichnen
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
