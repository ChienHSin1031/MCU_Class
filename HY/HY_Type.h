#ifndef _HLX_TYPE_H_
#define _HLX_TYPE_H_



typedef int             BOOL;

typedef void            (*func)();

typedef int		HLX_HANDLE;

typedef unsigned char	BYTE;

#ifndef NULL
#define NULL            ((void*)0)
#endif

#define RLT_TRUE       (1)
#define RLT_FALSE      (0)

#define RLT_SUCCESS    (0)
#define RLT_FAILURE    (1)

#define INVALID_HANDLE   (-1)

#endif
