/**
 * @addtogroup pilotes_arm_gpio
 * Pilote du contr�leur d'entr�es/sorties g�n�ralistes (GPIO) du microcontr�leur OKI ML675001
 * @{
 */

/**
 * @file gpio.h
 *
 * @brief Interface du pilote du contr�leur d'entr�es/sorties g�n�ralistes (GPIO) du microcontr�leur OKI ML675001
 *
 * $Author: GuillaumeSavaton $
 * $Date: 2007/09/19 07:16:40 $
 *
 * @version 2.0
 * $Revision: 1.4 $
 *
 * @todo Gestion des interruptions
 */

#ifndef H_GPIO
#define H_GPIO

#include "gpio_parametres_possibles.h"

#include <miro_types.h>

/**
 * @name Fonction d'initialisation
 * @{
 */

/**
 * @brief Initialisation des entr�es/sorties g�n�ralistes
 *
 * @post une instance de la Classe Gpio est cr�� ainsi que tous ses registres  
 */

void GPIO_initialiser ();

/** @} */

/**
 * @name Fonctions de configuration
 * @{
 */

/**
 * @brief Activation des fonctions sp�ciales des broches du GPIO
 *
 * Certaines broches du GPIO sont multiplex�es avec d'autres p�riph�riques (UART, contr�leur I2C, etc).
 * Cela signifie que ces broches sont utilisables soit commes des entr�es/sorties g�n�ralistes
 * (fonction "primaire" g�r�e par le GPIO), soit commes des broches d'un autre p�riph�rique (fonction
 * "secondaire").
 *
 * Cette fonction permet de s�lectionner les p�riph�riques pour lesquels les broches GPIO doivent �tre
 * configur�es pour leur fonction secondaire.
 *
 * @param fonctions Un masque d�signant les fonctions sp�ciales � activer (voir fichier gpio_parametres_possibles.h, section "Fonctions sp�ciales des broches d'entr�e/sortie)
 */
void GPIO_activer_fonctions (int fonctions);


/**
 * @brief D�sactivation des fonctions sp�ciales des broches du GPIO
 *
 * Certaines broches du GPIO sont multiplex�es avec d'autres p�riph�riques (UART, contr�leur I2C, etc).
 * Cela signifie que ces broches sont utilisables soit commes des entr�es/sorties g�n�ralistes
 * (fonction "primaire" g�r�e par le GPIO), soit commes des broches d'un autre p�riph�rique (fonction
 * "secondaire").
 *
 * Cette fonction permet de s�lectionner les p�riph�riques pour lesquels les broches GPIO doivent �tre
 * configur�es pour leur fonction primaire.
 *
 * @param fonctions Un masque d�signant les fonctions sp�ciales � d�sactiver (voir fichier gpio_parametres_possibles.h, section "Fonctions sp�ciales des broches d'entr�e/sortie)
 */
void GPIO_desactiver_fonctions (int fonctions);

/**
 * @brief Configuration de certaines broches d'un port en tant qu'entr�es
 *
 * Pour chaque port g�r� par le GPIO (plus pr�cis�ment pour chaque port configur� pour sa fonction primaire),
 * il est possible de r�gler broche par broche le sens de circulation des informations (entr�e ou sortie du microcontr�leur).
 * Pour le port s�lectionn�, cette fonction accepte en param�tre un masque qui d�finit les broches devant �tre configur�es
 * en entr�es : chaque bit du masque correspond � une broche, la broche num�ro 0 correspondant au bit de poids faible du masque.
 *
 * @param port Identifiant du port � configurer
 * @param masque Masque des broches � configurer (bits � 1 pour les broches � mettre en entr�e)
 */
void GPIO_set_entrees (int port, UInt16 masque);

/**
 * @brief Configuration de certaines broches d'un port en tant que sorties
 *
 * Pour chaque port g�r� par le GPIO (plus pr�cis�ment pour chaque port configur� pour sa fonction primaire),
 * il est possible de r�gler broche par broche le sens de circulation des informations (entr�e ou sortie du microcontr�leur).
 * Pour le port s�lectionn�, cette fonction accepte en param�tre un masque qui d�finit les broches devant �tre configur�es
 * en sorties : chaque bit du masque correspond � une broche, la broche num�ro 0 correspondant au bit de poids faible du masque.
 *
 * @param port Identifiant du port � configurer
 * @param masque Masque des broches � configurer (bits � 1 pour les broches � mettre en sortie)
 */
void GPIO_set_sorties (int port, UInt16 masque);

/** @} */

/**
 * @name Fonctions de lecture/�criture sur les ports
 * @{
 */

/**
 * @brief Lire la valeur d'un port
 *
 * @param port Identifiant du port � lire
 * @return La valeur courante des broches du port
 */
UInt16 GPIO_lire (int port);

/**
 * @brief Ecrire une valeur sur un port
 *
 * @param port Identifiant du port sur lequel �crire
 * @param valeur La valeur � �crire
 */
void GPIO_ecrire (int port, UInt16 valeur);

/** @} */

#endif

/** @} */

