#include <stdlib.h>
#include <stdint.h>

#include "crc.h"
#include "uart.h"
#include "config.h"
#include "sram.h"

extern FILE uart_stdout;

uint16_t crc_xmodem_update(uint16_t crc, uint8_t data)
{
    int i;
    crc = crc ^ ((uint16_t) data << 8);
    for (i = 0; i < 8; i++) {
        if (crc & 0x8000)
            crc = (crc << 1) ^ 0x1021;
        else
            crc <<= 1;
    }
    return crc;
}

uint16_t do_crc(uint8_t * data, uint16_t size)
{
    uint16_t crc = 0;
    uint16_t i;
    for (i = 0; i < size; i++) {
        crc = crc_xmodem_update(crc, data[i]);
        
    }
    return crc;
}


uint16_t do_crc_update(uint16_t crc, uint8_t * data, uint16_t size)
{
    uint16_t i;
    for (i = 0; i < size; i++)
        crc = crc_xmodem_update(crc, data[i]);
    return crc;
}


uint16_t crc_check_bulk_memory(uint32_t bottom_addr,uint32_t top_addr)
{
    uint16_t crc = 0;
    uint32_t addr = 0;
    uint8_t req_bank = 0;
    sram_bulk_read_start(bottom_addr);
    printf("crc_check_bulk_memory: bottom_addr=0x%08lx top_addr=0x%08lx\n",
        bottom_addr, top_addr);
    
    for (addr = bottom_addr; addr < top_addr; addr++) {
        if (addr && addr % 0x8000 == 0) {
            printf("crc_check_bulk_memory: bank=0x%02x addr=0x%08lx crc=0x%04x\n",
                req_bank,addr,crc);
            req_bank++;
            crc = 0;
        }
        crc = crc_xmodem_update(crc, sram_bulk_read());
        sram_bulk_read_next();
    }
    if (addr % 0x8000 == 0)
        printf("crc_check_bulk_memory: bank=0x%02x addr=0x%08lx crc=0x%04x\n",
            req_bank,addr,crc);
    sram_bulk_read_end();
    return crc;
}



void crc_check_memory(uint32_t bottom_addr,uint32_t top_addr,uint8_t *buffer)
{
    uint16_t crc = 0;
    uint32_t addr;
    uint8_t req_bank = 0;
    for (addr = bottom_addr; addr < top_addr; addr += TRANSFER_BUFFER_SIZE) {
        if (addr && addr % 0x8000 == 0) {
            printf("crc_check_memory: bank=0x%02x addr=0x%08lx crc=0x%04x\n",
                req_bank,addr,crc);
            req_bank++;
            crc = 0;
        }
        sram_read_buffer(addr, buffer, TRANSFER_BUFFER_SIZE);
        crc = do_crc_update(crc, buffer, TRANSFER_BUFFER_SIZE);
    }
}



uint16_t crc_check_memory_range(uint32_t start_addr, uint32_t size,uint8_t *buffer)
{
    uint16_t crc = 0;
    uint32_t addr;
    for (addr = start_addr; addr < start_addr + size; addr += TRANSFER_BUFFER_SIZE) {
        sram_read_buffer(addr, buffer, TRANSFER_BUFFER_SIZE);
        crc = do_crc_update(crc, buffer, TRANSFER_BUFFER_SIZE);
    }
    return crc;
}
