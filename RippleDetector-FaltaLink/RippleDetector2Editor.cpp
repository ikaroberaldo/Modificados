//****************************************************> significa que veio do Bandpass

#include "RippleDetector2Editor.h"
#include "RippleDetector2.h"

#include <stdio.h>
#include <cmath>

using namespace std;

RippleDetector2Editor::RippleDetector2Editor(GenericProcessor* parentNode, bool useDefaultParameterEditors=true)
    : GenericEditor(parentNode, useDefaultParameterEditors), previousChannelCount(-1)

{
    desiredWidth = 300;
/*
    detectorSelector = new ComboBox();
    detectorSelector->setBounds(35,30,150,20);
    detectorSelector->addListener(this);

    addAndMakeVisible(detectorSelector);                                                                      //Cria caixa seletora superior
*/
    plusButton = new UtilityButton("+", titleFont);
    plusButton->addListener(this);
    plusButton->setRadius(3.0f);
//    plusButton->setBounds(90,30,20,20);

    addAndMakeVisible(plusButton);   

    backgroundColours.add(Colours::green);
    backgroundColours.add(Colours::red);
    backgroundColours.add(Colours::orange);
    backgroundColours.add(Colours::magenta);
    backgroundColours.add(Colours::blue);

    plusButton->setToggleState(true, sendNotification);

//****************************************************    
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
//****************************************************
}
                                                                                                              //*Não retirar só tudo q tem a ver com o botaozinho de +

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
                                                                                                              //...
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
                                                                                                              //...
void RippleDetector2Editor::buttonEvent(Button* button)
{
    //if (button == plusButton && interfaces.size() < 5)
    //{

        addDetector();

    //}

}                                                                                                             //*Certamente não retirar essa função
                                                                                                              //...
void RippleDetector2Editor::addDetector()
{
//    cout << "Adding detector" << endl;

    RippleDetector2* sd = (RippleDetector2*) getProcessor();

    int detectorNumber = interfaces.size()+1;

    RippleInterface* di = new RippleInterface(sd, backgroundColours[detectorNumber%5], detectorNumber-1);
    di->setBounds(90,50,190,80);   //não tirar sozinho (deixa a interface só com o +)

    addAndMakeVisible(di); //não tirar sozinho (deixa a interface só com o +)

    interfaces.add(di); //não tirar, é isso q detecta o número de inputs

//    String itemName = "Detector ";
//    itemName += detectorNumber;

//    detectorSelector->addItem(itemName, detectorNumber);
//    detectorSelector->setSelectedId(detectorNumber, dontSendNotification);
/*
    for (int i = 0; i < interfaces.size()-1; i++)
    {
        interfaces[i]->setVisible(false);
    }
*/
}
                                                                                                              //Cria onda sen e seu espaço
void RippleDetector2Editor::saveCustomParameters(XmlElement* xml)
{

    xml->setAttribute("Type", "RippleDetector2Editor");

    for (int i = 0; i < interfaces.size(); i++)
    {
        XmlElement* d = xml->createNewChildElement("DETECTOR");
        d->setAttribute("PHASE",interfaces[i]->getPhase());
        d->setAttribute("INPUT",interfaces[i]->getInputChan());
        d->setAttribute("GATE",interfaces[i]->getGateChan());
        d->setAttribute("OUTPUT",interfaces[i]->getOutputChan());
    }
    
//****************************************************
    lastHighCutString = highCutValue->getText();
    lastLowCutString = lowCutValue->getText();

    XmlElement* textLabelValues = xml->createNewChildElement("VALUES");
    textLabelValues->setAttribute("HighCut",lastHighCutString);
    textLabelValues->setAttribute("LowCut",lastLowCutString);
//****************************************************
}
                                                                                                              //Salva dados
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
            
            interfaces[i]->setPhase(xmlNode->getIntAttribute("PHASE"));
            interfaces[i]->setInputChan(xmlNode->getIntAttribute("INPUT"));
            interfaces[i]->setGateChan(xmlNode->getIntAttribute("GATE"));
            interfaces[i]->setOutputChan(xmlNode->getIntAttribute("OUTPUT"));

            i++;
        }
//****************************************************
        else if (xmlNode->hasTagName("VALUES"))
        {
            highCutValue->setText(xmlNode->getStringAttribute("HighCut"),dontSendNotification);
            lowCutValue->setText(xmlNode->getStringAttribute("LowCut"),dontSendNotification);
        }
