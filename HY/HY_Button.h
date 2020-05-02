#ifndef HY_BUTTON_H
#define HY_BUTTON_H

#define typedef unsigned long int ret_code_t   //uint32_t 

void buttons_leds_init(bool * p_erase_bonds);

void bsp_init();
uint32_t bsp_event_to_button_action_assign(uint32_t button, bsp_button_action_t action, bsp_event_t event);


#endif