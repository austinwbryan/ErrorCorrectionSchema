﻿#include "TransmitterSource.h"
#include "Transmitter.h"
#include <iostream>
#include <random>

#include "Evaluator.h"
#include "Byte.h"
#include "Message.h"
using namespace std;

TransmitterSource::TransmitterSource(const shared_ptr<Transmitter>& _destination) { this->destination = _destination; }
void TransmitterSource::OnMessageReceive(const shared_ptr<Transmitter>& sender, Byte& byte, TransmissionLog& log)
{
    Transmitter::OnMessageReceive(sender, byte, log);
    
    message = make_unique<Message>(sender, byte, log);
    shouldResend = true;
}
void TransmitterSource::SendTo(const shared_ptr<Transmitter>& receiver, Byte& byte, TransmissionLog& log)
{
    if (byte.ValidateCheckSum())
        attemptedMessage = byte;
    Transmitter::SendTo(receiver, byte, log);
}
void TransmitterSource::ThreadMain()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribution(0, 255);
    
    for (int i = 0; i < 1000; i++)
    {
        int number = distribution(gen);
        Byte byte = number;

        auto log = TransmissionLog(byte);

        cout << "\nNumber: " << byte << endl;
        SendTo(destination, byte, log);

        std::this_thread::sleep_for(std::chrono::microseconds(1));

        while (shouldResend)
        {
            shouldResend = false;
            SendTo(message->receiver, attemptedMessage, message->log);

            std::this_thread::sleep_for(std::chrono::microseconds(1));

            if (message->byte.GetAck() == 0 &&!message->byte.ValidateCheckSum())
                log.MarkAckFlipped();
        }

        cout << log;
        Evaluator::AddLog(log);
    }

    cout << endl;
    Evaluator::Evaluate();
}