//****************************************************            
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
                                                                                                              //Dá forma à ondazinha
    cout << "Creating phase buttons" << endl;

    cout << "Creating combo boxes" << endl;


    inputSelector = new ComboBox();
    inputSelector->setBounds(140,5,50,20);
    inputSelector->addItem("-",1);
    inputSelector->setSelectedId(1);
    inputSelector->addListener(this);
    addAndMakeVisible(inputSelector);
                                                                                                              //Mexe a caixinha do Input
    gateSelector = new ComboBox();
    gateSelector->setBounds(140,30,50,20);
    gateSelector->addItem("-",1);
    gateSelector->addListener(this);
                                                                                                              //Mexe a caixinha do Gate
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
                                                                                                              //Mexe a caixinha do Output
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
                                                                                                              //Muda a parte gráfica inferior
RippleInterface::~RippleInterface()
{

}
                                                                                                              //Destructor
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
                                                                                                              //...
void RippleInterface::buttonClicked(Button* b)
{

    ElectrodeButton* pb = (ElectrodeButton*) b;

    int i = phaseButtons.indexOf(pb);

    processor->setActiveModule(idNum);

    processor->setParameter(1, (float) i+1);

}                                                                                                             //Parece não mudar nada (grafico)
                                                                                                              //...
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
        inputSelector->addItem(String(i+1), i++);

    }

    inputSelector->setSelectedId(1, dontSendNotification);
}
                                                                                                              //...
void RippleInterface::paint(Graphics& g)
{
    g.setColour(Colours::blue);
    g.strokePath(sineWave, PathStrokeType(2.0f));

    g.setColour(Colours::darkgrey);
    g.setFont(font);
    g.drawText("INPUT",50,10,85,10,Justification::right, true);
    g.drawText("GATE",50,35,85,10,Justification::right, true);
    g.drawText("OUTPUT",50,60,85,10,Justification::right, true);

}
                                                                                                              //...
int RippleInterface::getPhase()
{

    for (int i = 0; i < phaseButtons.size(); i++)
    {
        if (phaseButtons[i]->getToggleState())
        {
            return i;
        }
    }

    return -1;
}
                                                                                                              //...
void RippleInterface::setPhase(int p)
{

    if (p >= 0)
        phaseButtons[p]->setToggleState(true, dontSendNotification);
    
    processor->setActiveModule(idNum);

    processor->setParameter(1, (float) p+1);

}
                                                                                                              //...
void RippleInterface::setInputChan(int chan)
{
    inputSelector->setSelectedId(chan+2);

    processor->setParameter(2, (float) chan);
}
                                                                                                              //...
void RippleInterface::setOutputChan(int chan)
{
    outputSelector->setSelectedId(chan+2);

    processor->setParameter(3, (float) chan);
}
                                                                                                              //...
void RippleInterface::setGateChan(int chan)
{
    gateSelector->setSelectedId(chan+2);

    processor->setParameter(4, (float) chan);
}
                                                                                                              //...
int RippleInterface::getInputChan()
{
    return inputSelector->getSelectedId()-2;
}
                                                                                                              //...
int RippleInterface::getOutputChan()
{
    return outputSelector->getSelectedId()-2;
}
                                                                                                              //...
int RippleInterface::getGateChan()
{
    return gateSelector->getSelectedId()-2;
} 

//****************************************************
void RippleDetector2Editor::setDefaults(double lowCut, double highCut)
{
    lastHighCutString = String(roundFloatToInt(highCut));
    lastLowCutString = String(roundFloatToInt(lowCut));

    highCutValue->setText(lastHighCutString, dontSendNotification);
    lowCutValue->setText(lastLowCutString, dontSendNotification);
}

void RippleDetector2Editor::labelTextChanged(Label* label)
{
    RippleDetector2* sd = (RippleDetector2*) getProcessor();

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

                sd->setCurrentChannel(chans[n]);

                sd->setParameter(1, requestedValue);


            lastHighCutString = label->getText();

        }
        else
        {

                sd->setCurrentChannel(chans[n]);
                sd->setParameter(0, requestedValue);

            lastLowCutString = label->getText();

        }

    }

}
//****************************************************
                                                                                                            //...
