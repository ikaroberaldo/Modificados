#ifndef __THRESHOLDEDITOR_H_969BDB5__
#define __THRESHOLDEDITOR_H_969BDB5__


#include <EditorHeaders.h>

class FilterViewport;                                                                                         //User interface for the... 
                                                                                                              //... ThresholdNode processor
class ThresholdEditor : public GenericEditor,
    public Label::Listener
{
public:
    ThresholdEditor(GenericProcessor* parentNode, bool useDefaultParameterEditors);
    virtual ~ThresholdEditor();

    void buttonEvent(Button* button);
    void labelTextChanged(Label* label);

    void saveCustomParameters(XmlElement* xml);
    void loadCustomParameters(XmlElement* xml);

    void setDefaults(double lowCut, double highCut);

    void channelChanged (int chan, bool newState);

private:

    String lastHighCutString;
    String lastLowCutString;

    ScopedPointer<Label> highCutLabel;
    ScopedPointer<Label> lowCutLabel;

    ScopedPointer<Label> highCutValue;
    ScopedPointer<Label> lowCutValue;
    ScopedPointer<UtilityButton> applyFilterOnADC;
    ScopedPointer<UtilityButton> applyFilterOnChan;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ThresholdEditor);

};



#endif
