/* 2016  ITESM Campus Guadalajara. Laboratorio de Microcontroladores
 *
 * @file:      MessageManager.h
 * @author(s):    Manuel Madrigal Valenzuela; Efraín Duarte López    
 *
 * @brief (User's Manual):
 *    This manager controls the transmission and reception of messages.
 *    In order to send a message the user should check first if the manager is not already busy sending 
 *    a previous message. This is done by the MessageManager_TxIsBusy() function; once the user is sure
 *    that the manager isn't busy he can send the new message with the MessageManager_SendMessage(char[],u8);
 *    To receive a message the process is very similar, the user has to check first if there is a new message
 *    ready to be read.This is done by the MessageManager_IsMessageReady(void) function. Then the user can read
 *    the message with the MessageManager_getMessage(char msg[], u8* size) function.
 *    
 *    This manager has been tested on the following NXP MCUs:
 *     + MC9S08QG8
 **************************************************************************************************/

#ifndef GPSMANAGER_H_
#define GPSMANAGER_H_



/*************************************************************************************************/
/*********************                 Includes                **********************/
/*************************************************************************************************/
#include "derivative.h" /* include peripheral declarations */
#include "types.h"
#include "SCI.h"

/*************************************************************************************************/
/*********************                 Defines                    **********************/
/*************************************************************************************************/
#define ASCII_ENTER_CODE    13
#define MESSAGE_MANAGER_MAX_MSG_SIZE   SCI_MAX_MSG_SIZE

/*************************************************************************************************/
/*********************                 Typedefs                **********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************            Functions Like Macros              **********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************              Extern Variables              **********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************                  Extern Constants                    **********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************              Function Prototypes              **********************/
/*************************************************************************************************/

/*!*-----------------------------------------------------------------------------------------------
* \name     MessageManager_Init
* \brief    Initializes the Transmission and Reception from the SCI drivers. It also sets the call back fn
*           and the initial conditions needed to start receiving data.
* \param    void
* \return   void
-------------------------------------------------------------------------------------------------*/
void GPSManager_Init(void);

/*!*-----------------------------------------------------------------------------------------------
* \name     MessageManager_buildMessage
* \brief    It buffers the message character by character, waiting for an ASCCI_ENTER_CODE, when received,
*           it prepares the message to be read and sets a flag meaning that the message is ready
* \param    char
* \return   void
-------------------------------------------------------------------------------------------------*/
void GPSManager_validateMessage(char);

/*!*-----------------------------------------------------------------------------------------------
* \name     MessageManager_buildMessage
* \brief    It buffers the message character by character, waiting for an ASCCI_ENTER_CODE, when received,
*           it prepares the message to be read and sets a flag meaning that the message is ready
* \param    char
* \return   void
-------------------------------------------------------------------------------------------------*/
void GPSManager_PeriodicTask();



/*************************************************************************************************/



#endif /* GPSMANAGER_H_ */
