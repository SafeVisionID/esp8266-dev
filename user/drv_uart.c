/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2016 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS ESP8266 only, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "ets_sys.h"
#include "osapi.h"
#include "driver/uart.h"
#include "driver/uart_register.h"
#include "mem.h"
#include "os_type.h"
#include "user_interface.h"

#include "analog.h"

extern UartDevice    UartDev;

// Achmadi's Here
char uart_rx_buffer[100];
uint8 uart_rx_cnt = 0;
uint8 uart_rx_send = 0;

uint16 vadc;
uint64 vsleep=1000000; //1s

#define uart_recvTaskPrio        0
#define uart_recvTaskQueueLen    10
os_event_t    uart_recvTaskQueue[uart_recvTaskQueueLen];

#define DBG
#define DBG1 uart1_sendStr_no_wait
#define DBG2 os_printf

LOCAL void uart0_rx_intr_handler(void *para);

LOCAL void ICACHE_FLASH_ATTR
uart_config(uint8 uart_no)
{
    if (uart_no == UART1){
        PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_U1TXD_BK);
    }else{
        ETS_UART_INTR_ATTACH(uart0_rx_intr_handler,  &(UartDev.rcv_buff));
        PIN_PULLUP_DIS(PERIPHS_IO_MUX_U0TXD_U);
        PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0TXD_U, FUNC_U0TXD);
    }
    uart_div_modify(uart_no, UART_CLK_FREQ / (UartDev.baut_rate));

    WRITE_PERI_REG(UART_CONF0(uart_no), ((UartDev.exist_parity & UART_PARITY_EN_M)  <<  UART_PARITY_EN_S)
                                                                        | ((UartDev.parity & UART_PARITY_M)  <<UART_PARITY_S )
                                                                        | ((UartDev.stop_bits & UART_STOP_BIT_NUM) << UART_STOP_BIT_NUM_S)
                                                                        | ((UartDev.data_bits & UART_BIT_NUM) << UART_BIT_NUM_S));

    SET_PERI_REG_MASK(UART_CONF0(uart_no), UART_RXFIFO_RST | UART_TXFIFO_RST);
    CLEAR_PERI_REG_MASK(UART_CONF0(uart_no), UART_RXFIFO_RST | UART_TXFIFO_RST);

    if (uart_no == UART0){
        WRITE_PERI_REG(UART_CONF1(uart_no),
        ((100 & UART_RXFIFO_FULL_THRHD) << UART_RXFIFO_FULL_THRHD_S) |
        (0x02 & UART_RX_TOUT_THRHD) << UART_RX_TOUT_THRHD_S |
        UART_RX_TOUT_EN|
        ((0x10 & UART_TXFIFO_EMPTY_THRHD)<<UART_TXFIFO_EMPTY_THRHD_S));//wjl
        SET_PERI_REG_MASK(UART_INT_ENA(uart_no), UART_RXFIFO_TOUT_INT_ENA |UART_FRM_ERR_INT_ENA);
    }else{
        WRITE_PERI_REG(UART_CONF1(uart_no),((UartDev.rcv_buff.TrigLvl & UART_RXFIFO_FULL_THRHD) << UART_RXFIFO_FULL_THRHD_S));//TrigLvl default val == 1
    }

    WRITE_PERI_REG(UART_INT_CLR(uart_no), 0xffff);
    SET_PERI_REG_MASK(UART_INT_ENA(uart_no), UART_RXFIFO_FULL_INT_ENA|UART_RXFIFO_OVF_INT_ENA);
}

 STATUS uart_tx_one_char(uint8 uart, uint8 TxChar)
{
    while (true){
        uint32 fifo_cnt = READ_PERI_REG(UART_STATUS(uart)) & (UART_TXFIFO_CNT<<UART_TXFIFO_CNT_S);
        if ((fifo_cnt >> UART_TXFIFO_CNT_S & UART_TXFIFO_CNT) < 126) {
            break;
        }
    }
    WRITE_PERI_REG(UART_FIFO(uart) , TxChar);
    return OK;
}

LOCAL void ICACHE_FLASH_ATTR
uart1_write_char(char c)
{
    if (c == '\n'){
        uart_tx_one_char(UART1, '\r');
        uart_tx_one_char(UART1, '\n');
    }else if (c == '\r'){

    }else{
        uart_tx_one_char(UART1, c);
    }
}

