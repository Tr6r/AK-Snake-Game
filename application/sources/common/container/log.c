#include "log.h"
#include "task_log.h"

void init_log_ring_buffer(ring_log_buffer_t *rb, uint8_t *mem, uint16_t size)
{
    if (rb == NULL || mem == NULL || size == 0)
        return;

    rb->head_index = 0;
    rb->tail_index = 0;
    rb->count = 0;
    rb->buffer_size = size;
    rb->buffer = mem;
}
void print_time_sec(uint32_t seconds)
{
    uint8_t hh = seconds / 3600;
    uint8_t mm = (seconds % 3600) / 60;
    uint8_t ss = seconds % 60;
    xprintf("%02u:%02u:%02u", hh, mm, ss);
}
uint8_t ring_buffer_put_log(ring_log_buffer_t *rb, log_entry_t *entry)
{
    if (!rb || !rb->buffer || !entry)
        return 0;

    uint16_t total_len = 4 + 3 + entry->header.len; // 4 bytes timestamp + 3 bytes magic,len,type

    // Xóa log cũ nếu không đủ chỗ
    while (rb->count + total_len > rb->buffer_size)
    {
        // đọc len log cũ
        uint16_t tail = rb->tail_index;
        uint8_t old_len = rb->buffer[(tail + 1) % rb->buffer_size]; // len là byte thứ 1
        uint16_t old_total = 4 + 3 + old_len;

        rb->tail_index = (rb->tail_index + old_total) % rb->buffer_size;
        rb->count -= old_total;
    }

    uint16_t head = rb->head_index;

    // Ghi header từng byte
    rb->buffer[head % rb->buffer_size] = entry->header.magic;
    head++;
    rb->buffer[head % rb->buffer_size] = entry->header.len;
    head++;
    rb->buffer[head % rb->buffer_size] = entry->header.data_type;
    head++;

    rb->buffer[head % rb->buffer_size] = (entry->header.timestamp >> 0) & 0xFF;
    head++;
    rb->buffer[head % rb->buffer_size] = (entry->header.timestamp >> 8) & 0xFF;
    head++;
    rb->buffer[head % rb->buffer_size] = (entry->header.timestamp >> 16) & 0xFF;
    head++;
    rb->buffer[head % rb->buffer_size] = (entry->header.timestamp >> 24) & 0xFF;
    head++;

    // Ghi dữ liệu log
    for (uint16_t i = 0; i < entry->header.len; i++)
    {
        rb->buffer[head % rb->buffer_size] = entry->data[i];
        head++;
    }

    rb->head_index = head % rb->buffer_size;
    rb->count += total_len;

    return 1;
}

void ring_buffer_print_logs(ring_log_buffer_t *rb)
{
    if (!rb || !rb->buffer || rb->count == 0)
    {
        xprintf("Ring buffer empty or invalid.\n");
        return;
    }

    uint16_t idx = rb->tail_index;
    uint16_t remaining = rb->count;

    while (remaining >= 7) // tối thiểu 7 byte header
    {
        // Deserialize header từng byte
        uint8_t magic = rb->buffer[idx % rb->buffer_size];
        idx++;
        uint8_t len = rb->buffer[idx % rb->buffer_size];
        idx++;
        uint8_t type = rb->buffer[idx % rb->buffer_size];
        idx++;

        uint32_t timestamp = 0;
        timestamp |= ((uint32_t)rb->buffer[idx % rb->buffer_size]) << 0;
        idx++;
        timestamp |= ((uint32_t)rb->buffer[idx % rb->buffer_size]) << 8;
        idx++;
        timestamp |= ((uint32_t)rb->buffer[idx % rb->buffer_size]) << 16;
        idx++;
        timestamp |= ((uint32_t)rb->buffer[idx % rb->buffer_size]) << 24;
        idx++;

        // Kiểm tra còn đủ dữ liệu payload
        if (remaining < 7 + len)
            break;
        xprintf("Log | time:");
        print_time_sec(timestamp);
        xprintf("|len: %u | type: %u | magic: 0x%02X | data:",

                len,
                type,
                magic);

        // In payload
        for (uint16_t i = 0; i < len; i++)
        {
            uint16_t data_idx = (idx + i) % rb->buffer_size;
            xprintf(" %02X", rb->buffer[data_idx]);
        }
        xprintf("\n");

        // Di chuyển tới log kế tiếp
        idx = (idx + len) % rb->buffer_size;
        remaining -= (7 + len);
    }
}

void log_buffer_to_eeprom(ring_log_buffer_t *rb)
{
    if (!rb || !rb->buffer)
        return;

    ring_eeprom_t meta;
    meta.head_index = rb->head_index,
    meta.tail_index = rb->tail_index,
    meta.count = rb->count,
    meta.buffer_size = rb->buffer_size;

    uint32_t addr = EEPROM_START_ADDR;

    // Tổng kích thước cần lưu
    uint32_t total_len = sizeof(meta) + rb->buffer_size;

    // Xóa vùng trước khi ghi
    eeprom_erase(addr, total_len);

    // Ghi metadata
    eeprom_write(addr, (uint8_t *)&meta, sizeof(meta));
    addr += sizeof(meta);

    // Ghi nội dung log (các log dynamic)
    eeprom_write(addr, rb->buffer, rb->buffer_size);

    // reset ring buffer
    rb->tail_index = rb->head_index;
    rb->count = 0;
}

void print_logs_from_eeprom(ring_log_buffer_t *rb)
{
    if (!rb)
        return;

    uint32_t addr = EEPROM_START_ADDR;
    ring_eeprom_t meta;

    eeprom_read(addr, (uint8_t *)&meta, sizeof(meta));
    addr += sizeof(meta);

    rb->buffer = buffer_log;

    if (meta.buffer_size > BUFFER_LOG_SIZE)
        meta.buffer_size = BUFFER_LOG_SIZE;

    rb->buffer_size = meta.buffer_size;

    eeprom_read(addr, rb->buffer, rb->buffer_size);

    rb->head_index = meta.head_index;
    rb->tail_index = meta.tail_index;
    rb->count      = meta.count;

    if (rb->count > rb->buffer_size)
        rb->count = rb->buffer_size;
}
void log_clear_eeprom(void)
{
    eeprom_erase(EEPROM_START_ADDR, EEPROM_END_ADDR - EEPROM_START_ADDR);
        xprintf("CLEAR DONE");

}