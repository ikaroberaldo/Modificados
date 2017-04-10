/*
    ------------------------------------------------------------------

    This file is part of the Open Ephys GUI
    Copyright (C) 2014 Open Ephys

    ------------------------------------------------------------------

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "ThresholdEditor.h"
#include "ThresholdNode.h"
#include <stdio.h>


ThresholdEditor::ThresholdEditor(GenericProcessor* parentNode, bool useDefaultParameterEditors=true)
    : GenericEditor(parentNode, useDefaultParameterEditors)                                                   //Não sei

{
    desiredWidth = 150;                                                                                       //Tamanho do módulo

    lastLowCutString = " ";
    lastHighCutString = " ";                                                                                  //Inicializa string vazia

    highCutLabel = new Label("high cut label", "High cut:");
    highCutLabel->setBounds(10,65,80,20);
    highCutLabel->setFont(Font("Small Text", 12, Font::plain));
    highCutLabel->setColour(Label::textColourId, Colours::darkgrey);
    addAndMakeVisible(highCutLabel);                                                                          //Adiciona "HIGH CUT:"

    lowCutLabel = new Label("low cut label", "Low cut:");
    lowCutLabel->setBounds(10,25,80,20);
    lowCutLabel->setFont(Font("Small Text", 12, Font::plain));
    lowCutLabel->setColour(Label::textColourId, Colours::darkgrey);
    addAndMakeVisible(lowCutLabel);                                                                           //Adiciona "LOW CUT:"

    lowCutValue = new Label("low cut value", lastLowCutString);
    lowCutValue->setBounds(15,42,60,18);
    lowCutValue->setFont(Font("Default", 15, Font::plain));
    lowCutValue->setColour(Label::textColourId, Colours::white);
    lowCutValue->setColour(Label::backgroundColourId, Colours::grey);
    lowCutValue->setEditable(true);
    lowCutValue->addListener(this);
    lowCutValue->setTooltip("Set the low cut for the selected channels");
    addAndMakeVisible(lowCutValue);                                                                           //Adiciona caixa do Low Cut

    highCutValue = new Label("high cut label", lastHighCutString);
    highCutValue->setBounds(15,82,60,18);
    highCutValue->setFont(Font("Default", 15, Font::plain));
    highCutValue->setColour(Label::textColourId, Colours::white);
    highCutValue->setColour(Label::backgroundColourId, Colours::grey);
    highCutValue->setEditable(true);
    highCutValue->addListener(this);
    highCutValue->setTooltip("Set the high cut for the selected channels");
    addAndMakeVisible(highCutValue);                                                                          //Adiciona caixa do High Cut

    applyFilterOnADC = new UtilityButton("+ADCs",Font("Default", 10, Font::plain));
    applyFilterOnADC->addListener(this);
    applyFilterOnADC->setBounds(90,70,40,18);
    applyFilterOnADC->setClickingTogglesState(true);
    applyFilterOnADC->setTooltip("When this button is off, ADC channels will not be filtered");
    addAndMakeVisible(applyFilterOnADC);                                                                      //Adiciona Botão "+ADCs"

    applyFilterOnChan = new UtilityButton("+CH",Font("Default", 10, Font::plain));
    applyFilterOnChan->addListener(this);
    applyFilterOnChan->setBounds(95,95,30,18);
    applyFilterOnChan->setClickingTogglesState(true);
    applyFilterOnChan->setToggleState(true, dontSendNotification);
    applyFilterOnChan->setTooltip("When this button is off, selected channels will not be filtered");
    addAndMakeVisible(applyFilterOnChan);                                                                     //Adiciona Botão "+CH"

}

ThresholdEditor::~ThresholdEditor()
{

}                                                                                                             //Não sei

void ThresholdEditor::setDefaults(double lowCut, double highCut)
{
    lastHighCutString = String(roundFloatToInt(highCut));
    lastLowCutString = String(roundFloatToInt(lowCut));

    highCutValue->setText(lastHighCutString, dontSendNotification);
    lowCutValue->setText(lastLowCutString, dontSendNotification);
}                                                                                                             //Não sei


void ThresholdEditor::labelTextChanged(Label* label)
{
    ThresholdNode* fn = (ThresholdNode*) getProcessor();

    Value val = label->getTextValue();
    double requestedValue = double(val.getValue());

    if (requestedValue < 0.01 || requestedValue > 10000)
    {
        CoreServices::sendStatusMessage("Value out of range.");                                               //Notifica sobre valores...
                                                                                                              //... fora do range
        if (label == highCutValue)
        {
            label->setText(lastHighCutString, dontSendNotification);
            lastHighCutString = label->getText();                                                             //Recebe o Input do High
        }
        else
        {
            label->setText(lastLowCutString, dontSendNotification);
            lastLowCutString = label->getText();                                                              //Recebe o Input do Low
        }

        return;
    }

    Array<int> chans = getActiveChannels();

    // This needs to change, since there's not enough feedback about whether
    // or not individual channel settings were altered:

    for (int n = 0; n < chans.size(); n++)
    {

        if (label == highCutValue)
        {
            double minVal = fn->getLowCutValueForChannel(chans[n]);

            if (requestedValue > minVal)
            {
                fn->setCurrentChannel(chans[n]);
                fn->setParameter(1, requestedValue);
            }

            lastHighCutString = label->getText();

        }
        else
        {
            double maxVal = fn->getHighCutValueForChannel(chans[n]);

            if (requestedValue < maxVal)
            {
                fn->setCurrentChannel(chans[n]);
                fn->setParameter(0, requestedValue);
            }

            lastLowCutString = label->getText();                                                              //Não sei
        }

    }

}

void ThresholdEditor::channelChanged (int channel, bool /*newState*/)
{
    ThresholdNode* fn = (ThresholdNode*) getProcessor();

    highCutValue->setText (String (fn->getHighCutValueForChannel (channel)), dontSendNotification);
    lowCutValue->setText  (String (fn->getLowCutValueForChannel  (channel)), dontSendNotification);
    applyFilterOnChan->setToggleState (fn->getBypassStatusForChannel (channel), dontSendNotification);

}                                                                                                             //Não sei

