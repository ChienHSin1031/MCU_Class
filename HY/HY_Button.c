

#include <stdbool.h>

#include "app_timer.h"
#include "HY_GPIOTE.h"
#include "bsp.h"
#include "app_button.h"



void bsp_init(){

    m_registered_callback = callback; 
    err_code = bsp_event_to_button_action_assign(num, BSP_BUTTON_ACTION_PUSH, BSP_EVENT_DEFAULT); 

    err_code = app_button_init((app_button_cfg_t *)app_buttons,             //事件數組
                                                   BUTTONS_NUMBER,         //按鍵個數
                                                   APP_TIMER_TICKS(50));  //50ms的TICK時間數

    
    static const app_button_cfg_t app_buttons[BUTTONS_NUMBER] =
    { 
        #ifdef BSP_BUTTON_0
        {BSP_BUTTON_0, false, BUTTON_PULL, bsp_button_event_handler},
        #endif // BUTTON_0

        #ifdef BSP_BUTTON_1
        {BSP_BUTTON_1, false, BUTTON_PULL, bsp_button_event_handler},
        #endif // BUTTON_1

        #ifdef BSP_BUTTON_2
        {BSP_BUTTON_2, false, BUTTON_PULL, bsp_button_event_handler},
        #endif // BUTTON_2

        #ifdef BSP_BUTTON_3
        {BSP_BUTTON_3, false, BUTTON_PULL, bsp_button_event_handler},
        #endif // BUTTON_3

        #ifdef BSP_BUTTON_4
        {BSP_BUTTON_4, false, BUTTON_PULL, bsp_button_event_handler},
        #endif // BUTTON_4

        #ifdef BSP_BUTTON_5
        {BSP_BUTTON_5, false, BUTTON_PULL, bsp_button_event_handler},
        #endif // BUTTON_5

        #ifdef BSP_BUTTON_6
        {BSP_BUTTON_6, false, BUTTON_PULL, bsp_button_event_handler},
        #endif // BUTTON_6

        #ifdef BSP_BUTTON_7
        {BSP_BUTTON_7, false, BUTTON_PULL, bsp_button_event_handler},
        #endif // BUTTON_7

    };

    err_code = app_button_enable();

    err_code = app_timer_create(&m_bsp_button_tmr,
                                APP_TIMER_MODE_SINGLE_SHOT,
                                button_timer_handler);

}


uint32_t bsp_event_to_button_action_assign(uint32_t button, bsp_button_action_t action, bsp_event_t event){
      
      uint32_t err_code = NRF_SUCCESS;

      #if BUTTONS_NUMBER > 0
          if (button < BUTTONS_NUMBER)
          {
              if (event == BSP_EVENT_DEFAULT)
              {
                  // Setting default action: BSP_EVENT_KEY_x for PUSH actions, BSP_EVENT_NOTHING for RELEASE and LONG_PUSH actions.
                  event = (action == BSP_BUTTON_ACTION_PUSH) ? (bsp_event_t)(BSP_EVENT_KEY_0 + button) : BSP_EVENT_NOTHING;
              }
              switch (action)
              {
                  case BSP_BUTTON_ACTION_PUSH:     //按鍵壓下事件指定
                      m_events_list[button].push_event = event;
                      break;
                  case BSP_BUTTON_ACTION_LONG_PUSH:   //按鍵長按事件指定
                      m_events_list[button].long_push_event = event;
                      break;
                  case BSP_BUTTON_ACTION_RELEASE:     ////按鍵釋放事件指定
                      m_events_list[button].release_event = event;
                      break;
                  default:
                      err_code = NRF_ERROR_INVALID_PARAM;
                      break;
              }
          }
          else
          {
              err_code = NRF_ERROR_INVALID_PARAM;
          }
      #else
          err_code = NRF_ERROR_INVALID_PARAM;
      #endif // BUTTONS_NUMBER > 0

          return err_code;
}
                                    

}

