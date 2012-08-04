/**
 * @addtogroup pilotes_arm_uart
 * @{
 */

/**
 * @file uart_configuration_par_defaut.h
 *
 * @brief R�glages par d�faut des param�tres de fonctionnement de l'UART
 *
 * $Author: GuillaumeSavaton $
 * $Date: 2007/09/19 07:16:40 $
 *
 * @version 2.0
 * $Revision: 1.4 $
 */

#ifndef H_UART_CONFIGURATION_PAR_DEFAUT
#define H_UART_CONFIGURATION_PAR_DEFAUT

#include "uart_parametres_possibles.h"

/**
 * @name Valeurs des r�glages par d�faut
 * @{
 */

/** @brief Taille par d�faut des caract�res �chang�s sur la liaison s�rie */
#define UART_LONGUEUR_CARACTERE_PAR_DEFAUT UART_LONGUEUR_CARACTERE_8_BITS

/** @brief Nombre de bits d'arr�t par d�faut */
#define UART_BITS_DE_STOP_PAR_DEFAUT UART_LONGUEUR_ARRET_1_BIT

/** @brief Type de contr�le de parit� par d�faut */
#define UART_PARITE_PAR_DEFAUT UART_PARITE_AUCUNE

/** @brief Vitesse par d�faut, en bits/s */
#define UART_VITESSE_PAR_DEFAUT 9600

/** @brief Seuil par d�faut de d�tection du remplissage de la FIFO */
#define UART_SEUIL_FIFO_PAR_DEFAUT 0

/** @brief Vitesse de communication minimale (en bits/s) */
#define UART_VITESSE_MIN 75

/** @brief Vitesse de communication maximale (en bits/s) */
#define UART_VITESSE_MAX 115200

/** @} */

#endif

/** @} */

