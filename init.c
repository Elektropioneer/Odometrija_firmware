#include    "init.h"

/**
 * @brief Initialize uart at 57600 Baud
 * 
 */
void UARTinit()
{
    uart_close();

    U1MODEbits.STSEL = 0;   // 1 Stop bit
    U1MODEbits.PDSEL = 0;   // No Parity, 8 data bits
    U1MODEbits.BRGH = 0;    // Low-Speed mode 
    U1BRG = 31;             // BAUD Rate Setting for 57600
    U1MODEbits.UARTEN = 1;  // Enable UART
    U1STAbits.UTXEN = 1;    // Enable TX
    IFS0bits.U1RXIF = 0;    // disable rx interrupt
     
} // end of UARTinit()

/**
 * @brief Clear the PWM interrupts and PWM control registers (turn off motors)
 * 
 */
void CloseMCPWM(void)
{
    // clear the Interrupt enables 
    IEC3bits.PWM1IE = 0;	
    IEC3bits.FLTA1IE = 0;	

    // clear the Interrupt flags 
    IFS3bits.PWM1IF = 0;	
    IFS3bits.FLTA1IF = 0;	

    // clear the PWM control registers 
    PTCON       =       0;
    PWMCON1     =       0;
    PWMCON2     =       0;

    P1TCONbits.PTEN = P2TCONbits.PTEN = 0;

} // end of CloseMCPWM(...)

/**
 * @brief Init both PWMs at 18.4kHz
 * 
 */
void PWMinit()
{	
    CloseMCPWM();

    // init PWM1
    P1TCONbits.PTEN = 0;
    PWM1CON1bits.PMOD1 = 1;     // Indepedent output mode
    PWM1CON1bits.PEN1H = 0;     // enable pwm pin for pwm
    PWM1CON1bits.PEN1L = 1;     // enable pwm pin for pwm
    P1TMR = 1;
    P1TPER = 1627;              // PWM frequency: 18425Hz
    P1DC1 = 0;
    P1TCONbits.PTEN = 1;

    // init PWM2
    P2TCONbits.PTEN = 0;
    PWM2CON1bits.PMOD1 = 1;     // Independent output mode
    PWM2CON1bits.PEN1H = 0;     // enable pwm pin for pwm
    PWM2CON1bits.PEN1L = 1;     // enable pwm pin for pwm
    P2TPER = 1627;              // PWM frquenecy: 18425Hz
    P2DC1 = 0;
    P2TCONbits.PTEN = 1;

} // end of PWMinit()

/**
 * @brief inits the timer1 used to interrupt at 1ms
 * 
 */
void TimerInit()
{
    IEC0bits.T1IE = 0;          // Disable the Timer1 interrupt 
    T1CONbits.TON = 0;          // Disable timer1 
    IFS0bits.T1IF = 0;          // Clear Timer interrupt flag 

    T1CONbits.TGATE = 0;
    T1CONbits.TCKPS = 0;
    T1CONbits.TCS = 0;

    TMR1 = 0;
    PR1 = 30000;                // 1ms

    IFS0bits.T1IF = 0;          // Clear Timer1 Interrupt Flag
    IEC0bits.T1IE = 1;          // Enable Timer1 interrupt
    T1CONbits.TON = 1;

} // end of TimerInit(...)

/**
 * @brief init the registers for the encoders
 * 
 */
void QEIinit()
{

    QEI1CONbits.POSRES=0;       // index pulse doesn't reset  measurement
    QEI1CONbits.TQCS=1;         // pulses measured on QEA output
    QEI1CONbits.UPDN_SRC=1;     // for that time it processes the info
    QEI1CONbits.QEIM=6;         // Quadrature Encoder Interface enabled (x4 mode) with index pulse reset of position counter
    QEI1CONbits.TQCKPS=0;

    MAX1CNT=0000;
    POS1CNT=0;

    QEI2CONbits.POSRES=0;       // index pulse doesn't reset  measurement 
    QEI2CONbits.TQCS=1;         // pulses measured on QEA output
    QEI2CONbits.UPDN_SRC=1;     // for that time it processes the info
    QEI2CONbits.QEIM=6;         // Quadrature Encoder Interface enabled (x4 mode) with index pulse reset of position counter
    QEI2CONbits.TQCKPS=0;

    MAX2CNT=0000;
    POS2CNT=0;

} // end of QEIInit(...)

/**
 * @brief init all ports to input
 * 
 */
void PortInit()
{

    TRISAbits.TRISA4=0;

    TRISBbits.TRISB8=0;
    TRISBbits.TRISB9=0;
    TRISBbits.TRISB10=0;
    TRISBbits.TRISB11=0;
    TRISBbits.TRISB12=0;
    TRISBbits.TRISB13=0;
    TRISBbits.TRISB14=0;
    TRISBbits.TRISB15=0;

    LATAbits.LATA4 = 0;

    LATBbits.LATB8 = 0;
    LATBbits.LATB9 = 0;
    LATBbits.LATB10 = 0;
    LATBbits.LATB11 = 0;
    LATBbits.LATB12 = 0;
    LATBbits.LATB13 = 0;
    LATBbits.LATB14 = 0;
    LATBbits.LATB15 = 0;

} // end of PortInit()

/**
 * @brief setup pins to digital; uart and encoder
 * 
 */
void PinsInit() {
    
    AD1PCFGL = 0xFFFF;// all PORT Digital

    RPINR18bits.U1RXR   = PIN_UART1_RX;		
    RPOR0bits.RP1R      = PIN_UART1_TX;		
    RPINR14bits.QEA1R   = PIN_QEA1;		    
    RPINR14bits.QEB1R   = PIN_QEB1;		    
    RPINR16bits.QEA2R   = PIN_QEA2;		    
    RPINR16bits.QEB2R   = PIN_QEB2;	

} // end of PinsInit(...)

/**
 * @brief setup registers for PLL and oscillator
 * 
 */
void OscillatorInit() {

    // set the oscillator to 30MHz
	PLLFBD = 28; 				
	CLKDIVbits.PLLPOST = 0; 	
	CLKDIVbits.PLLPRE = 0; 	

	//new oscillator selection
	__builtin_write_OSCCONH(0b011); 

	//enable oscillator source switch
	__builtin_write_OSCCONL (OSCCONL | (1<<0));

	//wait for PLL lock -> wait to new settings become available
	while (OSCCONbits.COSC != 0b011); 

	//wait for PLL lock
	while (OSCCONbits.LOCK != 0b1);

} // end of OscillatorInit(...)



// PWM function not being used but reference here
/*
void OpenMCPWM(unsigned int period, unsigned int sptime, unsigned int 
               config1, unsigned int config2, unsigned int config3)
{
    PTPER   = period;
    SEVTCMP = sptime;
    PWMCON1 = config2;
    PWMCON2 = config3;
    PTCON   = config1;
}
void OverrideMCPWM(unsigned int config)
{
    OVDCON = config;
}
void SetDCMCPWM(unsigned int dutycyclereg, unsigned int dutycycle,
                char updatedisable)
{
    PWMCON2bits.UDIS = updatedisable & 0x1;
    
    // Assign dutycycle to the duty cycle register 
    *(&PDC1+dutycyclereg -1) = dutycycle; 
}
void SetMCPWMDeadTimeAssignment(unsigned int config)
{
    DTCON2 = config ; 
}
void SetMCPWMDeadTimeGeneration (unsigned int config)
{
    DTCON1 = config;
}
void SetMCPWMFaultA(unsigned int config)
{
    FLTACON = config;
}*/
