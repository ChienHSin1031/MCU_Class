static void delete_bonds(void)
{
    ret_code_t err_code;

    NRF_LOG_INFO("Erase bonds!");
    SEGGER_RTT_printf(0, "Erase bonds!\n");

    err_code = pm_peers_delete();
    APP_ERROR_CHECK(err_code);
}