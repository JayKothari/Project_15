#define COM1

#ifdef COM1
	#define BASE 0x3f8
#endif

/*Below three registers share the same address it can differentiate with DLAB bit in LCR */

#define TxBuf BASE+0		//Transmit Buffer register access with DLAB = 0

#define RxBuf BASE+0		//Recive Buffer register access with DLAB = 0

#define BaudRegL BASE+0		//Baud rate set register Low Byte it can be access with DLAB = 1


/*Below two register share the same address it can differentiate with DLAB bit in LCR */


#define BaudRegH BASE+1		//Baud rate set register High Byte it can be access with DLAB = 1

#define IER	BASE+1		/* Interrupt Enable Register it can be access with DLAB = 0
					Bit 0 :Enable Received Data Available Interrupt 
					Bit 1 :Enable Transmitter Holding Register Empty Interrupt */


/*Below two register share the same address */

#define FCR BASE+2		/*FIFO control register->Enabling FIFO Tx n Rx buffers only Writing can b don
					Bit 0 : Enable FIFO's
					Bit 1 : Clear Recieve FIFO
					Bit 2 :	Clear Transmit FIFO*/

		
#define IntIdentReg BASE+2	//Interrupt Identification Register only reading gives Intr Status 


#define LCR BASE+3		/*Line Control register 
					Bit 7: 1 Device Latch access bit(Baud rate)
					     : 0 Access to Transmit,Reciever Buffer and IntEnable Register
					Bit 5,4,3 :  x x 0 	No Parity			
						     0 0 1	Odd parity
						     0 1 1	Even parity
						     1 0 1	High Parity(sticky)
						     1 1 1	Low parity(sticky)
					Bit 2 : 0 One stop bit
						1 two stop bits
					Bit 1,0 :	0 0	5 Bits
							0 1	6 Bits
							1 0	7 Bits
							1 1 	8 Bits*/
					 			

#define MCR BASE+4		//Modem Control register

#define LSR BASE+5		//Line Status register only Read

#define MSR BASE+6		//Modem Status register

#define ScratchReg BASE+7	//Scratch register


#ifdef COM1
#define Serial_IRQ 4
#endif






