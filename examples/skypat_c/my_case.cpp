//===- my_case.cpp --------------------------------------------------------===//
//
//                              The SkyPat Team 
//
// This file is distributed under the New BSD License.
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// A sample program demonstrating using SkyPat performance testing framework.
//
// Author: Ying-Chieh Chen <louis@skymizer.com>
//===----------------------------------------------------------------------===//
#include "my_case.h"

#ifdef __linux__
#include <unistd.h>
#endif
#if defined(_WIN32) || defined(_WIN64)
#inlcude <windows.h>
#endif

// Create a Turret
Turret * TurretFactory::CreateTurret()
{
    Turret * t = new Turret();
    t->turnOn();
    return t;
}
// Delete a Turret
void TurretFactory::DestoryTurret(Turret *pTurret)
{
    delete pTurret;
}


// Fire if On and return true, else return false.
// (using sleep to simulate fire time cost.)
bool Turret::fire()
{
    if(m_status) {
       this->sleepMs(5);
    }
    return m_status;
}

// sleep function.
void Turret::sleepMs(int sleepMs)
{
#ifdef __linux__
    usleep(sleepMs * 1000);   // usleep takes sleep time in us
#endif
#if defined(_WIN32) || defined(_WIN64)
    Sleep(sleepMs);
#endif
}
