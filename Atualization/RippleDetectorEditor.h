#ifndef __RippleDETECTOREDITOR_H__
#define __RippleDETECTOREDITOR_H__

#include <EditorHeaders.h>

class RippleInterface;
class RippleDetector;

class RippleDetectorEditor : public GenericEditor,
    public ComboBox::Listener,
    public Label::Listener
{
public:
    RippleDetectorEditor(GenericProcessor* parentNode, bool useDefaultParameterEditors);
    virtual ~RippleDetectorEditor();

    void comboBoxChanged(ComboBox* c);

    void updateSettings();

    void saveCustomParameters(XmlElement* xml);
    void loadCustomParameters(XmlElement* xml);
    
    void labelTextChanged(Label* label);

    void setDefaults(double time, double amplitude);

private:

    void addDetector();

    OwnedArray<RippleInterface> interfaces;

    int previousChannelCount;

    Array<Colour> backgroundColours;
    
    String lastAmplitudeString;
    String lastTimeString;

    ScopedPointer<Label> amplitudeLabel;
    ScopedPointer<Label> timeLabel;

    ScopedPointer<Label> amplitudeValue;
    ScopedPointer<Label> timeValue;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RippleDetectorEditor);

};

class RippleInterface : public Component,
    public ComboBox::Listener
{
public:
    RippleInterface(RippleDetector*, Colour, int);
    ~RippleInterface();

    void paint(Graphics& g);

    void comboBoxChanged(ComboBox*);

    void updateChannels(int);

    void setInputChan(int);
    void setOutputChan(int);
    void setGateChan(int);

    int getInputChan();
    int getOutputChan();
    int getGateChan();

private:

    Colour backgroundColour;

    Path sineWave;
    Font font;

    int idNum;

    RippleDetector* processor;

    ScopedPointer<ComboBox> inputSelector;
    ScopedPointer<ComboBox> gateSelector;
    ScopedPointer<ComboBox> outputSelector;

};

#endif
