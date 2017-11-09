MicroViewWidget *TemperatureGauge;  // display for temp reading, at top of display
MicroViewWidget *VoltageGauge;      // display for an voltage, at bottom of display


void MicroViewDisplay_Init( void )
{
    uView.begin( );
    uView.clear( PAGE );

    TemperatureGauge = new MicroViewSlider( 0, 0, MV_TEMP_GAUGE_MIN, MV_TEMP_GAUGE_MAX, WIDGETSTYLE1 + WIDGETNOVALUE );
    VoltageGauge = new MicroViewSlider( 0, 38, 0, 5000, WIDGETSTYLE1 + WIDGETNOVALUE );
}

void MicroViewDisplay_Update( double Temperature, int Voltage )
{
    uView.clear(PAGE);
    DisplayTemperature( Temperature );
    DisplayVoltage( Voltage );
    uView.display( );
}

void DisplayTemperature( double Temperature )
{    
    // bump the cursor down by size of widget (small text font height)
    uView.setFontType( 0 ); // sm font
    uView.setCursor( 0, uView.getFontHeight( ) + 1 );
    uView.setFontType( 1 );

    // add correct amount of spaces to right justify reading
    if (Temperature < 10)
        uView.print( "    " );
    else if (Temperature < 100)
        uView.print( "   " );
    else if (Temperature < 1000)
        uView.print( "  " );
    else
        uView.print( " " );

    // print the value in large font
    uView.print( Temperature, 0 );
    uView.print( " C");

    // limit the temp value before passing to gauge, make sure it gets written
    Temperature = min( MV_TEMP_GAUGE_MAX, max( MV_TEMP_GAUGE_MIN, Temperature ) );

    // draw the gauge
    TemperatureGauge->reDraw( );
    TemperatureGauge->setValue( (int)Temperature );
}

void DisplayVoltage( int Voltage )
{
    uView.setFontType( 0 );         // small font
    uView.setCursor( 0, 28 );          // values configured to display correctly... trickyish...
    uView.print( "  " );
    uView.print( (float)Voltage/1000.0, 3 );
    uView.print( " V" );
    VoltageGauge->reDraw( );
    VoltageGauge->setValue( Voltage );
}

