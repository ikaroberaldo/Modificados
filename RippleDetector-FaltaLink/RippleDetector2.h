//****************************************************> significa que veio do Bandpass

#ifndef __RIPPLEDETECTOR2_H_F411F29D__
#define __RIPPLEDETECTOR2_H_F411F29D__

#ifdef _WIN32
#include <Windows.h>
#endif

#include <ProcessorHeaders.h>
//#include "Dsp/Dsp.h"

#define NUM_INTERVALS 5

class RippleDetector2 : public GenericProcessor

{
public:

    RippleDetector2();
    ~RippleDetector2();


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
    
//****************************************************    
//    AudioProcessorEditor* createEditor() /*override*/;

    bool hasEditor2() const /*override*/ { return true; }

    void process2 (AudioSampleBuffer& buffer, MidiBuffer& midiMessages) /*override*/;

    void setParameter2 (int parameterIndex, float newValue) /*override*/;

    void updateSettings2() /*override*/;

    void saveCustomChannelParametersToXml   (XmlElement* channelInfo, int channelNumber, bool isEventChannel) /*override*/;
    void loadCustomChannelParametersFromXml (XmlElement* channelInfo, bool isEventChannel)  /*override*/;
    
    double getLowCutValueForChannel  (int chan) const;
    double getHighCutValueForChannel (int chan) const;
//****************************************************

private:

    enum ModuleType
    {
        NONE, PEAK, FALLING_ZERO, TROUGH, RISING_ZERO
    };

    enum PhaseType
    {
        NO_PHASE, RISING_POS, FALLING_POS, FALLING_NEG, RISING_NEG
    };

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
        ModuleType type;
        PhaseType phase;
    };
        
    Array<DetectorModule> modules;

    int activeModule;
    
    void handleEvent(int eventType, MidiMessage& event, int sampleNum);

//    bool risingPos, risingNeg, fallingPos, fallingNeg;

//    void estimateFrequency();

//****************************************************
    void setFilterParameters (double, double, int);

    Array<double> lowCuts;
    Array<double> highCuts;

//ææ    OwnedArray<Dsp::Filter> filters;

    double defaultLowCut;
    double defaultHighCut;
    
//****************************************************
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RippleDetector2);

};

#endif  // __RippleDETECTOR2_H_F411F29D__
