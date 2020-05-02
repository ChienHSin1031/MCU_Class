/*****************************************************************************
 *   HY_MsgQ.c:  message queue module
 *
 *
 *   ECR List:
 *
******************************************************************************/

#ifdef _cplusplus
extern "C" {
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HY_Type.h"
#include "HY_Event.h"
	
//----- definitions -----
#define MAX_MSG_NUM				64

//----- local varibles -----
static int PreMsgQ[MAX_MSG_NUM];
static volatile int flagPreStart;
static volatile int flagPreEnd;

static int MsgQ[MAX_MSG_NUM];
static volatile int flagStart;
static volatile int flagEnd;


//----- public interfaces -----

/*****************************************************************************
** Function:			int HY_initMsgQ(void)
**
** Descriptions:		message queue initialization
**
** parameters:			None
** Returned value:		0 	-- SUCCESS
**						1 	-- RLT_FAILURE
** 
*****************************************************************************/

int HY_initMsgQ(void)
{
	memset(PreMsgQ, 0, sizeof(PreMsgQ));
	flagPreStart = 0;
	flagPreEnd = 0;

	memset(MsgQ, 0, sizeof(MsgQ));
	flagStart = 0;
	flagEnd = 0;

	return RLT_SUCCESS;
}

int HLX_DeinitMsgQ(void)
{
	return RLT_SUCCESS;
}



/*****************************************************************************
** Function:			int HY_PostMsgQ(int nEvent)
**
** Descriptions:		This function is used to post a message into the message queue.
**
** parameters:			nEvent 		-- Event IDs defined in the system configuration
**
** Returned value:		0 	-- SUCCESS
**						1 	-- RLT_FAILURE
** 
*****************************************************************************/

int HY_PostMsgQ(int nEvent)  //
{
	MsgQ[flagEnd++] = nEvent;

	if(flagEnd >= MAX_MSG_NUM)
		flagEnd = 0;
		
	if(flagEnd == flagStart)
		flagStart++;

	return RLT_SUCCESS;	
}


/*****************************************************************************
** Function:			int HY_PostPremierMsgQ(int nEvent)
**
** Descriptions:		This function is used to post a message into the message queue from interrupt.
**
** parameters:			nEvent 		-- Event IDs defined in the system configuration
**
** Returned value:		0 	-- SUCCESS
**						1 	-- RLT_FAILURE
** 
*****************************************************************************/

int HY_PostPremierMsgQ(int nEvent)
{
	PreMsgQ[flagPreEnd++] = nEvent;

	if(flagPreEnd >= MAX_MSG_NUM)
		flagPreEnd = 0;
		
	if(flagPreEnd == flagPreStart)
		flagPreStart++;
		
	return RLT_SUCCESS;	
}


BOOL HY_CheckPremierMsgQ(int nEvent)
{
	if(flagPreEnd != flagPreStart)
	{
		if((flagPreEnd > 0 && PreMsgQ[flagPreEnd - 1] == nEvent) || (flagPreEnd == 0 && PreMsgQ[MAX_MSG_NUM - 1] == nEvent))
			return RLT_TRUE;
	}

	return RLT_FALSE;
}



/*****************************************************************************
** Function:			int HLX_GetMsgQ(void)
**
** Descriptions:		This function is used to get an event from the message queue. 
**						This event will be removed from the message queue.
**
** parameters:			None
** Returned value:		>=0 	-- Event ID
**						-1 		-- RLT_FAILURE
** 
*****************************************************************************/

int HY_GetMsgQ(void)
{
	int nEvent;

	// check premior queue first
	if(flagPreStart != flagPreEnd)
	{
		nEvent = PreMsgQ[flagPreStart++];
		if(flagPreStart >= MAX_MSG_NUM)
			flagPreStart = 0;
	
		return nEvent;
	}

	// check normal queue
	if(flagStart == flagEnd)
		return INVALID_EVENT;

	nEvent = MsgQ[flagStart++];
	if(flagStart >= MAX_MSG_NUM)
		flagStart = 0;

	return nEvent;
}

#ifdef _cplusplus // if compiled with C++ compiler
} // end of extern "C" block
#endif



