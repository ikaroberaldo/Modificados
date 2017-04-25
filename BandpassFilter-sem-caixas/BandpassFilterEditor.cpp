#include "BandpassFilterEditor.h"
#include "BandpassFilter.h"
#include <stdio.h>


BandpassFilterEditor::BandpassFilterEditor(GenericProcessor* parentNode, bool useDefaultParameterEditors=true)
    : GenericEditor(parentNode, useDefaultParameterEditors)

{
    desiredWidth = 150;

    lastLowCutString = " ";
    lastHighCutString = " ";

    highCutLabel = new Label("high cut label", "High cut:");
    highCutLabel->setBounds(10,65,80,20);
    highCutLabel->setFont(Font("Small Text", 12, Font::plain));
    highCutLabel->setColour(Label::textColourId, Colours::darkgrey);
    addAndMakeVisible(highCutLabel);

    lowCutLabel = new Label("low cut label", "Low cut:");
    lowCutLabel->setBounds(10,25,80,20);
    lowCutLabel->setFont(Font("Small Text", 12, Font::plain));
    lowCutLabel->setColour(Label::textColourId, Colours::darkgrey);
    addAndMakeVisible(lowCutLabel);

    lowCutValue = new Label("low cut value", lastLowCutString);
    lowCutValue->setBounds(15,42,60,18);
    lowCutValue->setFont(Font("Default", 15, Font::plain));
    lowCutValue->setColour(Label::textColourId, Colours::white);
    lowCutValue->setColour(Label::backgroundColourId, Colours::grey);
    lowCutValue->setEditable(true);
    lowCutValue->addListener(this);
    lowCutValue->setTooltip("Set the low cut for the selected channels");
    addAndMakeVisible(lowCutValue);

    highCutValue = new Label("high cut label", lastHighCutString);
    highCutValue->setBounds(15,82,60,18);
    highCutValue->setFont(Font("Default", 15, Font::plain));
    highCutValue->setColour(Label::textColourId, Colours::white);
    highCutValue->setColour(Label::backgroundColourId, Colours::grey);
    highCutValue->setEditable(true);
    highCutValue->addListener(this);
    highCutValue->setTooltip("Set the high cut for the selected channels");
    addAndMakeVisible(highCutValue);
/*
    applyFilterOnADC = new UtilityButton("+ADCs",Font("Default", 10, Font::plain));
    applyFilterOnADC->addListener(this);
    applyFilterOnADC->setBounds(90,70,40,18);
    applyFilterOnADC->setClickingTogglesState(true);
    applyFilterOnADC->setTooltip("When this button is off, ADC channels will not be filtered");
    addAndMakeVisible(applyFilterOnADC);

    applyFilterOnChan = new UtilityButton("+CH",Font("Default", 10, Font::plain));
    applyFilterOnChan->addListener(this);
    applyFilterOnChan->setBounds(95,95,30,18);
    applyFilterOnChan->setClickingTogglesState(true);
    applyFilterOnChan->setToggleState(true, dontSendNotification);
    applyFilterOnChan->setTooltip("When this button is off, selected channels will not be filtered");
    addAndMakeVisible(applyFilterOnChan);
*/
}

BandpassFilterEditor::~BandpassFilterEditor()
{

}

void BandpassFilterEditor::setDefaults(double lowCut, double highCut)
{
    lastHighCutString = String(roundFloatToInt(highCut));
    lastLowCutString = String(roundFloatToInt(lowCut));

    highCutValue->setText(lastHighCutString, dontSendNotification);
    lowCutValue->setText(lastLowCutString, dontSendNotification);
}


void BandpassFilterEditor::labelTextChanged(Label* label)
{
    BandpassFilter* fn = (BandpassFilter*) getProcessor();

    Value val = label->getTextValue();
    double requestedValue = double(val.getValue());

    if (requestedValue < 0.01 || requestedValue > 10000)
    {
        CoreServices::sendStatusMessage("Value out of range.");

        if (label == highCutValue)
        {
            label->setText(lastHighCutString, dontSendNotification);
            lastHighCutString = label->getText();
        }
        else
        {
            label->setText(lastLowCutString, dontSendNotification);
            lastLowCutString = label->getText();
        }

        return;
    }

    Array<int> chans = getActiveChannels();

    for (int n = 0; n < chans.size(); n++)
    {

        if (label == highCutValue)
        {
//            double minVal = fn->getLowCutValueForChannel(chans[n]);

//            if (requestedValue > minVal)
//            {
                fn->setCurrentChannel(chans[n]);
                fn->setParameter(1, requestedValue);
//            }

            lastHighCutString = label->getText();

        }
        else
        {
//            double maxVal = fn->getHighCutValueForChannel(chans[n]);

//            if (requestedValue < maxVal)
//            {
                fn->setCurrentChannel(chans[n]);
                fn->setParameter(0, requestedValue);
//           }

            lastLowCutString = label->getText();
        }

    }

}
/*
void BandpassFilterEditor::channelChanged (int channel, bool )
{
    BandpassFilter* fn = (BandpassFilter*) getProcessor();

    highCutValue->setText (String (fn->getHighCutValueForChannel (channel)), dontSendNotification);
    lowCutValue->setText  (String (fn->getLowCutValueForChannel  (channel)), dontSendNotification);
//    applyFilterOnChan->setToggleState (fn->getBypassStatusForChannel (channel), dontSendNotification);

}
*//*
void BandpassFilterEditor::buttonEvent(Button* button)
{

    if (button == applyFilterOnADC)
    {
        BandpassFilter* fn = (BandpassFilter*) getProcessor();
        fn->setApplyOnADC(applyFilterOnADC->getToggleState());

    }
    else if (button == applyFilterOnChan)
    {
        BandpassFilter* fn = (BandpassFilter*) getProcessor();

        Array<int> chans = getActiveChannels();

        for (int n = 0; n < chans.size(); n++)
        {
            float newValue = button->getToggleState() ? 1.0 : 0.0;

            fn->setCurrentChannel(chans[n]);
            fn->setParameter(2, newValue);
        }
   // }
}*/


void BandpassFilterEditor::saveCustomParameters(XmlElement* xml)
{

    xml->setAttribute("Type", "BandpassFilterEditor");

    lastHighCutString = highCutValue->getText();
    lastLowCutString = lowCutValue->getText();

    XmlElement* textLabelValues = xml->createNewChildElement("VALUES");
    textLabelValues->setAttribute("HighCut",lastHighCutString);
    textLabelValues->setAttribute("LowCut",lastLowCutString);
//    textLabelValues->setAttribute("ApplyToADC",	applyFilterOnADC->getToggleState());
}

void BandpassFilterEditor::loadCustomParameters(XmlElement* xml)
{

    forEachXmlChildElement(*xml, xmlNode)
    {
        if (xmlNode->hasTagName("VALUES"))
        {
            highCutValue->setText(xmlNode->getStringAttribute("HighCut"),dontSendNotification);
            lowCutValue->setText(xmlNode->getStringAttribute("LowCut"),dontSendNotification);
//            applyFilterOnADC->setToggleState(xmlNode->getBoolAttribute("ApplyToADC",false), sendNotification);
        }
    }


}
