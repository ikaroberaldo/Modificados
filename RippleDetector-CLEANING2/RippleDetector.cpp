#include <stdio.h>
#include <math.h>
#include <time.h>
#include <algorithm>
#include "RippleDetector2.h"
#include "RippleDetector2Editor.h"

using namespace std;

RippleDetector2::RippleDetector2()
    : GenericProcessor("2Ripple Detector"), activeModule(-1),
      defaultTime(20.0), defaultAmplitude(2.00)
{

}

RippleDetector2::~RippleDetector2()
{

}

AudioProcessorEditor* RippleDetector2::createEditor()
{
    editor = new RippleDetector2Editor(this, true);

    cout << "Creating Editor." << endl;

    RippleDetector2Editor* ed = (RippleDetector2Editor*) getEditor();
    ed->setDefaults (defaultTime, defaultAmplitude);

    return editor;
}

void RippleDetector2::addModule()
{

    DetectorModule m = DetectorModule();
    m.inputChan = -1;
    m.outputChan = -1;
    m.gateChan = -1;
    m.isActive = true;
    m.lastSample = 0.0;
    m.samplesSinceTrigger = 5000;
    m.wasTriggered = false;
    m.MED = 0.00;
    m.STD = 0.00;
    m.AvgCount = 0;
    m.flag = 0;
    m.tReft = 0.0;
    m.count = 0;
    
    
    modules.add(m);

}


void RippleDetector2::setActiveModule(int i)
{

    activeModule = i;
}                                                                                                             


void RippleDetector2::setParameter(int parameterIndex, float newValue)
{
    DetectorModule& module = modules.getReference(activeModule);

    if (parameterIndex == 1) // module type
    {

        int val = (int) newValue;

    }
    else if (parameterIndex == 2)   // inputChan
    {
        module.inputChan = (int) newValue;
    }
    else if (parameterIndex == 3)   // outputChan
    {
        module.outputChan = (int) newValue;
    }
    else if (parameterIndex == 4)   // gateChan
    {
        module.gateChan = (int) newValue;
        if (module.gateChan < 0)
        {
            module.isActive = true; 
        }
        else
        {
            module.isActive = false;
        }
    }

    if (parameterIndex < 2) // change filter settings
    {
        if (newValue <= 0.01 || newValue >= 10000.0f)
            return;

        if (parameterIndex == 0)
        {
            TimeS.set (currentChannel,newValue);
        }
        else if (parameterIndex == 1)
        {
            AmplitudeS.set (currentChannel,newValue);
        }

        setFilterParameters (TimeS[currentChannel],
                             AmplitudeS[currentChannel],
                             currentChannel);

        editor->updateParameterButtons (parameterIndex);
    }

}

void RippleDetector2::updateSettings()
{

    int numInputs = getNumInputs();
    
    if (numInputs < 1024)
    {
        Array<double> OldTime;
        Array<double> OldAmplitude;
        OldTime = TimeS;
        OldAmplitude = AmplitudeS;

        TimeS.clear();
        AmplitudeS.clear();

        for (int n = 0; n < getNumInputs(); ++n)
        {
            float NewTime  = 0.f;
            float NewAmplitude = 0.f;

            if (OldTime.size() > n)
            {
                NewTime  = OldTime[n];
                NewAmplitude = OldAmplitude[n];
            }
            else
            {
                NewTime  = defaultTime;
                NewAmplitude = defaultAmplitude;
            }


            TimeS.add  (NewTime);
            AmplitudeS.add (NewAmplitude);

            setFilterParameters (NewTime, NewAmplitude, n);
        }
    }


}

bool RippleDetector2::enable()
{

    return true;
}

void RippleDetector2::handleEvent(int eventType, MidiMessage& event, int sampleNum)
{
    if (eventType == TTL)
    {
        const uint8* dataptr = event.getRawData();

        int eventId = *(dataptr+2);
        int eventChannel = *(dataptr+3);

        for (int i = 0; i < modules.size(); i++)
        {
            DetectorModule& module = modules.getReference(i);

            if (module.gateChan == eventChannel)
            {
                if (eventId)
                    module.isActive = true;
                else
                    module.isActive = false;
            }
        }

    }

}

double RippleDetector2::getTimeValueForChannel (int chan) const
{

    return TimeS[chan];
}

double RippleDetector2::getAmplitudeValueForChannel (int chan) const
{

    return AmplitudeS[chan];
}

