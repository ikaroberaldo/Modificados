#include "RippleDetector2Editor.h"
#include "RippleDetector2.h"

#include <stdio.h>
#include <cmath>

using namespace std;

RippleDetector2Editor::RippleDetector2Editor(GenericProcessor* parentNode, bool useDefaultParameterEditors=true)
    : GenericEditor(parentNode, useDefaultParameterEditors), previousChannelCount(-1)

{

    desiredWidth = 400;

    backgroundColours.add(Colours::green);
    backgroundColours.add(Colours::red);
    backgroundColours.add(Colours::orange);
    backgroundColours.add(Colours::magenta);
    backgroundColours.add(Colours::blue);

    addDetector();                                                                                            //Cria ondinha e caixinhas

    lastTimeString = " ";
    lastAmplitudeString = " ";
    
    AmplitudeLabel = new Label("Amplitude label", "Amplitude (sd):");
    AmplitudeLabel->setBounds(10,75,80,20);
    AmplitudeLabel->setFont(Font("Small Text", 10, Font::plain));
    AmplitudeLabel->setColour(Label::textColourId, Colours::darkgrey);
    addAndMakeVisible(AmplitudeLabel);

    TimeLabel = new Label("Time label", "Time (ms):");
    TimeLabel->setBounds(10,35,80,20);
    TimeLabel->setFont(Font("Small Text", 10, Font::plain));
    TimeLabel->setColour(Label::textColourId, Colours::darkgrey);
    addAndMakeVisible(TimeLabel);

    TimeValue = new Label("Time value", lastTimeString);
    TimeValue->setBounds(15,52,60,18);
    TimeValue->setFont(Font("Default", 15, Font::plain));
    TimeValue->setColour(Label::textColourId, Colours::white);
    TimeValue->setColour(Label::backgroundColourId, Colours::grey);
    TimeValue->setEditable(true);
    TimeValue->addListener(this);
    TimeValue->setTooltip("Set the time for the selected channels");
    addAndMakeVisible(TimeValue);

    AmplitudeValue = new Label("Amplitude label", lastAmplitudeString);
    AmplitudeValue->setBounds(15,92,60,18);
    AmplitudeValue->setFont(Font("Default", 15, Font::plain));
    AmplitudeValue->setColour(Label::textColourId, Colours::white);
    AmplitudeValue->setColour(Label::backgroundColourId, Colours::grey);
    AmplitudeValue->setEditable(true);
    AmplitudeValue->addListener(this);
    AmplitudeValue->setTooltip("Set the amplitude for the selected channels");
    addAndMakeVisible(AmplitudeValue);

}

RippleDetector2Editor::~RippleDetector2Editor()
{

}

void RippleDetector2Editor::updateSettings()
{

    if (getProcessor()->getNumInputs() != previousChannelCount)
    {

        for (int i = 0; i < interfaces.size(); i++)
        {
            interfaces[i]->updateChannels(getProcessor()->getNumInputs());
        }
    }

    previousChannelCount = getProcessor()->getNumInputs();

}

void RippleDetector2Editor::comboBoxChanged(ComboBox* c)
{

    for (int i = 0; i < interfaces.size(); i++)
    {

        if (i == c->getSelectedId()-1)
        {
            interfaces[i]->setVisible(true);
        }
        else
        {
            interfaces[i]->setVisible(false);
        }

    }

}

void RippleDetector2Editor::addDetector()
{

    RippleDetector2* sd = (RippleDetector2*) getProcessor();

    int detectorNumber = interfaces.size()+1;

    RippleInterface* di = new RippleInterface(sd, backgroundColours[detectorNumber%5], detectorNumber-1);
    di->setBounds(90,40,190,80);

    addAndMakeVisible(di);

    interfaces.add(di);

}

void RippleDetector2Editor::saveCustomParameters(XmlElement* xml)
{

    xml->setAttribute("Type", "RippleDetector2Editor");

    for (int i = 0; i < interfaces.size(); i++)
    {
        XmlElement* d = xml->createNewChildElement("DETECTOR");
        d->setAttribute("INPUT",interfaces[i]->getInputChan());
        d->setAttribute("GATE",interfaces[i]->getGateChan());
        d->setAttribute("OUTPUT",interfaces[i]->getOutputChan());
    }
    
    lastAmplitudeString = AmplitudeValue->getText();
    lastTimeString = TimeValue->getText();

    XmlElement* textLabelValues = xml->createNewChildElement("VALUES");
    textLabelValues->setAttribute("Amplitude",lastAmplitudeString);
    textLabelValues->setAttribute("Time",lastTimeString);
}

void RippleDetector2Editor::loadCustomParameters(XmlElement* xml)
{

    int i = 0;

    forEachXmlChildElement(*xml, xmlNode)
    {
        if (xmlNode->hasTagName("DETECTOR"))
        {

            if (i > 0)
            {
                addDetector();
            }
            
            interfaces[i]->setInputChan(xmlNode->getIntAttribute("INPUT"));
            interfaces[i]->setGateChan(xmlNode->getIntAttribute("GATE"));
            interfaces[i]->setOutputChan(xmlNode->getIntAttribute("OUTPUT"));

            i++;
        }

        else if (xmlNode->hasTagName("VALUES"))
        {
            AmplitudeValue->setText(xmlNode->getStringAttribute("Amplitude"),dontSendNotification);
            TimeValue->setText(xmlNode->getStringAttribute("Time"),dontSendNotification);
        }
    }
}

