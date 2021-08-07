/*

   This file is part of the Schrammel OJD audio plugin.
   Copyright (C) 2020  Janos Buttgereit

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

*/

#include "PluginProcessor.h"
#include "ProcessorEditor.h"

juce::Rectangle<int> scaledKeepingCentre (const juce::Rectangle<int>& src, float scale)
{
    auto centre = src.getCentre().toFloat();
    auto scaled = src.toFloat() * scale;

    scaled.setCentre (centre);

    return scaled.toNearestInt();
}


//==============================================================================
ProcessorEditor::ProcessorEditor (VelocityZoneAudioProcessor& proc)
    :  jb::PluginEditorBase<contentMinWidth, overallMinHeight> (proc, IsResizable::Yes, UseConstrainer::Yes),
    //background       (BinaryData::background_svg, BinaryData::background_svgSize),
    //pedal            (proc, *this),
    isInMessageState (false),
    messageOkButton ("OK"),
    messageLearnMoreButton ("Learn more")
       //knobDrawable     (juce::Drawable::createFromImageData (juce::BinaryData::knob_svg, BinaryData::knob_svgSize)),
       //ojdLookAndFeel   (*knobDrawable)
{
    //setLookAndFeel (&ojdLookAndFeel);

    //addAndMakeVisible (background);
    //addAndMakeVisible (pedal);
    //addAndMakeVisible (bypassSwitch);


    addPresetManager (proc);
    setupAndAddMessageOfTheDayComponents();

    restoreSizeFromState();

    //checkMessageOfTheDay (proc);
}

ProcessorEditor::~ProcessorEditor()
{
    setLookAndFeel (nullptr);
}

void ProcessorEditor::constrainedResized()
{
    presetManagerComponent->setBounds (getLocalBounds().removeFromTop (presetManagerComponentHeight));

    auto contentBounds = getLocalBounds().withTrimmedTop (presetManagerComponentHeight);
    //background.setBounds (contentBounds);

    auto pedalBounds = scaledKeepingCentre (contentBounds, 0.85f);

    if (isInMessageState)
    {
        pedalBounds.translate (proportionOfWidth (0.8f), 0);

        auto messageBounds = contentBounds.reduced (proportionOfHeight (0.05f));

        messageEditor.applyFontToAllText(getHeight() * 0.025f);
        messageEditor.setBounds (messageBounds.removeFromTop (proportionOfHeight (0.8f)));

        messageBounds.removeFromTop (proportionOfHeight (0.01f));

        auto okBounds = messageBounds.removeFromLeft (messageBounds.getWidth() / 2);
        auto learnMoreBounds = messageBounds;
        messageOkButton.setBounds (okBounds.reduced (proportionOfHeight (0.01f)));
        messageLearnMoreButton.setBounds (learnMoreBounds.reduced (proportionOfHeight (0.01f)));
    }

    //pedal.setBounds (pedalBounds);
}

void ProcessorEditor::checkBounds (juce::Rectangle<int>& bounds, const juce::Rectangle<int>&, const juce::Rectangle<int>&, bool, bool, bool, bool)
{
    auto contentHeight = juce::jmax (contentMinHeight, bounds.getHeight() - presetManagerComponentHeight);
    auto contentWidth  = juce::jmax (contentMinWidth, juce::roundToInt (contentHeight * contentAspectRatio));

    bounds.setSize (contentWidth, contentHeight + presetManagerComponentHeight);
}

void ProcessorEditor::paint(juce::Graphics &g)
{
    g.fillAll (juce::Colours::white);
}

void ProcessorEditor::addPresetManager (VelocityZoneAudioProcessor& processorToControl)
{
    presetManagerComponent = processorToControl.stateAndPresetManager.createPresetManagerComponent (*this, true);
    addAndMakeVisible (*presetManagerComponent);
}

void ProcessorEditor::setupAndAddMessageOfTheDayComponents()
{
    addChildComponent(messageEditor);
    addChildComponent (messageOkButton);
    addChildComponent (messageLearnMoreButton);

    messageEditor.setReadOnly (true);
    messageEditor.setColour (juce::TextEditor::ColourIds::backgroundColourId, juce::Colours::transparentBlack);
    messageEditor.setColour (juce::TextEditor::ColourIds::outlineColourId, juce::Colours::transparentBlack);
    messageEditor.setColour (juce::TextEditor::ColourIds::textColourId, juce::Colours::white);

    messageEditor.setJustification (juce::Justification::centred);
    messageEditor.setMultiLine (true);

    messageOkButton.setColour (juce::TextButton::ColourIds::buttonColourId, juce::Colours::transparentBlack);
    messageLearnMoreButton.setColour (juce::TextButton::ColourIds::buttonColourId, juce::Colours::transparentBlack);

    messageOkButton.onClick = [this]()
    {
        isInMessageState = false;

        messageEditor.setVisible (false);
        messageOkButton.setVisible (false);
        messageLearnMoreButton.setVisible (false);

        constrainedResized();
        repaint();
    };
}

void ProcessorEditor::setMessage (const juce::String& text, const juce::URL url)
{
    messageEditor.setText (text);

    messageLearnMoreButton.onClick = [url] ()
    {
        url.launchInDefaultBrowser();
    };

    messageEditor.setVisible (true);
    messageOkButton.setVisible (true);
    messageLearnMoreButton.setVisible (true);

    isInMessageState = true;

    constrainedResized();
    repaint();
}
