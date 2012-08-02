/**
 * @addtogroup pilotes_arm_gpio
 * @{
 */

/**
 * @file gpio_registres.h
 *
 * @brief D�finition des registres du contr�leur d'entr�es/sorties g�n�ralistes (GPIO) du microcontr�leur OKI ML675001
 *
 * $Author: GuillaumeSavaton $
 * $Date: 2007/09/19 07:16:40 $
 *
 * @version 2.0
 * $Revision: 1.6 $
 */

#ifndef H_GPIO_REGISTRES
#define H_GPIO_REGISTRES

#include <miro_types.h>

/**
 * @name Adresses de base des registres du module GPIO
 * @{
 */

/** @brief Adresse de base des registres des ports du module GPIO */
#define GPIO_ADRESSE_BASE 0xB7A01000

/** @brief Adresse du registre de configuration global du module GPIO */
#define GPIO_ADRESSE_GPCTL 0xB7000000

/** @} */

/**
 * @name Masque des registres d'entr�e en fonction du port
 * @{
 */

/** @brief Masque de s�lection des bits utiles en lecture des ports A � D */
#define GPIO_MASQUE_A_D 0x00FF

/** @brief Masque de s�lection des bits utiles en lecture du port E */
#define GPIO_MASQUE_E 0x03FF

/** @} */

/**
 * @brief Structure de registre g�n�rique pour le GPIO
 *
 * Les registres sont tous sur 16 bits, align�s sur des adresses multiples de 4.
 */
typedef struct
{
   /** @brief La valeur du registre */
   UInt16 valeur;

   unsigned : 16;
} GPIO_Registre;

/**
 * @brief Structure d'un port du GPIO
 *
 * Les registres sp�cifiques � chaque port sont dispos�s dans un espace adressable contigu.
 * Tous les ports se pr�sentent de la m�me fa�on.
 */
typedef struct
{
   /** @brief Registre de sortie du port */
   GPIO_Registre gppo;

   /** @brief Registre d'entr�e du port */
   GPIO_Registre gppi;

   /** @brief Registre de configuration des entr�es/sorties */
   GPIO_Registre gppm;

   /** @brief Registre d'activation/d�sactivation des interruptions */
   GPIO_Registre gpie;

   /** @brief Registre de configuration de la polarit� des interruptions */
   GPIO_Registre gpip;

   /** @brief Registre d'�tat des demandes d'interruptions en attente */
   GPIO_Registre gpis;

   /** @brief Espace inutilis� */
   GPIO_Registre remplissage1;

   /** @brief Espace inutilis� */
   GPIO_Registre remplissage2;

} GPIO_Port;

#endif

/** @} */

