#include <SwitecX25.h>
#include <MicroView.h>
#include <TimerOne.h>

//////////////////////////////////////////////////////////////////////////////////////////////////
// configurables
//////////////////////////////////////////////////////////////////////////////////////////////////

#define AN_FILTER_SZ                200             // number of samples for the averaging array
#define TEMP_RESOLUTION             ((float)0.25)   // °C/mv
#define TEMP_OFFSET                 ((float)0.0)    // °C
#define MV_TEMP_GAUGE_MIN           0 //°C
#define MV_TEMP_GAUGE_MAX           1000 //°C
#define AN_GAUGE_DEGREES            315
#define AN_GAUGE_STEPS              (AN_GAUGE_DEGREES*3)
#define AN_GAUGE_MIN                0.0
#define AN_GAUGE_MAX                1000.0
#define AN_GAUGE_DIR_ANTICLOCKWISE  0
#define AN_GAUGE_DIR_CLOCKWISE      1
#define AN_GAUGE_DIR                   AN_GAUGE_DIR_CLOCKWISE


//////////////////////////////////////////////////////////////////////////////////////////////////
// variables
//////////////////////////////////////////////////////////////////////////////////////////////////

int             sensorPin = A0;     // pin to use for analog readings
int             sensorMv = 0;       // filtered voltage to use for calculations
double          temperature = 0;    // calcluated temperature from sensor
unsigned long   runtime = 0;        // runtime in ms
SwitecX25       anGauge( AN_GAUGE_STEPS, 2, 3, 5, 6 );


//////////////////////////////////////////////////////////////////////////////////////////////////
// function prototypes
//////////////////////////////////////////////////////////////////////////////////////////////////

void TimerInterrupt( void );
void MicroViewDisplay_Init( void );
void MicroViewDisplay_Update( double Temperature, int Voltage );
void DisplayTemperature( double Temperature );
void DisplayVoltage( int Voltage );
int GetMotorPosition( double Value, double Min, double Max );


//////////////////////////////////////////////////////////////////////////////////////////////////
// main functions
//////////////////////////////////////////////////////////////////////////////////////////////////

void setup( void )
{
    Timer1.initialize( 1000 ); // interrupt every 1000us, 1ms
    Timer1.attachInterrupt( TimerInterrupt );
    MicroViewDisplay_Init( );
    anGauge.zero( );
}

void loop( ) 
{
    anGauge.update( );   
}

void TimerInterrupt( void )
{
    runtime++;

    sensorMv = AverageSensorValue( analogRead( sensorPin ) );
    temperature = (((double)sensorMv * TEMP_RESOLUTION) + TEMP_OFFSET);

    if ((runtime % 100) == 0)
    {
        MicroViewDisplay_Update( temperature, sensorMv );
    }
    
    if ((runtime % 100) == 1)
    {
        anGauge.setPosition( GetMotorPosition( temperature, AN_GAUGE_MIN, AN_GAUGE_MAX ) );
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// function implementations
//////////////////////////////////////////////////////////////////////////////////////////////////

int GetMotorPosition( double Value, double Min, double Max )
{
    Value = min( Max, max( Min, Value ) );      // limit value from Min to Max
    Value = (Value - Min) / (Max - Min);        // generate a 0-1 value
    if (AN_GAUGE_DIR == AN_GAUGE_DIR_CLOCKWISE) // flip percent if going clockwise
        Value = (1.0 - Value);
    return (int)(((double)AN_GAUGE_STEPS * Value) + 0.5);  // get the step position to send to motor
}
