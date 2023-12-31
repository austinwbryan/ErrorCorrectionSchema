﻿#include "../../Headers/Message/ChecksumByte.h"
#include "../../Headers/TransmissionLog.h"

// Constructor
ChecksumByte::ChecksumByte(int number) : Byte{ number }
{
    ChecksumByte::ComputeRedundancyBits();
}

int ChecksumByte::ToInt() const
{
    int sum = 0, pow2 = 0;

    // i starts at 7, and ends at 0. Indices 7-0 are the payload indices of the byte. 
    for (int i = 7; i > -1; i--)
        // Sum is totaled by mutliplying the bit value by the power of 2.
        // pow2++ increments the pow2 for next time, we use the value before the increment for the function call
        sum += static_cast<int>(bits[i] * pow(2, pow2++));    
    return sum;
}
int ChecksumByte::GetAck()      const { return bits[ACK_INDEX]; }       // Return the ack
int ChecksumByte::GetCheckSum() const { return bits[CHECKSUM_INDEX]; }  // Return the checksum

// Flips the ack bti and checksum to maintain the correct parity
void ChecksumByte::Acknowledge()
{
    FlipBit(bits[ACK_INDEX]);
    FlipBit(bits[CHECKSUM_INDEX]);
}
bool ChecksumByte::IsByteValid() { return GetCheckSum() == CalculateCheckSum(); } // If GetChecksum() matches the calculation, then the checksum is valid.

bool ChecksumByte::ShouldRetransmit(TransmissionLog& log)
{
    bool isValid = IsByteValid();

    if (isValid)                           // This step boosts accuracy by 30%, increasing from an average of 60% correct to 90% correct
        log.Verify(shared_from_this());    // Make a note of this verification in the log
    else Acknowledge();                    // Mark the byte as acknowledged before returning it 

    return !isValid;
}
void ChecksumByte::ComputeRedundancyBits()
{
    bits.push_back(0);                      // Add the ack bit, which always starts off as 0
    bits.push_back(CalculateCheckSum());    // Add the checksum bit, after first calculating it
}
bool ChecksumByte::operator==(const Byte& other) const
{
    // Ignoring checksum and ackbit increasing accuracy by 0.2%
    for (int i = 0; i < 8; i++)
        if (bits[i] != other.GetBits()[i])
            return false;
    return true;
}
int ChecksumByte::CalculateCheckSum() const
{
    int sum = 0;

    for (int i = 0; i < CHECKSUM_INDEX; i++)    // Add up all bits except the checksum, including the ack
        sum += bits[i];
    return sum % 2;                             // Mod by 2 to turn the result into 0 if sum is even and 1 if sum is odd
}