LOCAL void ICACHE_FLASH_ATTR
uart0_write_char_no_wait(char c)
{
    if (c == '\n'){
        uart_tx_one_char_no_wait(UART0, '\r');
        uart_tx_one_char_no_wait(UART0, '\n');
    }else if (c == '\r'){

    }
    else{
        uart_tx_one_char_no_wait(UART0, c);
    }

}

void ICACHE_FLASH_ATTR
uart0_tx_buffer(uint8 *buf, uint16 len)
{
    uint16 i;
    for (i = 0; i < len; i++)
    {
        uart_tx_one_char(UART0, buf[i]);
    }
}

void ICACHE_FLASH_ATTR
uart0_sendStr(const char *str)
{
    while(*str){
        uart_tx_one_char(UART0, *str++);
    }
}
void at_port_print(const char *str) __attribute__((alias("uart0_sendStr")));

LOCAL void
uart0_rx_intr_handler(void *para)
{
    uint8 uart_no = UART0;
    uint8 fifo_len = 0;
    uint8 buf_idx = 0;
    uint8 temp,cnt;

    if(UART_FRM_ERR_INT_ST == (READ_PERI_REG(UART_INT_ST(uart_no)) & UART_FRM_ERR_INT_ST)){
        DBG1("FRM_ERR\r\n");
        WRITE_PERI_REG(UART_INT_CLR(uart_no), UART_FRM_ERR_INT_CLR);
    }else if(UART_RXFIFO_FULL_INT_ST == (READ_PERI_REG(UART_INT_ST(uart_no)) & UART_RXFIFO_FULL_INT_ST)){
        DBG("f");
        uart_rx_intr_disable(UART0);
        WRITE_PERI_REG(UART_INT_CLR(UART0), UART_RXFIFO_FULL_INT_CLR);
        system_os_post(uart_recvTaskPrio, 0, 0);
    }else if(UART_RXFIFO_TOUT_INT_ST == (READ_PERI_REG(UART_INT_ST(uart_no)) & UART_RXFIFO_TOUT_INT_ST)){
        DBG("t");
        uart_rx_intr_disable(UART0);
        WRITE_PERI_REG(UART_INT_CLR(UART0), UART_RXFIFO_TOUT_INT_CLR);
        system_os_post(uart_recvTaskPrio, 0, 0);
    }else if(UART_TXFIFO_EMPTY_INT_ST == (READ_PERI_REG(UART_INT_ST(uart_no)) & UART_TXFIFO_EMPTY_INT_ST)){
        DBG("e");

        CLEAR_PERI_REG_MASK(UART_INT_ENA(UART0), UART_TXFIFO_EMPTY_INT_ENA);

        WRITE_PERI_REG(UART_INT_CLR(uart_no), UART_TXFIFO_EMPTY_INT_CLR);

    }else if(UART_RXFIFO_OVF_INT_ST  == (READ_PERI_REG(UART_INT_ST(uart_no)) & UART_RXFIFO_OVF_INT_ST)){
        WRITE_PERI_REG(UART_INT_CLR(uart_no), UART_RXFIFO_OVF_INT_CLR);
        DBG1("RX OVF!!\r\n");
    }
}

