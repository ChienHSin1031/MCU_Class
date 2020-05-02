static void gatt_evt_handler(nrf_ble_gatt_t * p_gatt, nrf_ble_gatt_evt_t const * p_evt)
{
    if (p_evt->evt_id == NRF_BLE_GATT_EVT_ATT_MTU_UPDATED)
    {
        NRF_LOG_INFO("GATT ATT MTU on connection 0x%x changed to %d.",
                     p_evt->conn_handle,
                     p_evt->params.att_mtu_effective);
        SEGGER_RTT_printf(0, "GATT ATT MTU on connection 0x%x changed to %d.\n",
                     p_evt->conn_handle,
                     p_evt->params.att_mtu_effective);                
    }

    ble_hrs_on_gatt_evt(&m_hrs, p_evt);
}