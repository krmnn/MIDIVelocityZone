# VelocityZone MIDI Plugin (JUCE)

Filters incoming MIDI for a specific note and triggers a different one if the note velocity is above a configurable threshold.

![screenshot](screenshot.png)

Motivation: The cheap Roland TD1 KV drumsets fake ride bell hits when hitting the ride tip with a certain velocity. This information is not passed via MIDI OUT and this plugin will re-create the behaviour before the messages enter your drum VST. But you can use it in general to add accents to your MIDI pads.

Default Settings:
```
Midi In: MIDI Note 51 / D#2 / Roland TD Ride Bow Tip
Midi Out: MIDI Note 53 / F2 / Roland TD Ride Bell
Midi In Threshold: 70
```

Tested on MacOS with Bitwig and AudioPluginHost.