// Achmadi's Here
LOCAL void ICACHE_FLASH_ATTR
uart_response(uint8 inChar){
    if(inChar == '\n' || inChar == '\r'){

        // Here are request string responses
        if(uart_rx_send == 0){
            uart0_sendStr("\r\n");

            if(os_strcmp(uart_rx_buffer,"test")==0){
                uart0_sendStr("What to test?\r\n");
            }
            else if(os_strcmp(uart_rx_buffer,"sdk")==0){
                os_printf("[INFO] SDK: %s\r\n", system_get_sdk_version());
            }
            else if(os_strcmp(uart_rx_buffer,"chip")==0){
                os_printf("[INFO] Chip ID: %08X\r\n", system_get_chip_id());
            }
            else if(os_strcmp(uart_rx_buffer,"when")==0){
                os_printf("[INFO] Compiled at %s %s\r\n", __DATE__,__TIME__);
            }
            else if(os_strcmp(uart_rx_buffer,"fcpu")==0){
                os_printf("[INFO] CPU Freq: %d MHz\r\n", system_get_cpu_freq());
            }
            else if(os_strcmp(uart_rx_buffer,"mems")==0){
                os_printf("[INFO] Memory Info:\r\n"); system_print_meminfo();
            }
            else if(os_strcmp(uart_rx_buffer,"adc")==0){
                vadc = user_get_adc();
                os_printf("[INFO] ADC Input: %4d\r\n",vadc);
            }
            else if(os_strcmp(uart_rx_buffer,"sleep")==0){
                os_printf("[INFO] Entering Deep Sleep\r\n");
                system_deep_sleep_set_option(0);
                system_deep_sleep(10*vsleep); // 10s
            }
            else{
                os_strcat(uart_rx_buffer,"\r\n");
                uart0_sendStr("Serial Data is ");
                uart0_sendStr(uart_rx_buffer);
            }

            os_memset(uart_rx_buffer,0,sizeof(uart_rx_buffer));
            uart_rx_cnt = 0;
            uart_rx_send = 1;
        }
    }
    else{
        uart_rx_buffer[uart_rx_cnt] = inChar;
        uart_rx_cnt++;
        uart_rx_send = 0;
    }
}

LOCAL void ICACHE_FLASH_ATTR ///////
uart_recvTask(os_event_t *events)
{
    if(events->sig == 0){
        uint8 fifo_len = (READ_PERI_REG(UART_STATUS(UART0))>>UART_RXFIFO_CNT_S)&UART_RXFIFO_CNT;
        uint8 d_tmp = 0;
        uint8 idx=0;
        for(idx=0;idx<fifo_len;idx++) {
            d_tmp = READ_PERI_REG(UART_FIFO(UART0)) & 0xFF;

            if(d_tmp == '\r' || d_tmp == '\n'){}
            else{ uart_tx_one_char(UART0, d_tmp); }

            uart_response(d_tmp);
        }
        WRITE_PERI_REG(UART_INT_CLR(UART0), UART_RXFIFO_FULL_INT_CLR|UART_RXFIFO_TOUT_INT_CLR);
        uart_rx_intr_enable(UART0);
    }
}

void ICACHE_FLASH_ATTR
uart_init(UartBautRate uart0_br, UartBautRate uart1_br)
{
    system_os_task(uart_recvTask, uart_recvTaskPrio, uart_recvTaskQueue, uart_recvTaskQueueLen);

    UartDev.baut_rate = uart0_br;
    uart_config(UART0);
    UartDev.baut_rate = uart1_br;
    uart_config(UART1);
    ETS_UART_INTR_ENABLE();
}

void ICACHE_FLASH_ATTR
uart_reattach()
{
    uart_init(BIT_RATE_115200, BIT_RATE_115200);
}

STATUS uart_tx_one_char_no_wait(uint8 uart, uint8 TxChar)
{
    uint8 fifo_cnt = (( READ_PERI_REG(UART_STATUS(uart))>>UART_TXFIFO_CNT_S)& UART_TXFIFO_CNT);
    if (fifo_cnt < 126) {
        WRITE_PERI_REG(UART_FIFO(uart) , TxChar);
    }
    return OK;
}

STATUS uart0_tx_one_char_no_wait(uint8 TxChar)
{
    uint8 fifo_cnt = (( READ_PERI_REG(UART_STATUS(UART0))>>UART_TXFIFO_CNT_S)& UART_TXFIFO_CNT);
    if (fifo_cnt < 126) {
        WRITE_PERI_REG(UART_FIFO(UART0) , TxChar);
    }
    return OK;
}

void uart1_sendStr_no_wait(const char *str)
{
    while(*str){
        uart_tx_one_char_no_wait(UART1, *str++);
    }
}

void uart_rx_intr_disable(uint8 uart_no)
{
    CLEAR_PERI_REG_MASK(UART_INT_ENA(uart_no), UART_RXFIFO_FULL_INT_ENA|UART_RXFIFO_TOUT_INT_ENA);
}

void uart_rx_intr_enable(uint8 uart_no)
{
    SET_PERI_REG_MASK(UART_INT_ENA(uart_no), UART_RXFIFO_FULL_INT_ENA|UART_RXFIFO_TOUT_INT_ENA);
}
