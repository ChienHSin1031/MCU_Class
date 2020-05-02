/*****************************************************************************
 *   HY_Framework.c:  Framework
 *
 *   Copyright(C) 2009, HY Technology, Inc.
 *   All rights reserved.
 *
 *   ECR List:
 *
******************************************************************************/
#ifdef _cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include "HY_Framework.h"


	
//----- Global varibles -----


//----- Definitions -----

#define MAX_EVENT_HANDLE			32

typedef struct tagEventHandle {
	int 	nEventID : 31;
	int	flag_mask : 1;
	func	funcEventHandler;
	func	pre_funcEventHandler;
} EventHandle;

//----- Private varibles -----
static EventHandle		s_arEventHandle[MAX_EVENT_HANDLE];

/*****************************************************************************
** Function:			int HY_initFramework(void)
**
** Descriptions:		This function is used to initialize framework.
**
** parameters:			None
** Returned value:		0 	-- RLT_SUCCESS
**						1 	-- RLT_FAILURE
** 
*****************************************************************************/

int HY_initFramework(void)
{

	// initialize event handler table
	memset(s_arEventHandle, 0, MAX_EVENT_HANDLE*sizeof(EventHandle));


	return RLT_SUCCESS;
}

int HY_DeinitFramework(void)
{


	return RLT_SUCCESS;
}


/*****************************************************************************
** Function:			int HY_InstallEventHandler(int nEvent, func *funcEventHandler)
**
** Descriptions:		This function is used to install event handler.
**
** parameters:			nEvent				-- Event ID
**						funcEventHandler	-- function pointer of event handler
**
** Returned value:		0 -- RLT_SUCCESS
**						1 -- RLT_FAILURE
** 
*****************************************************************************/

int HY_InstallEventHandler(int nEvent, func funcEventHandler)
{
	int i, null_index = -1;
	char cSet = 0;

	// find available handler
	for(i=0; i<MAX_EVENT_HANDLE; i++)
	{
		if(s_arEventHandle[i].nEventID == EVENT_NULL)
		{
			null_index = i;
		}
		else if(s_arEventHandle[i].nEventID == nEvent)
		{
			s_arEventHandle[i].pre_funcEventHandler = s_arEventHandle[i].funcEventHandler;	
			s_arEventHandle[i].funcEventHandler = funcEventHandler;
			cSet = 1;
		}
	}

	if(cSet == 0 && null_index > -1)
	{
		s_arEventHandle[null_index].pre_funcEventHandler = NULL;	
		s_arEventHandle[null_index].nEventID = nEvent;
		s_arEventHandle[null_index].funcEventHandler = funcEventHandler;
	}

	if(cSet == 1 || null_index != -1)
		return RLT_SUCCESS;
	else
		return RLT_FAILURE;
}


/*****************************************************************************
** Function:			int HY_InstallPreEventHandler(int nEvent)
**
** Descriptions:                  .
**
** parameters:			nEvent				-- Event ID
**
** Returned value:		0 -- RLT_SUCCESS
**						1 -- RLT_FAILURE
** 
*****************************************************************************/

int HY_InstallPreEventHandler(int nEvent)
{
	int i;

	for(i=0; i<MAX_EVENT_HANDLE; i++)
	{
		if(s_arEventHandle[i].nEventID == nEvent)
		{
			if(s_arEventHandle[i].pre_funcEventHandler != NULL)
			{
				s_arEventHandle[i].funcEventHandler = s_arEventHandle[i].pre_funcEventHandler;
			}
			else
			{
				s_arEventHandle[i].nEventID = EVENT_NULL;
				s_arEventHandle[i].funcEventHandler = NULL;
			}

			s_arEventHandle[i].pre_funcEventHandler = NULL;

			return RLT_SUCCESS;
		}
	}

	return RLT_FAILURE;
}


