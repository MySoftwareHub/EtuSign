/*********************************************************************************************************************************************************************************************/

#include "Arduino.h"                                            
#include "K.h"                                               
#include <SoftwareSerial.h>                                  //Librairie gérant les liaisons séries       

/*********************************************************************************************************************************************************************************************/

//Prototypes des fonctions gérant le module NFC
void Envoie_octet_module(unsigned char command_data);       //Envoie des données au module 
void Envoie_octet_pc(unsigned char command_data);           //Envoie des données à l'ordinateur => pour debug
void Tableau_ACK(unsigned char temp);                       //Met les données dans un tableau de caractères
void Reveil_carte();                                        //"Réveil" du module NFC
void Lecture_tag();                                         //Lecture de cartes étudiantes
void Ecriture_console(unsigned char tem);                   //Affichage des données sur la console => pour debug
void LancementNFC();                                        //Lancement de la liaison série
String Recuperer_Etu();                                     //Récupération du morceau de trame utile

/*********************************************************************************************************************************************************************************************/


