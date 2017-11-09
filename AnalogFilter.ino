unsigned int anFilter[AN_FILTER_SZ];// array of sensor mv readings to use for filtering
int iAnFilter = 0;                  // current index of the filter
bool anFilterInit = false;          // if false, fill all elements of array to value for quick start

//////////////////////////////////////////////////////////////////////////////////////////////////
// accepts an analog reading, 0-1023 counts, return a filtered, averaged 0-5000mV value
int AverageSensorValue( int Reading )
{
    unsigned long avg = 0;  // ulong for addition of up to 200 elements
    int i = 0;              // current index of the averating process

    // convert 0-1023 counts to mv, add 0.5 for truncation rounding
    avg = (unsigned long)(((float)Reading / 1023.0 * 5000.0) + 0.5);

    if (!anFilterInit)
    {
        // set filter
        anFilterInit = true;
        
        for (i = 0; i < AN_FILTER_SZ; i++)
            anFilter[i] = avg;
    }
    else
    {
        // get next array index
        if (++iAnFilter >= AN_FILTER_SZ)
            iAnFilter = 0;
    
        // save value
        anFilter[iAnFilter] = avg;
    }

    // add the values...
    for (i = 0, avg = 0; i < AN_FILTER_SZ; i++)
    {
        avg += anFilter[i];
    }

    // and divide
    avg = avg / AN_FILTER_SZ;

    // return the sensor average
    return (int)avg;
}
