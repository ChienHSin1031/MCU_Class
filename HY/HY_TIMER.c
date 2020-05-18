#include "app_timer.h"
#include "nrf_log.h"
#include "HY_Framework.h"
//#include "HY_MsgQ.h"
#include "app_error.h"
#include "Message.h"
#include "HY_BLE.h"

#define MS_INTERVAL        (20)

#define MAX_TIMER_FUNC      16

#define HY_INTERVAL              APP_TIMER_TICKS(20) //one second interval            
#define test_INTERVAL            APP_TIMER_TICKS(1000) //one second interval     

typedef short int int16_t;   

typedef struct TIM_HANDLER {
	int16_t value;
	int16_t count;
	void (*func)(int, int);
} TIM_HANDLER;

typedef struct TIM_REAL_HANDLER {
        int16_t value;
        int16_t count;
        void (*func)();
} TIM_REAL_HANDLER;


static TIM_HANDLER Handler20ms[MAX_TIMER_FUNC];			// 20ms-based handlers
static TIM_HANDLER Handler1sec[MAX_TIMER_FUNC];			// 1sec-based handlers
static TIM_REAL_HANDLER RealHandler20ms[MAX_TIMER_FUNC];	// 20ms-based real-time handlers

static int16_t Num20ms;				// number of 20ms-based handlers
static int16_t Num1sec;				// number of 1sec-based handlers

static volatile bool bEnable;

static void  timeout_20ms_handler(void);
static void timeout_1000ms_handler(void);

APP_TIMER_DEF(timer_20ms_id);
APP_TIMER_DEF(timer_1000ms_id);

void TIM_Init()
{
    
    memset(Handler20ms, 0, sizeof(Handler20ms));
    memset(Handler1sec, 0, sizeof(Handler1sec));
    memset(RealHandler20ms, 0, sizeof(RealHandler20ms));
    
    Num20ms = 0;
    Num1sec = 0;
    
    ret_code_t err_code;
    err_code = app_timer_init();
    err_code = app_timer_create(&timer_20ms_id,
                                APP_TIMER_MODE_REPEATED,    
                                timeout_20ms_handler);
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_create(&timer_1000ms_id,
                                APP_TIMER_MODE_REPEATED,    
                                timeout_1000ms_handler);
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_start(timer_20ms_id, HY_INTERVAL, NULL);
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_start(timer_1000ms_id, test_INTERVAL, NULL);
    APP_ERROR_CHECK(err_code);

		
}

  
static void timeout_1000ms_handler(void)
{
  
  
    TIM_HANDLER *pHandler;
    
    pHandler = Handler1sec;
      
    // scan the handler table and call handler functions
    for (int i = 0; i < MAX_TIMER_FUNC; i++, pHandler++)
    {
            if (pHandler->func != 0)
            {
                    pHandler->count++;

                    if (pHandler->count >= pHandler->value)
                    {
                            PostMessage(pHandler->func, 0, 0);

                            pHandler->count = 0;
                    }
            }
    }	

}

static void timeout_20ms_handler(void)
{
    TIM_HANDLER *pHandler; //type  pointer 
    TIM_REAL_HANDLER *pRealHandler;

    //HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_2);


    pHandler = Handler20ms;

    // scan the handler table and call handler functions
    for (int i = 0; i < MAX_TIMER_FUNC; i++, pHandler++)
    {
            if (pHandler->func != 0)
            {
                    pHandler->count++;

                    if (pHandler->count >= pHandler->value)
                    {
                            PostMessage(pHandler->func, 0, 0);

                            pHandler->count = 0;
                    }
            }
    }	

    pRealHandler = RealHandler20ms;

    // scan the real-time handler table and call handler functions
    for (int i = 0; i < MAX_TIMER_FUNC; i++, pRealHandler++)
    {
            if (pRealHandler->func != 0)
            {
                    pRealHandler->count++;

                    if (pRealHandler->count >= pRealHandler->value)
                    {
                            pRealHandler->func();

                            pRealHandler->count = 0;
                    }
            }
    }
}