void ThresholdEditor::buttonEvent(Button* button)
{

    if (button == applyFilterOnADC)
    {
        ThresholdNode* fn = (ThresholdNode*) getProcessor();
        fn->setApplyOnADC(applyFilterOnADC->getToggleState());

    }
    else if (button == applyFilterOnChan)
    {
        ThresholdNode* fn = (ThresholdNode*) getProcessor();

        Array<int> chans = getActiveChannels();

        for (int n = 0; n < chans.size(); n++)
        {
            float newValue = button->getToggleState() ? 1.0 : 0.0;

            fn->setCurrentChannel(chans[n]);
            fn->setParameter(2, newValue);
        }
    }
}                                                                                                             //Não sei


void ThresholdEditor::saveCustomParameters(XmlElement* xml)
{

    xml->setAttribute("Type", "ThresholdEditor");

    lastHighCutString = highCutValue->getText();
    lastLowCutString = lowCutValue->getText();

    XmlElement* textLabelValues = xml->createNewChildElement("VALUES");
    textLabelValues->setAttribute("HighCut",lastHighCutString);
    textLabelValues->setAttribute("LowCut",lastLowCutString);
    textLabelValues->setAttribute("ApplyToADC",	applyFilterOnADC->getToggleState());
}                                                                                                             //Salva dados como xml

void ThresholdEditor::loadCustomParameters(XmlElement* xml)
{

    forEachXmlChildElement(*xml, xmlNode)
    {
        if (xmlNode->hasTagName("VALUES"))
        {
            highCutValue->setText(xmlNode->getStringAttribute("HighCut"),dontSendNotification);
            lowCutValue->setText(xmlNode->getStringAttribute("LowCut"),dontSendNotification);
            applyFilterOnADC->setToggleState(xmlNode->getBoolAttribute("ApplyToADC",false), sendNotification);
        }
    }


}                                                                                                             //Carrega dados do xml
