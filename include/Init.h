
void Init(void){
    bool erase_bonds;

    // Initialize.
    log_init();
    timers_init();
    buttons_leds_init(&erase_bonds);
    power_management_init();
    ble_stack_init();
    gap_params_init();
    gatt_init();
    advertising_init();
    services_init();
    sensor_simulator_init();
    conn_params_init();
    peer_manager_init();

    // Start execution.
    NRF_LOG_INFO("Heart Rate Sensor example started.");
    SEGGER_RTT_printf(0, "Heart Rate Sensor example started.\n");

    application_timers_start();
    advertising_start(erase_bonds);
    NRF_LOG_PROCESS();
}