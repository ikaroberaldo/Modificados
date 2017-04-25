#ifndef __BandpassFilter_H_CED428E__
#define __BandpassFilter_H_CED428E__

#include <ProcessorHeaders.h>
#include "Dsp/Dsp.h"


class BandpassFilter : public GenericProcessor
{
public:
    BandpassFilter();
    ~BandpassFilter();

    AudioProcessorEditor* createEditor() override;

    bool hasEditor() const override { return true; }

    void process (AudioSampleBuffer& buffer, MidiBuffer& midiMessages) override;

    void setParameter (int parameterIndex, float newValue) override;

    void updateSettings() override;

    void saveCustomChannelParametersToXml   (XmlElement* channelInfo, int channelNumber, bool isEventChannel) override;
    void loadCustomChannelParametersFromXml (XmlElement* channelInfo, bool isEventChannel)  override;

//    double getLowCutValueForChannel  (int chan) const;
//    double getHighCutValueForChannel (int chan) const;

//    bool getBypassStatusForChannel (int chan) const;

//    void setApplyOnADC (bool state);


private:
    void setFilterParameters (double, double, int);

    Array<double> lowCuts;
    Array<double> highCuts;

    OwnedArray<Dsp::Filter> filters;
//    Array<bool> shouldFilterChannel;

//    bool applyOnADC;

    double defaultLowCut;
    double defaultHighCut;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BandpassFilter);
};

#endif  // __BandpassFilter_H_CED428E__
