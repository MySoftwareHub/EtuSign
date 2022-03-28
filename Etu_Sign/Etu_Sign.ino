/********************************************************************************
Projet      :  Berry ************************************************************
Objet       :  Programme pour le système EtuSign - Module Lecture de cartes *****
Concepteurs :  AUBERT, DECHENE, HENOT, MOURIER **********************************
Classe      :  GEII2 ************************************************************
Année       :  2017/2018 ********************************************************
********************************************************************************/

#include "K.h"                                                  //Appel de la librairie gérant les constantes                         
#include "NFC.h"                                                //Appel de la librairie gérant le module NFC
#include "Bluetooth.h"                                          //Appel de la librairie gérant le module bluetooth
#include "LCD.h"                                                //Appel de la librairie gérant l'écran LCD                           
#include <avr/wdt.h>                                            //Librairie gérant le watchdog du microcontroleur

/*********************************************************************************************************************************************************************************************/

//Variables
int NbrEtu = 0;                                                 //Variable qui reçoit le nombre d'étudiants
int NbrCartes = 0;                                              //Variable qui reçoit le nombre de cartes étudiantes
bool FraudeDet = false;

//Prototypes
void AssociationCartes();

/*********************************************************************************************************************************************************************************************/

void setup()                                                    //Configuration du programme pricipal
{
  LancementLCD();                                               //Configuration de l'écran LCD
  Affichage_LCD("EtuSign 1.0", "Initialisation.", false);       //Affichage du message de départ et pas de nettoyage de l'écran
  
  LancementNFC();                                               //Démarrage de la communication avec le module NFC
  LancementBt();                                                //Démarrage de la communication avec le module bluetooth
  //Parametrage_Bluetooth();                                    //Paramétrage du module bluetooth : à n'utiliser qu'une seule fois si pas de modifications de paramètres (ils restent en mémoire dans le module)
  
  Serial.begin(9600);                                           //Démarrage de la communication avec le PC : vitesse 9600 bauds
    
  pinMode(LED_BLANCHE, OUTPUT);                                 //Configuration de la pin en sortie
  pinMode(LED_VERTE, OUTPUT);                                   //Configuration de la pin en sortie
  pinMode(LED_ROUGE, OUTPUT);                                   //Configuration de la pin en sortie
  pinMode(BOUTON_RAZ, INPUT);                                   //Configuration de la pin en entrée                                
        
  Reveil_carte();                                               //"Réveil" du module NFC
  delay(100);                                                   //Délai de cent millisecondes
    
  analogWrite(LED_BLANCHE, 15);                                 //Diminution de la puissance de la LED blanche
  
  String Cmd = Lire_Bluetooth();                                //On attend la connexion avec le Raspberry Pi
  while(Cmd != "V"){
    Cmd = Lire_Bluetooth();
    delay(20);
  }
  
  Ecrire_Bluetooth(VERSION_FIRMWARE);                           //Envoi de la version du firmware Arduino

  wdt_enable(WDTO_8S);                                          //Watchdog de 8 secondes (compteur) : remise à zéro du programme en cas de plantage (sécurité)
  Affichage_LCD("EtuSign 1.0", "Pret.", true);                  //Affichage du message disant que la connexion est prête
  delay(2000);                                                  //Délai de deux secondes
}

/*********************************************************************************************************************************************************************************************/

void loop()                                                     
{
  String CodeEtu;
  String Etat = "";
  String Infos = "";
  
  while(Serial.available()){
    Etat += (char)Serial.read();
    delay(3);
  }
  if(Etat == "ESG"){
    Affichage_LCD("Association", "Cartes", true);
    wdt_reset();
    AssociationCartes();
  }

  Lecture_tag();
  delay(20);
  Tableau_ACK(TAILLE_CARTE);

  CodeEtu = Recuperer_Etu();
  
  if(CodeEtu != ""){
    Ecrire_Bluetooth(CodeEtu);

    Etat = Lire_Bluetooth();
    if(Etat == "OK"){
      NbrCartes++;
      
      digitalWrite(LED_VERTE, HIGH);
      delay(2000);
      digitalWrite(LED_VERTE, LOW);
    }else if(Etat == "E"){
      //Rien à exécuter, on relit la carte
    }else if(Etat == "E1"){
      for(int i = 0; i <= 4; i++){
        digitalWrite(LED_ROUGE, HIGH);
        delay(250);
        digitalWrite(LED_ROUGE, LOW);
        delay(250);
      }
      if(FraudeDet){
        digitalWrite(LED_ROUGE, HIGH);
      }
    }
  }

  Ecrire_Bluetooth("Infos");
  
  Infos = Lire_Bluetooth();
  if(Infos.indexOf('-') != -1){
    if(NbrEtu != 0){
      NbrEtu = NbrEtu - Infos.substring(1, Infos.indexOf('#')).toInt();
    }   
  }else{
    NbrEtu = NbrEtu + Infos.substring(0, Infos.indexOf('#')).toInt();
  }
  if(Infos.substring(Infos.indexOf("#")+1) == "1"){
    digitalWrite(LED_ROUGE, HIGH);
    FraudeDet = true;
  }
  Affichage_LCD("Nb. pers.  : " + String(NbrEtu) + ' ', "Nb. cartes : " + String(NbrCartes) + ' ', false);

  if(digitalRead(BOUTON_RAZ) == HIGH){
    Ecrire_Bluetooth("RAZ");
    digitalWrite(LED_ROUGE, LOW);
    NbrEtu = 0;
    NbrCartes = 0;
    FraudeDet = false;
  }
  
  wdt_reset();                                                  //Remise à zéro du compteur du watchdog
  delay(500);         
}

