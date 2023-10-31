﻿// #include <random>
// #include "../../Headers/Transmitter/TransmitterSource.h"
// #include "../../Headers/Transmitter/Transmitter.h"
// #include "../../Headers/Evaluator.h"
// #include "../../Headers/Message/AcksumByte.h"
// #include "../../Headers/Message/Byte.h"
// #include "../../Headers/Message/HammingByte.h"
// #include "../../Headers/Message/Message.h"
//
// using namespace std;
//
// // template<typename TByte>
// // TransmitterSource<TByte>::TransmitterSource(const shared_ptr<Transmitter>& _destination) { this->destination = _destination; }
// //
// // template<typename TByte>
// // void TransmitterSource<TByte>::OnMessageReceive(Message& message)
// // {
//     // Transmitter::OnMessageReceive(message);
//     // shouldResend = true;
// // }
//
// template<typename TByte>
// void TransmitterSource<TByte>::ThreadMain()
// {
//     std::random_device rd;
//     std::mt19937 gen(rd());
//     std::uniform_int_distribution<> distribution(0, 255);
//     
//     for (int i = 0; i < 1000; i++)
//     {
//         int number = distribution(gen);
//
//         shared_ptr<Byte> byte = make_shared<HammingByte>(number);
//         originalByte = make_shared<HammingByte>(number);
//
//         auto log  = TransmissionLog(originalByte);
//         
//         Message message(destination, byte, log);
//         Send(message);
//
//         std::this_thread::sleep_for(std::chrono::nanoseconds(1));
//         
//         while (shouldResend)
//         {
//             shouldResend = false;
//         
//             Message newMessage(message.receiver, originalByte, message.log);
//             Send(newMessage);
//         
//             std::this_thread::sleep_for(std::chrono::nanoseconds(1));
//         }
//         Evaluator::AddLog(log);
//     }
//     Evaluator::Evaluate();
// }