int TIM_RegisterHandler(void (*func)(int, int), int msec)
{
	if (NULL==func) return 0;
	
	if (msec >= 1000)
	{
		// old handler exists?
		for (int i = 0; i < MAX_TIMER_FUNC; i++)
		{
			if (Handler1sec[i].func == func)
			{
				// find out old handler
				Handler1sec[i].count = 0;
				Handler1sec[i].value = (msec + 500) / 1000;

				Num1sec++;
				
				return 1;
			}
		}

		for (int i = 0; i < MAX_TIMER_FUNC; i++)
		{
			if (Handler1sec[i].func == 0)
			{
				// find out a blank, then add the handler
				Handler1sec[i].func = func;
				Handler1sec[i].count = 0;
				Handler1sec[i].value = (msec + 500) / 1000;

				Num1sec++;

				return 1;
			}
		}
	}
	else
	{
		// old handler exists?
		for (int i = 0; i < MAX_TIMER_FUNC; i++)
		{
			if (Handler20ms[i].func == func)
			{
				// find out old handler
				Handler20ms[i].count = 0;
				Handler20ms[i].value = (msec + MS_INTERVAL / 2) / MS_INTERVAL;

				Num20ms++;

				return 1;
			}
		}

		for (int i = 0; i < MAX_TIMER_FUNC; i++)
		{
			if (Handler20ms[i].func == 0)
			{
				// find out a blank, then add the handler
				Handler20ms[i].func = func;
				Handler20ms[i].count = 0;
				Handler20ms[i].value = (msec + MS_INTERVAL / 2) / MS_INTERVAL;

				Num20ms++;

				return 1;
			}
		}
	}

	// the table is full
	return 0;
}


static int TIM_UnregisterHandler(void (*func)(int, int))
{
	if (NULL==func) return 0;
	
	for (int i = 0; i < MAX_TIMER_FUNC; i++)
	{
		if (Handler1sec[i].func == func)
		{
			Handler1sec[i].func = 0;

			Num1sec--;

			return 1;
		}
	}

	for (int i = 0; i < MAX_TIMER_FUNC; i++)
	{
		if (Handler20ms[i].func == func)
		{
			Handler20ms[i].func = 0;

			Num20ms--;

			return 1;
		}
	}

	return 0;
}

static int TIM_RegisterRealHandler(void (*func)(), int msec)
{
	if (NULL==func) return 0;

	// old handler exists?
	for (int i = 0; i < MAX_TIMER_FUNC; i++)
	{
		if (RealHandler20ms[i].func == func)
		{
			// find out old handler
			RealHandler20ms[i].count = 0;
			RealHandler20ms[i].value = (msec + MS_INTERVAL / 2) / MS_INTERVAL;

			Num20ms++;

			return 1;
		}
	}

	for (int i = 0; i < MAX_TIMER_FUNC; i++)
	{
		if (RealHandler20ms[i].func == 0)
		{
			// find out a blank, then add the handler
			RealHandler20ms[i].func = func;
			RealHandler20ms[i].count = 0;
			RealHandler20ms[i].value = (msec + MS_INTERVAL / 2) / MS_INTERVAL;

			Num20ms++;

			return 1;
		}
	}

	// the table is full
	return 0;
}

static int TIM_UnregisterRealHandler(void (*func)())
{
	if (NULL==func) return 0;
	
	for (int i = 0; i < MAX_TIMER_FUNC; i++)
	{
		if (RealHandler20ms[i].func == func)
		{
			RealHandler20ms[i].func = 0;

			Num20ms--;

			return 1;
		}
	}

	return 0;
}

static int TIM_ResetHandler(void (*func)(int, int))
{
	if (NULL==func) return 0;
	
	for (int i = 0; i < MAX_TIMER_FUNC; i++)
	{
		if (Handler1sec[i].func == func)
		{
			Handler1sec[i].count = 0;

			return 1;
		}
	}

	for (int i = 0; i < MAX_TIMER_FUNC; i++)
	{
		if (Handler20ms[i].func == func)
		{
			Handler20ms[i].count = 0;

			return 1;
		}
	}

	return 0;
}


static int TIM_ResetRealHandler(void (*func)())
{
	if (NULL==func) return 0;
	
	for (int i = 0; i < MAX_TIMER_FUNC; i++)
	{
		if (RealHandler20ms[i].func == func)
		{
			RealHandler20ms[i].count = 0;

			return 1;
		}
	}
	return 0;
}
