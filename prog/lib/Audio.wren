// Audio.wren
// Author: Paolo Mazzon
// Simple audio control

// Actual audio data, does not do anything itself
foreign class AudioData {
    // Loads audio from a .wav file
    construct open(filename) {}

    // Frees audio manually
    foreign free()
}

// This is what you use to play audio
foreign class Audio {
    // Plays audio from an audio data and returns this class
    construct play(audio_data, loop, left_volume, right_volume) {}

    // Updates a playing audio, volume should be a float from 0-1
    foreign update(loop, left_volume, right_volume)

    // Stops a playing audio
    foreign stop()

    // Stops all currently playing audio
    foreign static stop_all()
}