## Audio
Class for playing and managing playing audio. Audio must be loaded with [AudioData](AudioData.md)
before its able to be played with this class.

 + [play](#play)
 + [update](#update)
 + [stop](#stop)
 + [stop_all](#stop_all)

### play
`construct play(audio_data, loop, left_volume, right_volume)`

Parameters
 + `audio_data -> lib/Audio::AudioData` Audio to play.
 + `loop -> Bool` Whether or not to repeat the audio indefinitely.
 + `left_volume -> Num` Volume for the left ear from 0-1.
 + `right_volume -> Num` Volume for the right ear from 0-1.

Plays a sound returning a handle for the playing audio. The handle is no longer
valid after the audio stops playing, meaning you generally shouldn't update `Audio`
instances that don't repeat.

> 📝 If you are just playing a sound once or just don't care, you don't need
> to store the `Audio` instance.

### update
`update(loop, left_volume, right_volume)`

Parameters
 + `loop -> Bool` Whether or not to repeat the audio indefinitely.
 + `left_volume -> Num` Volume for the left ear from 0-1.
 + `right_volume -> Num` Volume for the right ear from 0-1.

Updates an `Audio`.

### stop
`stop()`

Stops a sound from continuing to play, the handle is no longer valid after calling this.

### stop_all
`static stop_all()`

Stops all currently playing sounds, making all existing Audio handles invalid.

-----------

[Getting Started](../GettingStarted.md) | [API Reference](../API.md)