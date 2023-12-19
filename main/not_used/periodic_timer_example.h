// static void periodic_timer_callback(void *arg)
// {
//     // esp_timer_stop(periodic_timer);
//     // if (data_buffer.len > 0) {
//     //     esp_receive_apple_data_source(data_buffer.buffer, data_buffer.len);
//     //     memset(data_buffer.buffer, 0, data_buffer.len);
//     //     data_buffer.len = 0;
//     // }
//     ESP_LOGI(TAG, "periodic_timer_callback");
//     if (mqtt_client)
//         (KEY_ID_HP_RAK_WORK_DATA, mqtt_client);
// }

// static void start_mqtt_data_timer()
// {
//     const esp_timer_create_args_t periodic_timer_args = {
//         .callback = &periodic_timer_callback,
//         /* name is optional, but may help identify the timer when debugging */
//         .name = "periodic"};

//     esp_timer_handle_t periodic_timer;
//     ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
//     int seconds = 300;
//     ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, seconds * 1000000));
// }