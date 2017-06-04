/*---------------------------------------------------
	main.c (v1.00)
	
	DDC-Z program, for electrocar.
----------------------------------------------------*/

#include "Main.h"
#include "port.h"

#include "Delay.h"
#include "ElecMotor.h"
                                 
/*------- Public variable declarations --------------------------*/

// ------ Private variable definitions -----------------------------

/*------- Private variable declarations --------------------------*/

void main()
	{	
	tByte itime = 0;
	
	Generator_lock = 1;
	
	while(1)
		{
		if(Generator_lock == 1)
			{
			for(itime = 0; itime < 10; itime++)
				Delay_ms(LOOP_TIMEOUT_INIT_500ms);

			ElecMotor_ACW();
			
			for(itime = 0; itime < 10; itime++)
				Delay_ms(LOOP_TIMEOUT_INIT_500ms);
			ElecMotor_CW();			
			}
		}
	}

/*---------------------------------------------------
	end of file
----------------------------------------------------*/