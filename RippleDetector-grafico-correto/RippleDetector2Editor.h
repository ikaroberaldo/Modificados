//****************************************************> significa que veio do Bandpass

#ifndef __RippleDETECTOR2EDITOR_H_136829C6__
#define __RippleDETECTOR2EDITOR_H_136829C6__

#include <EditorHeaders.h>

//****************************************************
class FilterViewport;
//****************************************************
class RippleInterface;
class RippleDetector2;
class ElectrodeButton;

class RippleDetector2Editor : public GenericEditor,
    public ComboBox::Listener,
//****************************************************
    public Label::Listener
//****************************************************
{
public:
    RippleDetector2Editor(GenericProcessor* parentNode, bool useDefaultParameterEditors);
    virtual ~RippleDetector2Editor();
//****************************************************
    void labelTextChanged(Label* label);

    void setDefaults(double lowCut, double highCut);
//****************************************************
    void buttonEvent(Button* button);

    void comboBoxChanged(ComboBox* c);

    void updateSettings();                                                                                    //Necessário

    void saveCustomParameters(XmlElement* xml);
    void loadCustomParameters(XmlElement* xml);
private:

//    ScopedPointer<ComboBox> detectorSelector;

//****************************************************
    String lastHighCutString;
    String lastLowCutString;

    ScopedPointer<Label> highCutLabel;
    ScopedPointer<Label> lowCutLabel;

    ScopedPointer<Label> highCutValue;
    ScopedPointer<Label> lowCutValue;
//****************************************************
    ScopedPointer<UtilityButton> plusButton;

    void addDetector();

    OwnedArray<RippleInterface> interfaces;

    int previousChannelCount;

    Array<Colour> backgroundColours;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RippleDetector2Editor);

};

class RippleInterface : public Component,
    public ComboBox::Listener,
    public Button::Listener
{
public:
    RippleInterface(RippleDetector2*, Colour, int);
    ~RippleInterface();

    void paint(Graphics& g);

    void comboBoxChanged(ComboBox*);
    void buttonClicked(Button*);

    void updateChannels(int);

    void setPhase(int);
    void setInputChan(int);
    void setOutputChan(int);
    void setGateChan(int);

    int getPhase();
    int getInputChan();
    int getOutputChan();
    int getGateChan();

private:

    Colour backgroundColour;

    Path sineWave;
    Font font;

    int idNum;

    RippleDetector2* processor;

    OwnedArray<ElectrodeButton> phaseButtons;

    ScopedPointer<ComboBox> inputSelector;
    ScopedPointer<ComboBox> gateSelector;
    ScopedPointer<ComboBox> outputSelector;

};

#endif  // __RippleDETECTOR2EDITOR_H_136829C6__
