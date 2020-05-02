#include "Message.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MESSAGE		96//192
#define MAX_PRE_MESSAGE		16//32


typedef struct MESSAGE {
	int prm0;				// parameter 0
	int prm1;				// parameter 1
	void (*func)(int,int);	// message handler
} MESSAGE;

typedef struct PREMIER_MESSAGE {
	int prm0;						// parameter 0
	int prm1;						//buff len
	void (*func)(char *,int);	// message handler
} PREMIER_MESSAGE;


static MESSAGE msgQueue[MAX_MESSAGE];	       // Main message queue (looped)
static volatile int flagStart;
static volatile int flagEnd;
static PREMIER_MESSAGE msgPreQueue[MAX_PRE_MESSAGE]; // Premier message queue (looped)
static volatile int flagPreStart;
static volatile int flagPreEnd;

int Message_Init(void)
{
	memset(msgPreQueue, 0, sizeof(msgPreQueue));
	flagPreStart = 0;
	flagPreEnd = 0;

	memset(msgQueue, 0, sizeof(msgQueue));
	flagStart = 0;
	flagEnd = 0;

	return 1;
}




void PostMessage(void (*func)(int,int), int prm0, int prm1)
{

	MESSAGE* pMsg;

	pMsg = &msgQueue[flagEnd++];

	pMsg->prm0 = prm0;
	pMsg->prm1 = prm1;
	pMsg->func = func;

	if(flagEnd >= MAX_MESSAGE)
		flagEnd = 0;

	if(flagEnd == flagStart)
		flagStart++;

}

void PostPremierMessage(void (*func)(int, int), int prm0, int prm1) //prm0 --> buff , prm1 --> len
{
	PREMIER_MESSAGE* pMsg;

	if (!prm0 || prm1<=0) return;

	pMsg = &msgPreQueue[flagPreEnd++];


	pMsg->prm0 = prm0;	
	pMsg->prm1 = prm1;
	pMsg->func = func;

	if(flagPreEnd >= MAX_PRE_MESSAGE)
		flagPreEnd = 0;

	if(flagPreEnd == flagPreStart)
		flagPreStart++;

}


void DeleteMessage(void (*func)(int,int))
{

}

void DeletePremierMessage(void (*func)(int,int))
{

}

// Get one message from the message queue and handle the function
// return: 0 - no message to be processed, 1 - processed one message
int ProcessMessage()
{
	MESSAGE* pMsg = 0;
	PREMIER_MESSAGE* pPMsg = 0;
	char execute=0;

	// check premier queue first
	if(flagPreStart != flagPreEnd)
	{
		pPMsg = &msgPreQueue[flagPreStart++];

		if (pPMsg->func != (void *)0)
		{
			pPMsg->func(pPMsg->prm0, pPMsg->prm1);		// call the function
			pPMsg->prm0 = 0;
			pPMsg->prm1 = 0;
			pPMsg->func = 0;
		}

		if(flagPreStart >= MAX_PRE_MESSAGE)
			flagPreStart = 0;

		//return 1;
		execute|=1;
	}

	// check normal queue
	if(flagStart == flagEnd)
		return execute;

	pMsg = &msgQueue[flagStart++];

	if (pMsg->func != (void *)0)
	{
		pMsg->func(pMsg->prm0, pMsg->prm1);		// call the function
		pMsg->prm0 = 0;
		pMsg->prm1 = 0;
		pMsg->func = 0;
	}

	if(flagStart >= MAX_MESSAGE)
		flagStart = 0;

	return execute|=1;
}

