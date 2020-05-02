static void on_adv_evt(ble_adv_evt_t ble_adv_evt)
{
    ret_code_t err_code;

    switch (ble_adv_evt)
    {
        case BLE_ADV_EVT_FAST:
            NRF_LOG_INFO("Fast advertising.");
            SEGGER_RTT_printf(0, "Fast advertising.\n");
            err_code = bsp_indication_set(BSP_INDICATE_ADVERTISING);
            APP_ERROR_CHECK(err_code);
            break;
	

        case BLE_ADV_EVT_IDLE:
            sleep_mode_enter();
            SEGGER_RTT_printf(0, "sleep 02\n");
            break;

        default:
            break;
    }
}
