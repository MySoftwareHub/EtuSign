/*********************************************************************************************************************************************************************************************/

#include "NFC.h"                                                                                                                                               

//Choix de la librairie suivant la version d'Arduino              
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#define printOctet(args) module_NFC.write(args)
#define printlnOctet(args)  module_NFC.write(args),module_NFC.println()
#else
#include "WProgram.h"
#define printOctet(args) module_NFC.print(args,BYTE)
#define printlnOctet(args)  module_NFC.println(args,BYTE)
#endif

/*********************************************************************************************************************************************************************************************/

SoftwareSerial module_NFC(RXNFC,TXNFC);               //Construction de la liaison série virtuelle dédiée au module NFC

/*********************************************************************************************************************************************************************************************/

const unsigned char Reveil_module[24]={0x55,0x55,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x03,0xfd,0xd4,0x14,0x01,0x17,0x00};
const unsigned char Tag[11]={0x00,0x00,0xFF,0x04,0xFC,0xD4,0x4A,0x01,0x00,0xE1,0x00};
unsigned char Recevoir_ACK[TAILLE_CARTE];

void Envoie_octet_module(unsigned char command_data)   //Envoie de données au module
{
  module_NFC.listen();
  
  printOctet(command_data);
  #if defined(ARDUINO) && ARDUINO >= 100
    module_NFC.flush();
  #endif
} 

void Envoie_octet_pc(unsigned char command_data)   //Envoie de données au PC
{
  Serial.print(command_data,HEX);
  Serial.print(" ");
} 

void Tableau_ACK(unsigned char temp)
{
  for(unsigned char i=0;i<temp;i++)                                           
  {
    Recevoir_ACK[i]= module_NFC.read();
    delay(3);
  }
}

void Reveil_carte(void)                       //Réveil du module 
{
  unsigned char i;
  for(i=0;i<24;i++) 
  Envoie_octet_module(Reveil_module[i]);
}

void Lecture_tag(void)                        //Lecture de la carte étudiante
{
  unsigned char i;
  for(i=0;i<11;i++) 
  Envoie_octet_module(Tag[i]);
}

void Ecriture_console(unsigned char tem)      //Affichage sur la console
{
  unsigned char i;
  for(i=0;i<tem;i++) 
  Envoie_octet_pc(Recevoir_ACK[i]);
  Serial.println();
}

void LancementNFC()                           //Lancement de la liaison série
{
  module_NFC.begin(115200);
}

String Recuperer_Etu()                        //Fonction récupérant juste le morceau de trame utile
{
  if (Recevoir_ACK[6] != 0xFF)
  {
    if (Recevoir_ACK[3] != 0x15)
    {
      if (Recevoir_ACK[21] != 0xFF)
      {
        String Code;
        Code = String(Recevoir_ACK[20], HEX);
        Code += " ";
        Code += String(Recevoir_ACK[21], HEX);
        return Code;
      }
    }
  }
  return "";
}

/*********************************************************************************************************************************************************************************************/


