/*********************************************************************************************************************************************************************************************/

#include "Arduino.h" 
#include "K.h"                                                                                              
#include <SoftwareSerial.h>                            //Librairie gérant les liaisons séries                                      

/*********************************************************************************************************************************************************************************************/

//Prototypes des fonctions gérant le module bluetooth
void Parametrage_Bluetooth();                         //Configuration du module bluetooth                              
void Ecrire_Bluetooth(String msg);                    //Ecriture sur la liaison série
String Lire_Bluetooth();                              //Lecture sur la liaison série
void LancementBt();                                   //Lancement de la liaison série

/*********************************************************************************************************************************************************************************************/

