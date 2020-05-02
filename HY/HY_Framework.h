#ifndef HY_FRAMEWORK_H
#define HY_FRAMEWORK_H

#include "HY_Type.h"
#include "HY_Event.h"

int HY_initFramework(void);
int HY_DeinitFramework(void);
int HY_InstallEventHandler(int nEvent, func funcEventHandler);
int HY_InstallPreEventHandler(int nEvent);
int HY_CheckEventHandler(int nEvent);
int HY_UninstallEventHandler(int nEvent);
int HY_MaskEventHandler(int nEvent);
int HY_UnmaskEventHandler(int nEvent);
int HY_ProcessEventHandler(int nEvent);



#endif