void AssociationCartes()
{
  String CodeEtu;
  String NEtu;
  String Etat;

  /* Envoi de la commande au Raspberry Pi
   * pour se mettre en configuration "Association Cartes"
   */
  Ecrire_Bluetooth("ESG");

  /* On récupère la réponse.
   * Le code OK signifie qu'on est prêt.
   * Un code différent provoque la sortie
   * du mode "Association Cartes"
   */
  Etat = Lire_Bluetooth();
  if(Etat == "OK"){
    /* Indication au logiciel qu'on est connecté
     * et en mode d'association.
     */
    Serial.println("OK");
    delay(50); //Délai nécéssaire pour séparer les envois sur le port série

    /* On récupère chaque n° d'étudiant associé
     * et on l'envoie au logiciel.
     * Lorsque qu'on reçoit le code OK:F,
     * la mise à jour est terminée.
     */
    NEtu = Lire_Bluetooth();  
    while(NEtu != "OK:F"){
      Serial.println(NEtu);
      NEtu = Lire_Bluetooth();
      delay(20);
    }

    Serial.println("OK:F");
    NEtu = "";

    /* Boucle infinie tant qu'on ne termine
     * pas la configuration avec la commande
     * _ESG.
     * En fonction du code (par défaut le n°
     * d'étudiant), on effectue :
     *  - Un ajout d'une association carte<->étudiant
     *  - Une suppression d'une assication carte<->étudiant
     */
    while(NEtu != "_ESG"){
      //Lecture d'un éventuel code
      while(Serial.available()){
        NEtu += (char)Serial.read();
        delay(3);
      }

      //Vérification de la présence d'un code d'association
      if(NEtu != "" && NEtu != "_ESG"){
        if (NEtu.indexOf("S") == 0){
          //On supprime un étudiant (S:21xxxxx)
          Ecrire_Bluetooth(NEtu);
        }else{
          //On tente de lire une carte étudiante dans un temps imparti (~3s)
          for(int i = 0; i < 5; i++){
            Lecture_tag();
            delay(20);
            Tableau_ACK(TAILLE_CARTE);
    
            CodeEtu = Recuperer_Etu();
      
            if(CodeEtu != ""){
              Ecrire_Bluetooth(CodeEtu + ":" + NEtu); //Envoi d'une trame d'association

              //On vérifie que la carte ait bien été associée à un étudiant
              Etat = Lire_Bluetooth();
              if(Etat == "OK:CE"){
                Serial.println("OK:CE"); //On signale l'association au logiciel
                
                digitalWrite(LED_VERTE, HIGH);
                delay(2000);
                digitalWrite(LED_VERTE, LOW);
  
                break; //On peut donc sortir de la boucle
              }      
            }

            delay(500); //Délai de 500ms avant rebouclage et relecture
          } //Fin de la boucle for

          /* Si en sortant de la boucle, aucune association
           * n'a été effectuée, on signale une erreur au logiciel.
           */
          if(Etat != "OK:CE"){
            Serial.println("E:CE");
          }    
        }
        
        NEtu = "";  //On réinitialise le code avant de reboucler
      }

      //RAZ du Watchdog et délai avant rebouclage
      wdt_reset();
      delay(20);
    }

    /* On quitte la boucle d'association
     * et on envoie au Raspberry Pi la commande pour
     * valider les associations (sortie du mode
     * "Association Cartes").
     */
    Ecrire_Bluetooth("_ESG");
  }
  // ### Sortie du mode "Association Cartes" ### //
}

/*********************************************************************************************************************************************************************************************/

