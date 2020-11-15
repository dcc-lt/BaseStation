
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


//=============================================================================
// I N C L U D E   F I L E S   A N D   F O R W A R D   D E C L A R A T I O N S

#include "comm/SerialComm.h"

//=============================================================================
// C O N S T A N T S   &   L O C A L   V A R I A B L E S


//=============================================================================
// C O N S T R U C T O R ( S ) / D E S T R U C T O R   C O D E   S E C T I O N

//-----------------------------------------------------------------------------
//
dcclt::comm::SerialComm::SerialComm(HardwareSerial comm, unsigned long rate, uint8_t config)
    : _comm(comm)
    , _rate(rate)
    , _config(config)
{
}

//-----------------------------------------------------------------------------
//
dcclt::comm::SerialComm::~SerialComm()
{
    _comm.end();
}

//=============================================================================
// M E T H O D S   C O D E   S E C T I O N

//-----------------------------------------------------------------------------
//
void
dcclt::comm::SerialComm::begin()
{
    _comm.begin(_rate, _config);
}

//-----------------------------------------------------------------------------
//
void
dcclt::comm::SerialComm::flush()
{
    _comm.flush();
}

//-----------------------------------------------------------------------------
//
bool
dcclt::comm::SerialComm::available()
{
    return _comm.available() > 0;
}

//-----------------------------------------------------------------------------
//
byte
dcclt::comm::SerialComm::read()
{
    return _comm.read();
}

//-----------------------------------------------------------------------------
//
void
dcclt::comm::SerialComm::write(const byte& data)
{
    _comm.write(data);
}
