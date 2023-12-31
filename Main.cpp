#include <iostream>
#include <thread>

#include "EByteMethod.h"
#include "Headers/NoisyChannel.h"
#include "Headers/ThousandSeparatorLocale.h"
#include "Headers/Transmitter/TransmitterSource.h"
#include "Headers/Transmitter/TransmitterDestination.h"
#include "Headers/Message/ChecksumByte.h"
#include "Headers/Message/HammingByte.h"
#include "Headers/Message/TripledByte.h"

// Forwward declarations
int GetIterationCount();
double GetErrorRatio();
EByteMethod GetByteMethod();

int main()
{
    // ThousandSeparatorLocale::Setup();   // Setup the program to output numbers like 10000 as 10,000
    cout << "Welcome to the Discrete Gang's Error Correction Schema." << endl;

    NoisyChannel::NoisePercentage = GetErrorRatio();            // Get how often to flip bits
    auto destination = make_shared<TransmitterDestination>();   // Create destination

    auto byteMethod = GetByteMethod();
    auto iterationCount = GetIterationCount();

    // Use hamming code if use selects it
    if (byteMethod == EByteMethod::Hamming)
    {
        auto source = make_shared<TransmitterSource<HammingByte>>(destination, iterationCount);
        std::this_thread::sleep_for(std::chrono::seconds(1000)); // Wait for program
    }
    else if (byteMethod == EByteMethod::Checksum)
    {
        auto source = make_shared<TransmitterSource<ChecksumByte>>(destination, iterationCount);
        std::this_thread::sleep_for(std::chrono::seconds(1000)); // Wait for program
    }
    else
    {
        auto source = make_shared<TransmitterSource<TripledByte>>(destination, iterationCount);
        std::this_thread::sleep_for(std::chrono::seconds(1000)); // Wait for program
    }

    return 0;
}

// Get how many times to loop
int GetIterationCount()
{
    system("cls");
    
    const int MIN_ITERATIONS = 100, MAX_ITERATIONS = 1000000;
    cout << "Enter how many iterations you want. The number must be between " << MIN_ITERATIONS << " and " << MAX_ITERATIONS << ": ";

    int iterationCount;
    cin >> iterationCount;

    // Validate interation count
    while (iterationCount < MIN_ITERATIONS || iterationCount > MAX_ITERATIONS)
    {
        cout << "Invalid input. Input must be between " << MIN_ITERATIONS << " and " << MAX_ITERATIONS << ". Try again: ";
        cin.clear();
        cin.ignore(1000, '\n');
        cin >> iterationCount;
    }

    Evaluator::IterationCount = iterationCount;

    return iterationCount;
}

// Gets how often to flip bits
double GetErrorRatio()
{
    cout << endl << "Enter how often a bit should be flipped. As a percentage, " <<
                    "the value should be between 0% (no bits will be flipped) and 99% (a bit will be flipped almost every transmission): ";
    double percentage = 0;
    cin >> percentage;

    // Validate percentage
    while (percentage < 0 || percentage > 99)
    {
        cout << "Invalid input. Input must be between 0 and 99. Try again: ";
        cin >> percentage;
    }

    Evaluator::Percentage = percentage;

    cout << endl;
    cout << "You've chosen " << percentage << "%, which means: " << endl;
    cout << percentage << "% of transmissions will have at least one bit flip" << endl;
    cout << percentage * percentage / 100 << "% of transmissions will have at least two bit flips" << endl;
    cout << pow(percentage, 3) / 10000 << "% of transmissions will have at least three bit flips." << endl;
    cout << pow(percentage, 4) / 1000000 << "% of transmissions will have at least four bit flips, and so on." << endl << endl;

    // Wait for user input
    cout << "Press any key to continue... ";

    char buffer;
    cin >> buffer;

    return percentage;
}

// Returns true if the user selects to use hamming byte
EByteMethod GetByteMethod()
{
    system("cls");

    cout << "There are two different algorithms you can use: " << endl;
    cout << "1. Ack/Checksum method: Adds 2 bits of redundancy. It is able to detect a multiple bit errors but not fix them." << endl;
    cout << "2. Hamming Code: Adds 5 redundant parity bits, giving enough redundancy to fix single bit errors, and detect 2 or more bit errors." << endl;
    cout << "3. Tripled Bits: Triples up all the bits, using by far the most redundancy bits, but no longer needs to retransmit." << endl;
    cout << "Take your pick: ";

    int input;
    cin >> input;

    // Validate input
    while (input < 1 || input > 3)
    {
        cout << "Invalid input. Input must be 1, 2 or 3. Try again: ";
        cin.clear();
        cin.ignore(1000, '\n');
        cin >> input;
    }

    auto byteMethod = (EByteMethod)input;
    Evaluator::ByteMethod = input == 3 ? "Tripled Bits" : input == 2 ? "Hamming" : "Checksum";
    return byteMethod;
}