/*****************************************************************************
** Function:			int HY_CheckEventHandler(int nEvent)
**
** Descriptions:		This function is used to check event handler.
**
** parameters:			nEventID	-- event ID
**
** Returned value:		0 	-- RLT_SUCCESS
**						1 	-- RLT_FAILURE
** 
*****************************************************************************/

int HY_CheckEventHandler(int nEvent)
{
	int i;

	for(i=0; i<MAX_EVENT_HANDLE; i++)
	{
		if(s_arEventHandle[i].nEventID == nEvent)
		{
			func pFunc = s_arEventHandle[i].funcEventHandler;

			if(pFunc != NULL)
                            return RLT_SUCCESS;
			else
                            return RLT_FAILURE;
			
		}
	}

	return RLT_FAILURE;
}

/*****************************************************************************
** Function:			int HY_UninstallEventHandler(int nEvent)
**
** Descriptions:		This function is used to uninstall event handler.
**
** parameters:			nEventID	-- event ID
**
** Returned value:		0 	-- RLT_SUCCESS
**						1 	-- RLT_FAILURE
** 
*****************************************************************************/

int HY_UninstallEventHandler(int nEvent)
{
	int i;

	for(i=0; i<MAX_EVENT_HANDLE; i++)
	{
		if(s_arEventHandle[i].nEventID == nEvent)
		{
			s_arEventHandle[i].nEventID = EVENT_NULL;
			s_arEventHandle[i].funcEventHandler = NULL;
			s_arEventHandle[i].pre_funcEventHandler = NULL;
		}
	}

	return RLT_SUCCESS;
}


/*****************************************************************************
** Function:			int HY_MaskEventHandler(int nEvent)
**
** Descriptions:		This function is used to mask event handler.
**
** parameters:			nEventID	-- event ID
**
** Returned value:		0 	-- RLT_SUCCESS
**						1 	-- RLT_FAILURE
** 
*****************************************************************************/

int HY_MaskEventHandler(int nEvent)
{
	int i;

	for(i=0; i<MAX_EVENT_HANDLE; i++)
	{
		if(s_arEventHandle[i].nEventID == nEvent)
		{
			s_arEventHandle[i].flag_mask = 1;
			break;
		}
	}

	return RLT_SUCCESS;
}


/*****************************************************************************
** Function:			int HY_UnmaskEventHandler(int nEvent)
**
** Descriptions:		This function is used to unmask event handler.
**
** parameters:			nEventID	-- event ID
**
** Returned value:		0 	-- RLT_SUCCESS
**						1 	-- RLT_FAILURE
** 
*****************************************************************************/

int HY_UnmaskEventHandler(int nEvent)
{
	int i;

	for(i=0; i<MAX_EVENT_HANDLE; i++)
	{
		if(s_arEventHandle[i].nEventID == nEvent)
		{
			s_arEventHandle[i].flag_mask = 0;
			break;
		}
	}

	return RLT_SUCCESS;
}


/*****************************************************************************
** Function:			int HY_ProcessEventHandler(int nEvent)
**
** Descriptions:		This function is used to process event handler.
**
** parameters:			nEventID	-- event ID
**
** Returned value:		0 	-- RLT_SUCCESS
**						1 	-- RLT_FAILURE
** 
*****************************************************************************/

int HY_ProcessEventHandler(int nEvent)
{
	int i;

	for(i=0; i<MAX_EVENT_HANDLE; i++)
	{
		if(s_arEventHandle[i].nEventID == nEvent && s_arEventHandle[i].flag_mask == 0)
		{
			func pFunc = s_arEventHandle[i].funcEventHandler;

			if(pFunc != NULL)
				pFunc();
			
			break;
		}
	}

	if(i < MAX_EVENT_HANDLE)
		return RLT_SUCCESS;
	else
		return RLT_FAILURE;
}



#ifdef _cplusplus // if compiled with C++ compiler
} // end of extern "C" block
#endif




