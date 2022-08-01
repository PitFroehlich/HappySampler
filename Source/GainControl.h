/*
  ==============================================================================

    GainControl.h
    Created: 1 Aug 2022 5:45:46pm
    Author:  pitfr

  ==============================================================================
*/

#pragma once

class JUCE_API GainControl {
public:
    GainControl() {}

    struct JUCE_API  Parameters
    {
        Parameters() = default;

        Parameters(
            float gain1,
            float gain2,
            float gain3)
            : gainValue1(gain1),
            gainValue2(gain2),
            gainValue3(gain3)
        {
        }
        float gainValue1 = 1.0f, gainValue2 = 1.0f, gainValue3 = 1.0f;
    };

    void setParameters(const Parameters& newParameters)
    {
        parameters = newParameters;
    }

    const Parameters& getParameters() const noexcept { return parameters; }


private:
    Parameters parameters;
};


