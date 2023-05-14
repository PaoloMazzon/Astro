---
layout: default
title: AudioData
parent: API Documentation
---

## AudioData
`AudioData` is the actual audio data needed to play audio, see [Audio](Audio) for
information on playing audio.

 + [open](#open)
 + [free](#free)

### open
`construct open(filename)`

Parameters
 + `filename -> String` Filename of the audio to load.
 
Loads a piece of audio, only `.wav` and `.ogg` files are supported.

### free
`foreign free()`

Forces the audio to be freed. This must not be used while this audio is playing.

