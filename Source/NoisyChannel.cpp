﻿#include <random>

#include "../Headers/NoisyChannel.h"
#include "../Headers/TransmissionLog.h"

double NoisyChannel::NoisePercentage;

void NoisyChannel::ApplyNoise(const shared_ptr<Byte>& byte, TransmissionLog& log)
{
    // Set up the random device, generator, and distribution of a range 0-3
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(0, 100);

    int noiseOdds = distribution(gen);  // Generate random number 

    // There is a 1/4 chance noise odds = 0. This will continue to fire each time it equals zero
    while (noiseOdds < NoisePercentage)
    {
        int bitIndex = rand() % byte->Size();           // Pick a random number of the index

        byte->ApplyNoise(bitIndex);          // Apply noise to the byte
        log.CountNoise(bitIndex);            // Tells the log file that Noise was applied

        noiseOdds = distribution(gen);   // Generate another random number again.
        // There is a 1/16 chance this will equal 0 twice in a row (two bits flipped) and a 1/64 chance it flips 3 bits, etc.
    }
}
