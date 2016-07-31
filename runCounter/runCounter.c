/* Author: Chenyu Yao
 * Date: Mar 28th
 * purpose: This program works with the simulator, 
            prompts user for the a start value and stop value and count on the 8 LED display.
 */


#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <DAQlib.h>
#include <Windows.h>

const int ON = 1,
          OFF = 0,
          RUN  = 0,
          RESET = 1,
          Number_Of_Display = 8,
          SLEEP = 1000;


void runCounter(void);
void writeDigit (int digit, int position);
void writeNumber (int num);


int main(void) {
	int setupNUM;

	printf("Enter the DAQ device numeber: ");
	scanf("%d", &setupNUM );

	if(setupDAQ(setupNUM)){
		printf("successfully set up DAQ\n\n");
		runCounter();
	}
		
	else{
		printf("Error, Failed to initialize the DAQ\n");
	}

	system("PAUSE");
	return 0;
}

void runCounter( void ){
	int count;
    int start, stop;
	int switch_run, switch_reset;

	printf("Enter the number you wish to start to count:");
	scanf("%d", &start);
	count = start;
	printf("Enter the number you wish to stop: ");
	scanf("%d", &stop);

	printf("\nInitially, both swtiches should be off to proceed.\n");
	printf("Please make sure both switches are off.\n\n");

	do{
		switch_run = digitalRead(RUN);
		switch_reset= digitalRead(RESET);
	} while (!(switch_reset == 0 && switch_run == 0));

	printf("Both switches are now off, proceeding ... \n");
	printf("To start/stop the counter, use the RUN swtich (#0). \n");
	printf("To reset the counter, use the RESET swtich (#1). \n");

	writeNumber(count);

    while(continueSuperLoop()){

       if( digitalRead(RESET) == ON){
		    count = start;
		    writeNumber(count);
		   }

	   else if (digitalRead(RUN) == ON){

	   /*No while Loop here!!! instead, just use if statement. Break the whole counting procedure. 
	   Go though the whole loop everytime the counter counts 1 sec. */
	       if(count <= stop){
	           Sleep(SLEEP);

			   /* you need to add another control here, 
			    * so that if the switch was turned off during the sleep, the counter will still know to stop*/
			   if(digitalRead( RUN ) == ON){
		           writeNumber(count++);
			   }
	       }
	   }
	}
}
	

 void writeDigit (int digit, int position) {
	int LookUp[] = {252, 96, 218, 242, 102, 182, 190, 224, 254, 246};
	displayWrite( LookUp[digit], position);

}


void writeNumber (int num) {
	int digit;
	int pos = 0;

    while (num>0 && pos < Number_Of_Display) {
        digit = num % 10;
		writeDigit( digit, pos);
		num /= 10;
		pos++;
	}

	while (pos < Number_Of_Display){
		displayWrite(OFF, pos);/*displayWrite(0, pos) can tur off the LED display at given position*/
		pos++;
	}

}
