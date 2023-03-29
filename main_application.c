// Driver includes
#include "HWS_Driver/HWS_conf.h"
static const char character[] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x06D, 0x7D, 0x07, 0x7F, 0x6F };

// Priorities at which the tasks are created
#define task_prioritet		( tskIDLE_PRIORITY + 2 )

// Kernel includes
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"
#include "I_Vezba.h"

static mxDisp7seg_Handle myDisp;
static SemaphoreHandle_t binSem1, binSem2, binSem3, binSem4, binSem5;
static TimerHandle_t timer1;
static uint8_t dispMem[5];

// Local function declaration
static void task1(void* pvParams);
static void task2(void* pvParams);
static void task3(void* pvParams);
static void task4(void* pvParams);
//static void task5(void* pvParams);
static void TimerCallBack(TimerHandle_t tmr);


// Local function implementation
static void task1(void* pvParams) {
	static uint8_t value = 0;
	xTimerStart(timer1, portMAX_DELAY);

	while (1) {
		xSemaphoreTake(binSem1, portMAX_DELAY);

		//mxDisp7seg_SelectDigit(myDisp, 4); //prvi deo zadatka
		//mxDisp7seg_SetDigit(myDisp, character[value]); //prvi deo zadatka

		dispMem[0] = value;

		value++;
		if (value == 10) {
			value = 0;
		}
		if (value == 1){
			xSemaphoreGive(binSem2);

		}
	}
}

static void task2(void* pvParams) {
	static uint8_t value = 0;
	while (1) {
		xSemaphoreTake(binSem2, portMAX_DELAY);

		//mxDisp7seg_SelectDigit(myDisp, 3);
		//mxDisp7seg_SetDigit(myDisp, character[value]);

		dispMem[1] = value;

		value++;
		if (value == 10) {
			value = 0;
		}
		if (value == 1) {
			xSemaphoreGive(binSem3);

		}
	}
}
static void task3(void* pvParams) {
	static uint8_t value = 0;
	while (1) {
		xSemaphoreTake(binSem3, portMAX_DELAY);

		//mxDisp7seg_SelectDigit(myDisp, 3);
		//mxDisp7seg_SetDigit(myDisp, character[value]);

		dispMem[2] = value;

		value++;
		if (value == 10) {
			value = 0;
		}
		if (value == 1) {
			xSemaphoreGive(binSem4);

		}
	}
}
static void task4(void* pvParams) {
	static uint8_t value = 0;
	while (1) {
		xSemaphoreTake(binSem4, portMAX_DELAY);

		//mxDisp7seg_SelectDigit(myDisp, 3);
		//mxDisp7seg_SetDigit(myDisp, character[value]);

		dispMem[3] = value;

		value++;
		if (value == 10) {
			value = 0;
		}
		if (value == 1) {
			xSemaphoreGive(binSem5);

		}
	}
}


static void TimerCallBack (TimerHandle_t tmr) {
	// xSemaphoreGive(binSem1); //prvi deo zadatka
	static uint8_t ctrl = 4;
	static uint8_t count = 0;
	mxDisp7seg_SelectDigit(myDisp, ctrl);
	mxDisp7seg_SetDigit(myDisp, character[dispMem[4-ctrl]]);
	ctrl--;
	if (ctrl == 0) {
		ctrl = 4;
	}
	count++;
		if (count == 10) {
			count = 0;
			xSemaphoreGive(binSem1);
		}
}

void III_vezba_1(void)
{
	// Inicijalizacija drajvera za displej
	mxDisp7seg_Init();
	myDisp = mxDisp7seg_Open(MX7_DISPLAY_0);
	dispMem[0] = 0;
	dispMem[1] = 0;
	dispMem[2] = 0;
	dispMem[3] = 0;
	dispMem[4] = 0;

	// Kreiranje taskova 
	
	xTaskCreate(task1, NULL, configMINIMAL_STACK_SIZE, NULL, task_prioritet, NULL);
	xTaskCreate(task2, NULL, configMINIMAL_STACK_SIZE, NULL, task_prioritet, NULL);
	xTaskCreate(task3, NULL, configMINIMAL_STACK_SIZE, NULL, task_prioritet, NULL);
	xTaskCreate(task4, NULL, configMINIMAL_STACK_SIZE, NULL, task_prioritet, NULL);
	//xTaskCreate(task5, NULL, configMINIMAL_STACK_SIZE, NULL, task_prioritet, NULL);
	//xTaskCreate(task3, NULL, configMINIMAL_STACK_SIZE, NULL, task_prioritet, NULL);

	// Kreiranje softverskih tajmera 
	timer1 = xTimerCreate(NULL, pdMS_TO_TICKS(20), pdTRUE, NULL, TimerCallBack);
	
	// Kreiranje binarnih semafora 
	binSem1 = xSemaphoreCreateBinary();
	binSem2 = xSemaphoreCreateBinary();
	binSem3 = xSemaphoreCreateBinary();
	binSem4 = xSemaphoreCreateBinary();
	binSem5 = xSemaphoreCreateBinary();
	// Pokretanje rasporedjivaca
	vTaskStartScheduler();
	while (1);
}