#ifndef __HISTORY_LOG_H__
#define __HISTORY_LOG_H__

int history_log_init(void);

int history_log_output_enable(void);
int history_log_output_disable(void);

int history_log_save(uint32_t timestamp, void* data, size_t size);
int history_log_report_saved(void *mqtt_client);
int history_log_report_output_logs(void *mqtt_client);

#endif //__HISTORY_LOG_H__