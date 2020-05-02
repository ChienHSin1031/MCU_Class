#ifndef HY_BLE_H
#define HY_BLE_H

void ble_init(void);
void ble_hy_idele_state_handle(void);

void HY_TIMER_init(void);
void ble_hearrate_update(int heart_rate);

#endif
