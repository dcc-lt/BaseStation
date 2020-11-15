// =============================================================================
// JTV SOFTWARES @ 2020
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see http://www.gnu.org/licenses
// =============================================================================

#pragma once

// =============================================================================
// I N C L U D E S   &   F O R W A R D   D E C L A R A T I O N S

#include "comm/CommInterface.h"

//=============================================================================
// C O N S T A N T S


// =============================================================================
// C L A S S E S

namespace dcclt {
namespace comm {

class SerialComm : public CommInterface
{
public:

    SerialComm(HardwareSerial comm, unsigned long rate, uint8_t config = SERIAL_8N1);
    virtual ~SerialComm();

    void begin() override;
    void flush() override;

    virtual bool available() override;

    virtual byte read() override;
    virtual void write(const byte& data) override;
    
private:

    HardwareSerial _comm;
    unsigned long _rate;
    uint8_t _config;
    

};

}}
