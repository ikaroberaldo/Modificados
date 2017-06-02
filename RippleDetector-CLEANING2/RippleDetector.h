#ifndef __RIPPLEDETECTOR2_H__
#define __RIPPLEDETECTOR2_H__

#ifdef _WIN32
#include <Windows.h>
#endif

#include <ProcessorHeaders.h>

#define NUM_INTERVALS 5

class RippleDetector2 : public GenericProcessor

{
public:

    RippleDetector2();
    ~RippleDetector2();

    double getTimeValueForChannel  (int chan) const;
    double getAmplitudeValueForChannel (int chan) const;

    void process(AudioSampleBuffer& buffer, MidiBuffer& midiMessages);
    void setParameter(int parameterIndex, float newValue);
    
    
    AudioProcessorEditor* createEditor();

    bool hasEditor() const
    {
        return false;
    }

    bool enable();

    void updateSettings();

    void addModule();
    void setActiveModule(int);
    
    bool hasEditor2() const { return true; }


private:

    struct DetectorModule
    {

        int inputChan;
        int gateChan;
        int outputChan;
        bool isActive;
        int samplesSinceTrigger;
        bool wasTriggered;
        int cntLimits;
        int SR;        
        float MED;
        float STD;
        int AvgCount;
        int flag;
        double tReft;
	int count;
	float lastSample;
    };
        
    Array<DetectorModule> modules;

    int activeModule;
    
    void handleEvent(int eventType, MidiMessage& event, int sampleNum);

    void setFilterParameters (double, double, int);

    Array<double> TimeS;
    Array<double> AmplitudeS;
    double TimeT;
    double Amplitude;

    double defaultTime;
    double defaultAmplitude;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RippleDetector2);

};

#endif  // __RippleDETECTOR2_H__
