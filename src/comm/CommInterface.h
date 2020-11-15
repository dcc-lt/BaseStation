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

#include <Arduino.h>

// =============================================================================
// C L A S S E S

namespace dcclt { 
namespace comm {

class CommInterface
{
public:
    // Virtual dtor
    virtual ~CommInterface() {};
    // Initialisation methods
    virtual void begin() = 0;
    virtual void flush() = 0;
    // Check if data is availabe from the interface
    // returns true if data can be read, false otherwise
    virtual bool available() = 0;
    // Read the next byte of data 
    virtual byte read() = 0;
    // Write a byte of data on the interface
    virtual void write(const byte&) = 0;
};

}}
