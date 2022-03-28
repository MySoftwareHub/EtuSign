/*********************************************************************************************************************************************************************************************/

#include "Arduino.h"  
#include <Wire.h>                                                 //Librairie gérant l'I2C
#include "src/LiquidCrystal_I2C-1.1.2/LiquidCrystal_I2C.h"        //Inclusion de la librairie portable gérant le programmateur de l'écran LCD

/*********************************************************************************************************************************************************************************************/

//Prototypes des fonctions gérant l'écran LCD
void Affichage_LCD(String l1, String l2, bool Effacement);        //l1 : Ligne 1 , l2 : Ligne 2 , Effacement : nettoyage de l'écran avant écriture //Gestion de l'affichage
void LancementLCD();                                              //Configuration de départ de l'écran LCD

/*********************************************************************************************************************************************************************************************/