void RippleDetector2::process(AudioSampleBuffer& buffer,
                            MidiBuffer& events) //This is the core of the code, is the script that will run when every buffer comes
{

    Time time; //I'm using a library to count time
    checkForEvents(events);
    // loop through the modules
    for (int i = 0; i < modules.size(); i++)
    {
      DetectorModule& module = modules.getReference(i);

        double t;    
        double t3;
        double RefratTime;
        double ThresholdAmplitude = 2.00;
        double ThresholdTime = 0.020;/*<- THIS IS THE TIME THRESHOLD*/ //Divide it for 1000 when it comes from the user input, for now

        ThresholdTime = TimeT/1000;
        ThresholdAmplitude = Amplitude;
        
            t = double(time.getHighResolutionTicks()) / double(time.getHighResolutionTicksPerSecond());//Starting to count time for the script here
            double arrSized = round(getNumSamples(module.inputChan)/4);//the following 3 lines are to create an array of specific size for saving the RMS from buffer
            int arrSize = (int) arrSized;
            float RMS[arrSize];
            for (int index = 0; index < arrSize; index++) //here the RMS is calculated
            {
                RMS[index] = sqrt( (
                   pow(buffer.getSample(module.inputChan,(index*4)),2) +
                   pow(buffer.getSample(module.inputChan,(index*4)+1),2) +
                   pow(buffer.getSample(module.inputChan,(index*4)+2),2) +
                   pow(buffer.getSample(module.inputChan,(index*4)+3),2)
                )/4 );
            }
            
            for (int pac = 0; pac < arrSize; pac++)
            {
                if (module.AvgCount < 60000/4) //Using the RMS value in the first 2 s as baseline to build a threshold of detection
                {
                    module.AvgCount++; // all the values that must be saved from one buffer to other has to be save in another function outside the process (this function), when I need to save values to reuse in the next buffer I use the structure module from addModule function
                    float var = RMS[pac];
                    float delta = var - module.MED;
                    module.MED = module.MED + (delta/module.AvgCount); //calculates average for threshold
                    module.STD = module.STD + delta*(var-module.MED); // calculates standard deviation for threhsold
                }
                else
                {
                    break;
                }
            }

            for (int i = 0; i < arrSize; i++)
            {
            
                const float sample = RMS[i];
                double threshold = module.MED + ThresholdAmplitude*sqrt(module.STD/(module.AvgCount*4)); //building the threshold from average + n*standard deviation                
                
                if ( sample >=  threshold & RefratTime > 2 ) //counting how many points are above the threshold and if has been 2 s after the last event (refractory period)
                {                
                  module.count++;
                }
		              else if(sample < threshold & i == 0)//protect from acumulation
		            {
		                 module.count = 0;
 	             	}               
                if (module.flag == 1) //if it had a detector activation, starts to recalculate refrat time
                {
                    t3 = ( double(time.getHighResolutionTicks()) / double(time.getHighResolutionTicksPerSecond()) )- module.tReft;//calculating refractory time
                    RefratTime = t3;
                }
                else //if module.flag == 0, then the script is just starting and refractory time is adjusted for a a value that works
                {
                    RefratTime = 3;
                }
       
                if (module.count >= round(ThresholdTime*30000/4) & RefratTime > 2 ) //this is the time threshold, buffer RMS amplitude must be higher than threshold for a certain period of time, the second term is the Refractory period for the detection, so it hasn't a burst of activation after reaching both thresholds
                {
			//below from here starts the activation for sending the TTL to the actuator
			
                        module.flag = 1;
			                  module.count = 0;
                        module.tReft = double(time.getHighResolutionTicks()) / double(time.getHighResolutionTicksPerSecond());
    
                        addEvent(events, TTL, i, 1, module.outputChan);
                        module.samplesSinceTrigger = 0;

                        module.wasTriggered = true;
                        
                }
                module.lastSample = sample;

                if (module.wasTriggered)
                {
                    if (module.samplesSinceTrigger > 1000)
                    {
                        addEvent(events, TTL, i, 0, module.outputChan);
                        module.wasTriggered = false;
                    }
                    else
                    {
                        module.samplesSinceTrigger++;
                    }
                }


        }

    }

    for (int n = 0; n < getNumOutputs(); ++n)
    {
            float* ptr = buffer.getWritePointer (n);
    }

}

void RippleDetector2::setFilterParameters (double a, double b, int chan)
{
    if (channels.size() - 1 < chan)
        return;
        
    TimeT = a;
    Amplitude = b;
}
