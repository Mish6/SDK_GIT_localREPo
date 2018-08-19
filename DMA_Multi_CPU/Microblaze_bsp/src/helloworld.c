/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "xaxicdma.h"
#include "platform.h"
#include "xil_printf.h"



int main()
{

    init_platform();
    unsigned int *addr;
    unsigned int *addr2;
    unsigned int value=0x0000a001;
    unsigned int *stopaddr;
    unsigned int *DMA_From;
    unsigned int *DMA_to;
    unsigned int *DMA_Byte;

	XAxiCdma_Config *CfgPtr_DMA_0;
	XAxiCdma_Config *CfgPtr_DMA_1;
	static XAxiCdma AxiCdmaInstance_0;
	static XAxiCdma AxiCdmaInstance_1;
	int Status;

    stopaddr=(unsigned int *) 0x40800040;
	stopaddr=(unsigned int *)0x40800040;
    addr=(unsigned int *)0x408002f0;
    *addr = 0xcafecafe;
    DMA_From=(unsigned int *)0x40000018;
    DMA_to=(unsigned int *)0x40000020;
    DMA_Byte=(unsigned int *)0x40000028;
    addr=(unsigned int *) 0x40800300;
    xil_printf("this is a value of address 0x%x",*addr);

    addr2=(unsigned int *)0x40800000;

while(addr2<stopaddr)
{
	   *addr2=value;
    	addr2++;
    	value++;
	}
// *DMA_From=0xc0000000;
// *DMA_to=0xc0000f00;
// *DMA_Byte=0x00000044;
   /* for(addr2;&addr2=0xc0000040;addr2++)
    *{
    *	*addr2 = value;
    *	 value++;
    *}
*/

print("Start DMA Transfer Test\n\r");

CfgPtr_DMA_0 = XAxiCdma_LookupConfig(XPAR_AXICDMA_0_DEVICE_ID);
if (!CfgPtr_DMA_0) {
	return XST_FAILURE;
}

Status = XAxiCdma_CfgInitialize(&AxiCdmaInstance_0, CfgPtr_DMA_0,
		CfgPtr_DMA_0->BaseAddress);
if (Status != XST_SUCCESS) {
	return XST_FAILURE;
}

CfgPtr_DMA_1 = XAxiCdma_LookupConfig(XPAR_AXICDMA_1_DEVICE_ID);
if (!CfgPtr_DMA_1) {
	return XST_FAILURE;
}

Status = XAxiCdma_CfgInitialize(&AxiCdmaInstance_1, CfgPtr_DMA_1,
		CfgPtr_DMA_1->BaseAddress);
if (Status != XST_SUCCESS) {
	return XST_FAILURE;
}


while (XAxiCdma_IsBusy(&AxiCdmaInstance_0))
{
	int i;
	i++;
}

print("Start DMA_1 Transfer\n\r");


Status = XAxiCdma_SimpleTransfer(&AxiCdmaInstance_0, (UINTPTR)0x40800000,
	(UINTPTR)0x40800a00, 0x00000044, NULL, NULL);

print("End Of DMA_1 Transfer\n\r");

while (XAxiCdma_IsBusy(&AxiCdmaInstance_1))
{
    int i;
    i++;
}
print("Start DMA_2 Transfer\n\r");


Status = XAxiCdma_SimpleTransfer(&AxiCdmaInstance_1, (UINTPTR)0x40800000,
	(UINTPTR)0x40800a50, 0x00000044, NULL, NULL);

print("End Of DMA_2 Transfer\n\r");



while (1)
{
    print("Hello World\n\r");
}
    cleanup_platform();
    return 0;
}
