
#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "nrf_sdm.h"
#include "app_error.h"
#include "ble.h"
#include "ble_err.h"
#include "ble_hci.h"
#include "ble_srv_common.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_bas.h"
#include "ble_hrs.h"
#include "ble_dis.h"
#include "ble_conn_params.h"
#include "sensorsim.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "nrf_sdh_soc.h"
#include "app_timer.h"
#include "bsp_btn_ble.h"
#include "peer_manager.h"
#include "peer_manager_handler.h"
#include "fds.h"
#include "nrf_ble_gatt.h"
#include "nrf_ble_lesc.h"
#include "nrf_ble_qwr.h"
#include "ble_conn_state.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_drv_clock.h"

#include "HY_BLE.h"
#include "HY_Framework.h"
#include "HY_MsgQ.h"


static int count = 1;


// when timeout run one time
void HY_BLE_Update_HeartRate_Event(void){

  static heart=60;

//  NRF_LOG_INFO("HY_BLE_Update_HeartRate_Event");
  ble_hearrate_update(heart++);

  if (heart > 200)
    heart=60;
  
}

void HY_test_Event(){

    if(count == 5){
      HY_PostMsgQ(EVENT_one_second);
    }
    if(count == 7){
      HY_PostMsgQ(EVENT_seven_seconds);
      count = 0;
    }
    count++; 
    NRF_LOG_INFO("1 second Event");
}

void HY_EVENT_one_second(){
    NRF_LOG_INFO("Event of count is 5 seconds");

}

void HY_EVENT_seven_seconds(){
    NRF_LOG_INFO("Event of count is 7 seconds");
}


int main(void){
    
    int nEvent = INVALID_EVENT;  //-1
    HY_TIMER_init();
    HY_initMsgQ();
    HY_initFramework();
    ble_init();
        
    //HY_InstallEventHandler(int 0x80, func HY_InstallEventHandler)
    HY_InstallEventHandler(EVENT_BLE_HEART_RATE_UPDATE, HY_BLE_Update_HeartRate_Event);
    HY_InstallEventHandler(Event_HY_test_Event ,HY_test_Event);
    HY_InstallEventHandler(EVENT_one_second, HY_EVENT_one_second);
    HY_InstallEventHandler(EVENT_seven_seconds, HY_EVENT_seven_seconds);
    
    // Enter main loop.
    for (;;)
    {
                                
        nEvent = HY_GetMsgQ();   //get the Q's storage Event
                   
        do{
            
            if(HY_ProcessEventHandler(nEvent) != RLT_SUCCESS) 
            {
                //WINDOW *pWin = NULL;
                //pWin = HLX_GetCurrentWindow();
                //HLX_ProcWindowEvent(pWin, nEvent);
            }

            nEvent = HY_GetMsgQ();

            
        }while(nEvent != INVALID_EVENT);

        ble_hy_idele_state_handle();

    }
}




