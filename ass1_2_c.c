//GPIO Start Adress
#define GPIO_START 0x20200000UL

//GPIO Port Function 
#define INPUT	0
#define OUTPUT	1
#define ALT_FUNC0 4
#define ALT_FUNC1 5 
#define ALT_FUNC2 6
#define ALT_FUNC3 7
#define ALT_FUNC4 3
#define ALT_FUNC5 2

//GPIO Port Number to adjust the function
#define STATUS_LED 16
#define PWM_PORT 18
#define LED_PORT 24


//Depending on Port funciton "1" and "0" have diffrent functions.
//Setting First 30 GPIO Pins to "1"
#define GPIO_SET0 7UL
//Setting rest to "1"
#define GPIO_SET1 8UL
//Setting First 30 GPIO Pins to "0" 
#define GPIO_CLR0 10UL
//Setting rest to "0"
#define GPIO_CLR1 11UL

//Position inside the GPIO Array. Each has 10x (3bits) ports to adjust it's function
#define GPIO_FUNC_SEL0 0UL
#define GPIO_FUNC_SEL1 1UL
#define GPIO_FUNC_SEL2 2UL
#define GPIO_FUNC_SEL3 3UL
#define GPIO_FUNC_SEL4 4UL
#define GPIO_FUNC_SEL5 5UL

//gpio_start can change outside this program due to hardware.
//To prevent that the compiler discards it we have to add the volatile keyword
volatile unsigned int* gpio_start;
volatile unsigned int tim;

//Set a function to a port (like: Make port 10 into an Output => setPortFunction(10, OUTPUT) 
void setPortFunction(char port, char func)
{
	//Int division: get the ten's digit
	char funcSel = port / 10;
	unsigned int sel = 0;
	//Base on the ten's digit select the proper bit array to adjust the function
	switch(funcSel)
	{
		case 0:
			sel = GPIO_FUNC_SEL0;
			break;
		case 1:
			sel = GPIO_FUNC_SEL1;
			break;
		case 2:
			sel = GPIO_FUNC_SEL2;
			break;
		case 3:
			sel = GPIO_FUNC_SEL3;
			break;
		case 4:
			sel = GPIO_FUNC_SEL4;
			break;
		case 5:
			sel = GPIO_FUNC_SEL5;
			break;
	}		
	if(port >= 0 && port <= 53 && func >= 0 && func <= 7)
	{
		// 7 = 111 => Reseting all 3 bits of the port to 0
		gpio_start[sel] &= ~(7<<(port%10)*3);
		//Set the function bits pattern
		gpio_start[sel] |= (func<<(port%10)*3);
	}
}

//Set the PIN to High/Low, depending if PIN is an Outout or Input
void turnOn(unsigned int port, char type)
{
	if(type == INPUT)
	{
		if(port > 31)
			gpio_start[GPIO_CLR1] |= (1<<(port%32));
		else
			gpio_start[GPIO_CLR0] |= (1<<port);
	}
	else if(type == OUTPUT)
	{
		if(port > 31)
			gpio_start[GPIO_SET1] |= (1<<(port%32));
		else
			gpio_start[GPIO_SET0] |= (1<<port);
	}
}	

//Set the PIN to High/Low, depending if PIN is an Outout or Input
void turnOff(unsigned int port, char type)
{
	if(type == INPUT)
	{
		if(port > 31)
			gpio_start[GPIO_SET1] |= (1<<(port%32));
		else
			gpio_start[GPIO_SET0] |= (1<<port);
	}
	else if(type == OUTPUT)
	{
		if(port > 31)
			gpio_start[GPIO_CLR1] |= (1<<(port%32));
		else
			gpio_start[GPIO_CLR0] |= (1<<port);
	}
}

void main(void)
{
	gpio_start = (unsigned int*)GPIO_START;
	setPortFunction(LED_PORT, OUTPUT);
	while(1)
	{
		for(int counter = 0; counter < 3; counter++)
		{
			for(tim = 0; tim < 500000; tim++);
			turnOn(LED_PORT, OUTPUT);
			for(tim = 0; tim < 500000; tim++);
			turnOff(LED_PORT, OUTPUT);
		}
		for(int counter = 0; counter < 5; counter++)
		{
			for(tim = 0; tim < 500000; tim++);
		}
	};
}