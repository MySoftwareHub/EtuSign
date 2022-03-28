/*********************************************************************************************************************************************************************************************/

#include "Bluetooth.h"                                                                                            

/*********************************************************************************************************************************************************************************************/

SoftwareSerial module_BLUETOOTH( RXBLUETOOTH,TXBLUETOOTH);             //Construction de la liaison série virtuelle dédiée au module bluetooth            

/*********************************************************************************************************************************************************************************************/

void Parametrage_Bluetooth()                                           //Adresse MAC du module bluetooth : 00:6A:8E:16:C8:B2                       
{
  module_BLUETOOTH.begin(38400);                                       //On définit le débit de données à 38400 bauds/s
  module_BLUETOOTH.print("\r\n+STWMOD=0\r\n");                         //On met le module Bluetooth en mode esclave
  module_BLUETOOTH.print("\r\n+STNA=EtuSignArd\r\n");                  // On l'appelle "EtuSign"
  module_BLUETOOTH.print("\r\n+STPIN=1509\r\n");                       //On définit un code PIN qui est 1509 (date de création du projet Berry : 15/09)
  module_BLUETOOTH.print("\r\n+STOAUT=1\r\n");                         //On autorise les appareils appairés à se connecter directement avec le module bluetooth                                                                                     
  module_BLUETOOTH.print("\r\n+STAUTO=0\r\n");                         //On dit que l'auto-connexion est autorisée
  delay(2000);                                                         //On définit un délai de 2 secondes (obligatoire)
  module_BLUETOOTH.print("\r\n+INQ=1\r\n");                            //On dit que la connexion bluetooth est détectable
  delay(2000);                                                         //On définit un délai de 2 secondes (obligatoire)
  module_BLUETOOTH.flush();                                            //On vide le buffer de la liaison série
}

void Ecrire_Bluetooth(String msg)                                      //Fonction qui écrit sur la liaison série
{           
  module_BLUETOOTH.println("##+" + msg);                              //Ecrit un message précédé de deux caractères nuls sur la liaison série                                                           
}

String Lire_Bluetooth()                                                 //Fonction qui lit sur la liaison série
{
  String Message;

  /*"Ecouter" sur la liaison série 
   * tant que le module ne reçoit
   * rien
   */
  module_BLUETOOTH.listen();
  while(!module_BLUETOOTH.available());                               

  /*Mettre l'ordre dans une variable 
   * dès que le module bluetooth
   * le reçoit
   */
  while(module_BLUETOOTH.available())
  {
    delay(3);
    char c = module_BLUETOOTH.read();
    Message += c;
  }
  return Message;
}

void LancementBt()                                                        //Fonction lançant la liaison série et qui la rend détectable                                                                               
{
  module_BLUETOOTH.begin(38400);                                          //Initialisation de la liaison série à 38400 bauds 
  delay(2000);
  module_BLUETOOTH.print("\r\n+INQ=1\r\n");                               //On dit que la connexion bluetooth est détectable
  delay(2000);
  module_BLUETOOTH.flush();                                               //On vide le buffer de la liaison série            
}

/*********************************************************************************************************************************************************************************************/



