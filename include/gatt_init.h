
NRF_BLE_GATT_DEF(m_gatt);                                           /**< GATT module instance. */


static void gatt_init(void)
{
    ret_code_t err_code = nrf_ble_gatt_init(&m_gatt, gatt_evt_handler);
    APP_ERROR_CHECK(err_code);
}