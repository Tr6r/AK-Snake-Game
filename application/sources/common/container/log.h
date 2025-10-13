

#ifndef __LOG_H__
#define __LOG_H__

#ifdef __cplusplus

extern "C"
{
#endif
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "app_eeprom.h"
#include "eeprom.h"
    typedef enum
    {
        LOG_TYPE_INFO = 0,
        LOG_TYPE_WARNING,
        LOG_TYPE_ERROR,
        LOG_TYPE_SENSOR,
        LOG_TYPE_COMM_RX,
        LOG_TYPE_COMM_TX, // dữ liệu gửi đi
        LOG_TYPE_SYSTEM,
        LOG_TYPE_COUNT
    } log_type_t;

    typedef struct
    {
        uint8_t magic;
        uint8_t len;
        uint8_t data_type;
        uint32_t timestamp;
    } log_header_t;

    typedef struct
    {
        log_header_t header;
        uint8_t data[10];
    } log_entry_t;

    typedef struct
    {
        uint16_t head_index;  // vị trí ghi tiếp theo
        uint16_t tail_index;  // vị trí đọc tiếp theo
        uint16_t count;       // số phần tử hiện có trong buffer
        uint16_t buffer_size; // tổng số slot
        uint8_t *buffer;      // con trỏ đến vùng nhớ
    } ring_log_buffer_t;

    typedef struct
    {
        uint16_t head_index;
        uint16_t tail_index;
        uint16_t count;
        uint16_t buffer_size;
    } ring_eeprom_t;
    
    void init_log_ring_buffer(ring_log_buffer_t *rb, uint8_t *mem, uint16_t size);
    uint8_t ring_buffer_put_log(ring_log_buffer_t *rb, log_entry_t *entry);

    void ring_buffer_print_logs(ring_log_buffer_t *rb);
    void log_buffer_to_eeprom(ring_log_buffer_t *rb);
    void print_logs_from_eeprom(ring_log_buffer_t *rb);
    void log_clear_eeprom(void);
#ifdef __cplusplus
}
#endif

#endif //__LOG_H__
