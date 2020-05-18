#include "app_button.h"
#include "app_timer.h"
#include "bsp_btn_ble.h"

#include "app_error.h"
#include "Message.h"
#include "nrf_log.h"


#define PIN_0 0


#define BUTTON_STATE_POLL_INTERVAL_MS  500

#define LONG_PRESS(MS)    (uint32_t)(MS)/BUTTON_STATE_POLL_INTERVAL_MS 

APP_TIMER_DEF(m_button_action);


static int page;

void HY_ShortPress_Event(){
  
  NRF_LOG_INFO("Short Event");
  
  switch(page){
    case 1:
      Page1();
      page++;
      break;
    case 2:
      Page2();
      page++;
      break;

    case 3:
      Page3();
      page = 1;
      break;

  }

}

void Page1(){
    NRF_LOG_INFO("Page1");
}

void Page2(){
    NRF_LOG_INFO("Page2");
}

void Page3(){
    NRF_LOG_INFO("Page3");
}


void HY_LongPress_Event(){
  NRF_LOG_INFO("LongPress Event");

}



static void button_timeout_handler(void * p_context)
{ 
     uint32_t err_code;
     static uint32_t cnt;
     void *short_event_ptr;
     short_event_ptr = HY_ShortPress_Event;
     void *long_event_ptr;
     long_event_ptr = HY_LongPress_Event;

     if (app_button_is_pushed(0))
     {
         cnt++;
         NRF_LOG_INFO("%d", cnt);
         if ( cnt >= LONG_PRESS(2000))
         {
             cnt = 0;
             //NRF_LOG_INFO("Long Button press2");
             PostMessage(long_event_ptr,0 ,0);

         }
         else
         {
              err_code = app_timer_start(m_button_action, 
                                         APP_TIMER_TICKS(BUTTON_STATE_POLL_INTERVAL_MS),
                                         NULL);
              APP_ERROR_CHECK(err_code);
         }
     }
     else
     {
          cnt = 0; // reset counter variable
          //NRF_LOG_INFO("Short button press2");
          PostMessage(short_event_ptr, 0, 0);
     }
}

static void button_callback(uint8_t pin_no, uint8_t button_action)
{
      uint32_t err_code;

      
      if ((pin_no == BUTTON_1) && (button_action == APP_BUTTON_PUSH))
      {
          err_code = app_timer_start(m_button_action, 
                                     APP_TIMER_TICKS(BUTTON_STATE_POLL_INTERVAL_MS),
                                     NULL);
          APP_ERROR_CHECK(err_code);
      }
}


static void buttons_init()
{
      uint32_t err_code;
       page = 1;
      
      static app_button_cfg_t  button_cfg;

      button_cfg.pin_no         = BUTTON_1;
      button_cfg.button_handler = button_callback;
      button_cfg.pull_cfg       = NRF_GPIO_PIN_PULLUP;
      button_cfg.active_state   = APP_BUTTON_ACTIVE_HIGH;
      
      err_code = app_button_init(&button_cfg, 1, 100);
      APP_ERROR_CHECK(err_code);
      
      err_code = app_button_enable();
      APP_ERROR_CHECK(err_code);

      /*Enable an app timer instance to detect long button press*/
      err_code = app_timer_create(&m_button_action, APP_TIMER_MODE_SINGLE_SHOT, button_timeout_handler);
      APP_ERROR_CHECK(err_code);
}


void HY_buttons_init(){
  buttons_init();
}