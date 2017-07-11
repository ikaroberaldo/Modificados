#ifndef __RIPPLEDETECTOR_H__
#define __RIPPLEDETECTOR_H__

#ifdef _WIN32
#include <Windows.h>
#endif

#include <ProcessorHeaders.h>

#define NUM_INTERVALS 5

class RippleDetector : public GenericProcessor

{
public:

    RippleDetector();
    ~RippleDetector();

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

    Array<double> times;
    Array<double> amplitudes;
    double TimeT;
    double amplitude;

    double defaultTime;
    double defaultAmplitude;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RippleDetector);

};

#endif  // __RippleDETECTOR_H__
