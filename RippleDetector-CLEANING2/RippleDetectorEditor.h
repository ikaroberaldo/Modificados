#ifndef __RippleDETECTOR2EDITOR_H__
#define __RippleDETECTOR2EDITOR_H__

#include <EditorHeaders.h>

class RippleInterface;
class RippleDetector2;
class ElectrodeButton;                                                                                        //Não tirar ainda
class FilterViewport;

class RippleDetector2Editor : public GenericEditor,
    public ComboBox::Listener,
    public Label::Listener
{
public:
    RippleDetector2Editor(GenericProcessor* parentNode, bool useDefaultParameterEditors);
    virtual ~RippleDetector2Editor();

    void comboBoxChanged(ComboBox* c);

    void updateSettings();

    void saveCustomParameters(XmlElement* xml);
    void loadCustomParameters(XmlElement* xml);
    
    void labelTextChanged(Label* label);

    void setDefaults(double Time, double Amplitude);

private:

    void addDetector();

    OwnedArray<RippleInterface> interfaces;

    int previousChannelCount;

    Array<Colour> backgroundColours;
    
    String lastAmplitudeString;
    String lastTimeString;

    ScopedPointer<Label> AmplitudeLabel;
    ScopedPointer<Label> TimeLabel;

    ScopedPointer<Label> AmplitudeValue;
    ScopedPointer<Label> TimeValue;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RippleDetector2Editor);

};

class RippleInterface : public Component,
    public ComboBox::Listener,
    public Button::Listener
{
public:
    RippleInterface(RippleDetector2*, Colour, int);
    ~RippleInterface();

    void comboBoxChanged(ComboBox*);
    void buttonClicked(Button*);                                                                              //Tirar junto com o Listener

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

    RippleDetector2* processor;

    OwnedArray<ElectrodeButton> phaseButtons;                                                                 //Não tirar ainda

    ScopedPointer<ComboBox> inputSelector;
    ScopedPointer<ComboBox> gateSelector;
    ScopedPointer<ComboBox> outputSelector;

};

#endif  // __RippleDETECTOR2EDITOR_H__
