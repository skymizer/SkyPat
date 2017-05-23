//===- my_case.h ----------------------------------------------------------===//
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
#ifndef SKYPAT_SAMPLE2_MY_CASE_H
#define SKYPAT_SAMPLE2_MY_CASE_H

class Turret;

// A factory which can create turret.
class TurretFactory
{
public:
    TurretFactory() {}
    ~TurretFactory(){}
    static Turret * CreateTurret();
    static void DestoryTurret(Turret *pTurret);
};


// A Turret class which can be turn on/off
class Turret
{
public:
    Turret()  {}
    ~Turret() {}
    bool isOn()   { return m_status; }
    void turnOn() { m_status=true;   }
    void turnOff(){ m_status=false;  }
    // Fire if On and return true, else return false. 
    bool fire();

private:
    void sleepMs(int sleepMs);

private:
    bool m_status;
};

#endif