// ===================================================================

RippleInterface::RippleInterface(RippleDetector2* sd, Colour c, int id) :
    backgroundColour(c), idNum(id), processor(sd)
{

    font = Font("Small Text", 10, Font::plain);

    const double PI = 3.14159265;

    sineWave.startNewSubPath(5,35);

    cout << "Creating sine wave" << endl;

    for (double i = 0; i < 2*PI; i += PI/80)
    {
        
        if (i > 1.5*PI/3 || i < 1.7*PI/3)
        {
            sineWave.lineTo(i*12+5.0f, -sin(i*9)*sin(i*0.5)*30 + 35);
        }
        else
        {
            sineWave.lineTo(i*12 + 5.0f, -sin(i*9)*sin(i*0.5)*10 + 35);
        }
    }

    cout << "Creating combo boxes" << endl;


    inputSelector = new ComboBox();
    inputSelector->setBounds(140,5,50,20);
    inputSelector->addItem("-",1);
    inputSelector->setSelectedId(1);
    inputSelector->addListener(this);
    addAndMakeVisible(inputSelector);

    gateSelector = new ComboBox();
    gateSelector->setBounds(140,30,50,20);
    gateSelector->addItem("-",1);
    gateSelector->addListener(this);

    cout << "Populating combo boxes" << endl;


    for (int i = 1; i < 9; i++)
    {
        gateSelector->addItem(String(i),i+1);
    }

    gateSelector->setSelectedId(1);
    addAndMakeVisible(gateSelector);

    outputSelector = new ComboBox();
    outputSelector->setBounds(140,55,50,20);
    outputSelector->addItem("-",1);
    outputSelector->addListener(this);

    for (int i = 1; i < 9; i++)
    {
        outputSelector->addItem(String(i),i+1);
    }
    outputSelector->setSelectedId(1);
    addAndMakeVisible(outputSelector);


    cout << "Updating channels" << endl;

    updateChannels(processor->getNumInputs());

    cout << "Updating processor" << endl;


    processor->addModule();

}

RippleInterface::~RippleInterface()
{

}

void RippleInterface::comboBoxChanged(ComboBox* c)
{

    processor->setActiveModule(idNum);

    int parameterIndex;

    if (c == inputSelector)
    {
        parameterIndex = 2;
    }
    else if (c == outputSelector)
    {
        parameterIndex = 3;
    }
    else if (c == gateSelector)
    {
        parameterIndex = 4;
    }


    processor->setParameter(parameterIndex, (float) c->getSelectedId() - 2);

}

void RippleInterface::updateChannels(int numChannels)
{

    inputSelector->clear();

    inputSelector->addItem("-", 1);

    if (numChannels > 2048) // channel count hasn't been updated yet
    {
        return;
    }

    for (int i = 0; i < numChannels; i++)
    {
        inputSelector->addItem(String(i+1), i+2);

    }

    inputSelector->setSelectedId(1, dontSendNotification);
}

void RippleInterface::setInputChan(int chan)
{
    inputSelector->setSelectedId(chan+2);

    processor->setParameter(2, (float) chan);
}

void RippleInterface::setOutputChan(int chan)
{
    outputSelector->setSelectedId(chan+2);

    processor->setParameter(3, (float) chan);
}

void RippleInterface::setGateChan(int chan)
{
    gateSelector->setSelectedId(chan+2);

    processor->setParameter(4, (float) chan);
}

int RippleInterface::getInputChan()
{
    return inputSelector->getSelectedId()-2;
}

int RippleInterface::getOutputChan()
{
    return outputSelector->getSelectedId()-2;
}

int RippleInterface::getGateChan()
{
    return gateSelector->getSelectedId()-2;
} 

void RippleDetector2Editor::setDefaults(double Time, double Amplitude)
{

    lastAmplitudeString = String(roundFloatToInt(Amplitude));
    lastTimeString = String(roundFloatToInt(Time));

    AmplitudeValue->setText(lastAmplitudeString, dontSendNotification);
    TimeValue->setText(lastTimeString, dontSendNotification);
}

void RippleDetector2Editor::labelTextChanged(Label* label)
{

    RippleDetector2* sd = (RippleDetector2*) getProcessor();

    Value val = label->getTextValue();
    double requestedValue = double(val.getValue());

    if (requestedValue < 0.01 || requestedValue > 10000)
    {
        CoreServices::sendStatusMessage("Value out of range.");

        if (label == AmplitudeValue)
        {
            label->setText(lastAmplitudeString, dontSendNotification);
            lastAmplitudeString = label->getText();
        }
        else
        {
            label->setText(lastTimeString, dontSendNotification);
            lastTimeString = label->getText();
        }

        return;
    }

    Array<int> chans = getActiveChannels();


    for (int n = 0; n < chans.size(); n++)
    {

        if (label == AmplitudeValue)
        {

                sd->setCurrentChannel(chans[n]);

                sd->setParameter(1, requestedValue);


            lastAmplitudeString = label->getText();

        }
        else
        {

                sd->setCurrentChannel(chans[n]);
                sd->setParameter(0, requestedValue);

            lastTimeString = label->getText();

        }

    }

}
