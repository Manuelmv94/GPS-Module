/*! **********************************************************************************************
* 2016  ITESM Campus Guadalajara. Laboratorio de Microcontroladores 
*  
*
* @file:      GPSManager.c
* @author(s): Manuel Madrigal Valenzuela; Efraín Duarte López
*
* @brief (Theory of Operation)
*     This driver uses the SCI driver to receive and transmit data through the serial port.
*     The receiving part is done by the MessageManager_buildMessage function, which is called
*     from an interruption of the SCI, buffering the message in a bi-dimensional array.
*     When the message is ready, it is hold by the cMessage char array.
*     The transmitting part is done by just passing the parameters to the SCI driver.
*     
**************************************************************************************************/


/*************************************************************************************************/
/*********************						Includes						**********************/
/*************************************************************************************************/
#include "GPSManager.h"

/*************************************************************************************************/
/*********************						Defines							**********************/
/*************************************************************************************************/
#define INIT_VALUE   0

/*************************************************************************************************/
/*********************						Typedefs						**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Function Prototypes					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************                  Static Variables                    **********************/
/*************************************************************************************************/
static const u8 u8HashKeyToMatch=(u8)'M'+(u8)'C'; 
static const char cMessagesArray[4][13]={"\n\rTime:",
									  "\n\rValid:",
									  "\n\rLatitude:",
									  "\n\rLongitude:"
									 };
static const u8 u8SizesArray[]={7,8,11,12};

/*************************************************************************************************/
/*********************					Global Variables					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************                  Static Constants                    **********************/
/*************************************************************************************************/
static bool bValidString;
static u8 	u8StringPosition;
static u8 u8HashKey;
static u8 u8CommaCounter;

static char cTime[6];
static char cValid;
static char cLatitude[14];
static char cLongitude[14];
static u8 	u8BuffersIndex;

static u8 	u8TaskIndex;
static bool bMessageReady;
/*************************************************************************************************/
/*********************                  Global Constants                    **********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************				   Exported Functions					**********************/
/*************************************************************************************************/

void GPSManager_Init(void)
{
   SCI_InitTx();
   SCI_InitRx();
   SCI_RxSetCallback(&GPSManager_validateMessage);//Sets the function to be called when an interruption of the SCI reception is rised
   bValidString=FALSE;
   u8StringPosition=0;
   u8HashKey=0;
   u8CommaCounter=0;
   u8TaskIndex=0;
   bMessageReady=FALSE;
}

void GPSManager_validateMessage(char cReceivedChar)
{
   
   if(bValidString==TRUE)
   {
	  if(u8HashKey==u8HashKeyToMatch)
	  {
		 if(cReceivedChar==',')
		  {
			  u8CommaCounter++;
			  if(u8CommaCounter==4)
			  {
				  if(u8BuffersIndex<14){
					  cLatitude[u8BuffersIndex++]=cReceivedChar;
				  }
			  }
			  else if(u8CommaCounter==6)
			  {
				  if(u8BuffersIndex<14){
					  cLongitude[u8BuffersIndex++]=cReceivedChar;
				  }
			  }
			  else{
				  u8BuffersIndex=0; 
			  }
			  
		  }
		  else
		  {
			  switch(u8CommaCounter)
			  {
			  	  case 1:
			  		  if(u8BuffersIndex<6){
			  			  cTime[u8BuffersIndex++]=cReceivedChar;
			  		  }
			  		  break;
			  	  case 2:
			  		  cValid=cReceivedChar;
			  		  break;
			  	  case 3:
			  	  case 4:
			  		  if(u8BuffersIndex<14){
			  			  cLatitude[u8BuffersIndex++]=cReceivedChar;
			  		  }
			  		  break;
			  	  case 5:
			  	  case 6:
			  		  if(u8BuffersIndex<14){
			  			  cLongitude[u8BuffersIndex++]=cReceivedChar;
			  		  }
			  		  break;
			  	  default:
			  		  break;
			  }
		  }
	  }
	  
	  if(u8StringPosition==4 || u8StringPosition==5)
	  {
		  u8HashKey+=(u8)cReceivedChar;
	  }
	  
	  u8StringPosition++;
	  
	  if(cReceivedChar == ASCII_ENTER_CODE || (u8CommaCounter==7))
	  {
		 bValidString=FALSE;
		 u8StringPosition=0;
		 u8HashKey=0;
		 u8CommaCounter=0;
		 bMessageReady=TRUE;
		 
	  } 
   }
   else
   {
	   if(cReceivedChar == '$')
	   {
		   bValidString=TRUE;
		   u8StringPosition++;
	   }
   }
}

void GPSManager_PeriodicTask()
{
	if(!SCI_TxIsBusy() && bMessageReady)
	{
		switch(u8TaskIndex)
		{
			case 0:
				SCI_SendMessage(cMessagesArray[0],u8SizesArray[0]);
				break;
			case 1:
				SCI_SendMessage(cTime,6);
				break;
			case 2:
				SCI_SendMessage(cMessagesArray[1],u8SizesArray[1]);
				break;
			case 3:
				if(cValid=='A')
				{
					SCI_SendMessage("Yes",3);
				}
				else{
					SCI_SendMessage("No\r\n",4);
					u8TaskIndex=7;
					bMessageReady=FALSE;
				}
				
				break;
			case 4:
				SCI_SendMessage(cMessagesArray[2],u8SizesArray[2]);
				break;
			case 5:
				SCI_SendMessage(cLatitude,14);
				break;
			case 6:
				SCI_SendMessage(cMessagesArray[3],u8SizesArray[3]);
				break;
			case 7:
				SCI_SendMessage(cLongitude,14);
				break;
			default:
				while(1){}
				
		}
		if(u8TaskIndex++==7)
		{
			u8TaskIndex=0;
			bMessageReady=FALSE;
		}
	}
}




//-------------------------------------------------------------------------------------------------


/*************************************************************************************************/
/*********************				    Private Functions					**********************/
/*************************************************************************************************/

