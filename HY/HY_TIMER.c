
#include "app_timer.h"
#include "nrf_log.h"
#include "HY_Framework.h"
#include "HY_MsgQ.h"

APP_TIMER_DEF(m_HY_timer_id);
APP_TIMER_DEF(test_timer_id);


void HY_TIMER_init(void);
static void HY_timers_start(void);
static void hy_timeout_handler();   //void * p_context
static void test_timeout_handler();


#define HY_INTERVAL              APP_TIMER_TICKS(1000) //one second interval            
#define test_INTERVAL            APP_TIMER_TICKS(1000) //one second interval            
          


void HY_TIMER_init(void){
  //Init Timer module.
  ret_code_t err_code; 
  err_code = app_timer_init();
      
  APP_ERROR_CHECK(err_code);

  // create Timer.
  err_code = app_timer_create(&m_HY_timer_id,
                              APP_TIMER_MODE_REPEATED,  //REPEATED
                              hy_timeout_handler);
  APP_ERROR_CHECK(err_code);
   
  err_code = app_timer_create(&test_timer_id,
                              APP_TIMER_MODE_REPEATED,    //SINGLE_SHOT
                              test_timeout_handler);
  APP_ERROR_CHECK(err_code);



  HY_timers_start();
}

static void HY_timers_start(void){
    ret_code_t err_code;

    // Start application timers.
    err_code = app_timer_start(m_HY_timer_id, HY_INTERVAL, NULL);
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_start(test_timer_id, test_INTERVAL, NULL);
    APP_ERROR_CHECK(err_code);


}

static void hy_timeout_handler(){   //void * p_context
      //put the event to Q
      HY_PostMsgQ(EVENT_BLE_HEART_RATE_UPDATE);

 }

static void test_timeout_handler(){ 
     HY_PostMsgQ(Event_HY_test_Event);

}


