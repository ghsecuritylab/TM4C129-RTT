/*
 * File      : application.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2014, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2014-07-18     ArdaFu       the first version for TM4C129X
 */

#include <rtthread.h>
#include <components.h>
#include "board.h"
#include "stdio.h"
#include "uart_app.h"

struct rt_messagequeue mq_uart;
struct rt_messagequeue mq_spi;

//��Ϣ�����ڴ��
static char mq_uart_msg_pool[8 * 8];
static char mq_spi_msg_pool[8 * 8];

/* thread phase init */
void rt_init_thread_entry(void *parameter)
{
	while(1)
	{
		static int i  = 0;
		rt_thread_delay(500);
		//printf("hello %d \n", i++);
		if(i > 8) i = 0;
		//rt_hw_uart_send( i, (rt_uint8_t *)"UART0_DATA INIT\r\n", rt_strlen("UART0_DATA INIT\r\n"));
		i++;
	}
	
}

int rt_application_init(void)
{
  rt_thread_t tid;
	
	
	 /* ��ʼ����Ϣ���� */
	rt_mq_init(&mq_uart, "mq_uart",
						 &mq_uart_msg_pool[0],        /* �ڴ��ָ��modbus_485_mq_msg_pool */
						 sizeof(struct Uart_tx_info_t) , /* ÿ����Ϣ�Ĵ�С */
						 sizeof(mq_uart_msg_pool),    /* �ڴ�صĴ�С */
						 RT_IPC_FLAG_FIFO);   /* ����ж���̵߳ȴ������������ȵõ��ķ���������Ϣ */
						 
	rt_mq_init(&mq_spi, "mq_spi",
						 &mq_spi_msg_pool[0],        /* �ڴ��ָ��modbus_485_mq_msg_pool */
						 sizeof(struct Uart_rx_info_t) , /* ÿ����Ϣ�Ĵ�С */
						 sizeof(mq_spi_msg_pool),    /* �ڴ�صĴ�С */
						 RT_IPC_FLAG_FIFO);   /* ����ж���̵߳ȴ������������ȵõ��ķ���������Ϣ */
						 
						 
	tid = rt_thread_create("uart_2_spi",
												 uart_2_spi_thread, RT_NULL,
												 2048, 8, 20);
						 
	if (tid != RT_NULL) rt_thread_startup(tid);

	tid = rt_thread_create("spi_2_uart",
												 spi_2_uart_thread, RT_NULL,
												 2048, 8, 20);
						 
	if (tid != RT_NULL) rt_thread_startup(tid);
						 
						 
	tid = rt_thread_create("init",
												 rt_init_thread_entry, RT_NULL,
												 1024, 8, 20);
	if (tid != RT_NULL) rt_thread_startup(tid);

	return 0;
}

