/*********************************************************************************************************************************************************************************************/

#include "LCD.h"

/*********************************************************************************************************************************************************************************************/

LiquidCrystal_I2C lcd(0x20,16,2);                             //Construction de la liaison I2C avec mise en paramètre de l'API de l'écran

/*********************************************************************************************************************************************************************************************/

void Affichage_LCD(String l1, String l2, bool Effacement)     //Fonction de gestion de l'affichage
{
  if(Effacement) lcd.clear();                                 //Condition de nettoyage de l'écran
  
  lcd.setCursor(0,0);                                         //Sélection de la position du curseur : première ligne
  lcd.print(l1);                                              //Ecriture sur la première ligne

  lcd.setCursor(0,1);                                         //Sélection de la position du curseur : seconde ligne
  lcd.print(l2);                                              //Ecriture sur la seconde ligne
}

void LancementLCD ()                                          //Fonction de configuration de départ de l'écran LCD
{
  lcd.init();                                                 //Initialisation de l'écran
  lcd.backlight();                                            //Allumage du rétro éclairage
}

/*********************************************************************************************************************************************************************************************/

