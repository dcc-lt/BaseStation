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

#include "RefCount.h"

//=============================================================================
// C O N S T A N T S


// =============================================================================
// C L A S S E S

namespace jtv {
namespace std {

template<typename TYPE>
class AutoPtr
{
public:

    using Type = TYPE;

    AutoPtr();
    AutoPtr(TYPE* ptr);
    AutoPtr(const AutoPtr<TYPE>& rhs);

    template<typename BASE>
    AutoPtr(const AutoPtr<BASE>& rhs);

    virtual ~AutoPtr();

    AutoPtr<TYPE>& operator=(const AutoPtr<TYPE>& rhs);

    TYPE& operator*() const;
    TYPE* operator->() const;
    operator bool() const;

    bool operator==(const AutoPtr<TYPE>& rhs) const;
    bool operator!=(const AutoPtr<TYPE>& rhs) const;
    bool operator==(void* rhs) const;
    bool operator!=(void* rhs) const;
    template<typename OTHER>
    bool operator==(const AutoPtr<OTHER>& rhs) const;
    template<typename OTHER>
    bool operator!=(const AutoPtr<OTHER>& rhs) const;

    template<typename CHILD>
    AutoPtr<CHILD> as();

    void release();

private:

    void decrement_ref();

    TYPE* _ptr;
    RefCount* _ref_count;

    template<typename OTHER>
    friend class AutoPtr;

    // template<typename CHILD, typename BASE>
    // friend AutoPtr<CHILD> staticAutoPtrCast(const AutoPtr<BASE>& rhs);
};

// template<typename CHILD, typename BASE> AutoPtr<CHILD> staticAutoPtrCast(const AutoPtr<BASE>& rhs);

} }

#include "AutoPtr.hpp" // To build templated code
