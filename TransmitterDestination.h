﻿#pragma once
#include "Transmitter.h"

class TransmitterDestination : public Transmitter 
{
public:
    std::string GetName() override { return "Destination"; }

    void OnMessageReceive(Transmitter* sender, Byte& byte, TransmissionLog* log) override;
};
