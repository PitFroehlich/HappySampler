#include "MultiVoiceSynth.h"

void MultiVoiceSynth::noteOn(
    const int midiChannel,
    const int midiNoteNumber,
    const float velocity)
{
    const juce::ScopedLock sl(lock);

    for (auto* sound : sounds)
    {
        if (sound->appliesToNote(midiNoteNumber) && sound->appliesToChannel(midiChannel))
        {
            for (auto* voice : voices) {
                if ((voice->getCurrentlyPlayingNote() == midiNoteNumber) &&
                    voice->isPlayingChannel(midiChannel) &&
                    (voice->getCurrentlyPlayingSound() == sound)
                    ) {
                    stopVoice(voice, 1.0f, true);
                }
            }

            startVoice(findFreeVoice(sound, midiChannel, midiNoteNumber, isNoteStealingEnabled()),
                sound, midiChannel, midiNoteNumber, velocity);
        }
    }
}