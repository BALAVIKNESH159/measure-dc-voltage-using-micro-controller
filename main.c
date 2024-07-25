 #include <xc.h>
#inlude ?clcd.h?
#define _XTAL_FREQ 20000000    // Define oscillator frequency for delay

void ADC_Init()
{
    ADFM=1; // Selecting Right Justification
    ADON= 1; // Starting the ADC Module
}


unsigned int ADC_Read(unsigned char channel)
{
    if(channel > 7) 
    {
         return 0;  // ADC has 8 channels, 0-7
    }
    ADCON0 =ADCON0 & 0xC5;     // Clear existing channel selection bits
    ADCON0 =(ADCON0 &0XC7) | (channel << 3);  // Set new channel
    __delay_ms(2);  // Acquisition time to charge the hold capacitor
    GO= 1;  // Start ADC conversion
    while(GO==1);  // Wait for conversion to complete
    return (ADRESL | (ADRESH << 8) );  // Return result
}
void display(unsigned float adc_reg_val_1)
{ 
       char buff[5];
       int i;
       i = 3; // buff: "1 0 2 3"
      do
     {
           buff[i] = (adc_reg_val_1 % 10) + '0';
           adc_reg_val_1 = adc_reg_val_1 / 10; 
      } while (i--); 
      buff[4] = '\0';
    
       clcd_print(buff, LINE1(0));
}

void main()
{
    unsigned int adc_value;
    float voltage, input_voltage;
                

    TRISA = 0xFF;  // Configure PORTA as input
    ADC_Init();  // Initialize ADC

    while(1)
    {
        adc_value = ADC_Read(0);  // Read ADC value from channel 0
        voltage = (adc_value * 5.0) / 1023.0;  // Assuming Vref+ is 5V and 10-bit ADC resolution
        input_voltage = voltage * ((100.0 + 3.3) / 3.3);  // Convert to input voltage

        // Now you can use input_voltage as needed
        __delay_ms(500);  // Delay for a while
        display(input_voltage);
    }
}

