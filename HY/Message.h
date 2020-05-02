
#ifndef __MESSAGE_H__
#define __MESSAGE_H__

int Message_Init(void);
void PostMessage(void (*func)(int,int), int, int);
int ProcessMessage(void);
void DeleteMessage(void (*func)(int, int));

void PostPremierMessage(void (*func)(int,int), int prm0, int prm1);
void DeletePremierMessage(void (*func)(int,int));

#endif	/* __MESSAGE_H__ */

