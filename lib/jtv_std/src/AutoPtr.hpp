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

#pragma once
// No include of h file because this is an hpp

//=============================================================================
// C O N S T A N T S   &   L O C A L   V A R I A B L E S


//=============================================================================
// C O N S T R U C T O R ( S ) / D E S T R U C T O R   C O D E   S E C T I O N

//-----------------------------------------------------------------------------
//
template<typename TYPE>
jtv::std::AutoPtr<TYPE>::AutoPtr()
    : _ptr(nullptr)
    , _ref_count(new jtv::std::RefCount())
{
    _ref_count->increment();
}

//-----------------------------------------------------------------------------
//
template<typename TYPE>
jtv::std::AutoPtr<TYPE>::AutoPtr(TYPE* ptr)
    : _ptr(ptr)
    , _ref_count(new jtv::std::RefCount())
{
    _ref_count->increment();
}

//-----------------------------------------------------------------------------
//
template<typename TYPE>
jtv::std::AutoPtr<TYPE>::AutoPtr(const jtv::std::AutoPtr<TYPE>& rhs)
    : _ptr(rhs._ptr)
    , _ref_count(rhs._ref_count)
{
    _ref_count->increment();
}

//-----------------------------------------------------------------------------
//
template<typename TYPE>
template<typename BASE>
jtv::std::AutoPtr<TYPE>::AutoPtr(const jtv::std::AutoPtr<BASE>& rhs)
    : _ptr(static_cast<TYPE*>(rhs._ptr))
    , _ref_count(rhs._ref_count)
{
    _ref_count->increment();
}

//-----------------------------------------------------------------------------
//
template<typename TYPE>
jtv::std::AutoPtr<TYPE>::~AutoPtr()
{
    decrement_ref();
}

//=============================================================================
// M E T H O D S   C O D E   S E C T I O N

//-----------------------------------------------------------------------------
//
template<typename TYPE>
jtv::std::AutoPtr<TYPE>&
jtv::std::AutoPtr<TYPE>::operator=(const jtv::std::AutoPtr<TYPE>& rhs)
{
    if (this != &rhs) 
    {
        decrement_ref();
        _ptr = rhs._ptr;
        _ref_count = rhs._ref_count;
        _ref_count->increment();
    }

    return *this;
}

//-----------------------------------------------------------------------------
//
template<typename TYPE>
TYPE&
jtv::std::AutoPtr<TYPE>::operator*() const
{
    return *_ptr;
}

//-----------------------------------------------------------------------------
//
template<typename TYPE>
TYPE*
jtv::std::AutoPtr<TYPE>::operator->() const
{
    return _ptr;
}

//-----------------------------------------------------------------------------
//
template<typename TYPE>
bool 
jtv::std::AutoPtr<TYPE>::operator==(const AutoPtr<TYPE>& rhs) const
{
    return _ptr == rhs._ptr;
}

//-----------------------------------------------------------------------------
//
template<typename TYPE>
bool 
jtv::std::AutoPtr<TYPE>::operator!=(const AutoPtr<TYPE>& rhs) const
{
    return _ptr != rhs._ptr;
}

//-----------------------------------------------------------------------------
//
template<typename TYPE>
bool 
jtv::std::AutoPtr<TYPE>::operator==(void* rhs) const
{
    return _ptr == rhs;
}

//-----------------------------------------------------------------------------
//
template<typename TYPE>
bool 
jtv::std::AutoPtr<TYPE>::operator!=(void* rhs) const
{
    return _ptr != rhs;
}

//-----------------------------------------------------------------------------
//
template<typename TYPE>
template<typename OTHER>
bool 
jtv::std::AutoPtr<TYPE>::operator==(const AutoPtr<OTHER>& rhs) const
{
    return _ptr == (void*)rhs._ptr;
}

//-----------------------------------------------------------------------------
//
template<typename TYPE>
template<typename OTHER>
bool 
jtv::std::AutoPtr<TYPE>::operator!=(const AutoPtr<OTHER>& rhs) const
{
    return _ptr != (void*)rhs._ptr;
}

//-----------------------------------------------------------------------------
//
template<typename TYPE>
jtv::std::AutoPtr<TYPE>::operator bool() const
{
    return _ptr != nullptr;
}

//-----------------------------------------------------------------------------
//
template<typename TYPE>
template<typename CHILD>
jtv::std::AutoPtr<CHILD>
jtv::std::AutoPtr<TYPE>::as()
{
    AutoPtr<CHILD> result;
    result._ptr = static_cast<CHILD*>(_ptr);
    result._ref_count = _ref_count;
    _ref_count->increment();

    return result;
}

//-----------------------------------------------------------------------------
//
template<typename TYPE>
void
jtv::std::AutoPtr<TYPE>::release()
{
    decrement_ref();
    if (_ptr != nullptr)
    {
        _ptr = nullptr;
        _ref_count = new jtv::std::RefCount();
        _ref_count->increment();
    }
}

//-----------------------------------------------------------------------------
//
template<typename TYPE>
void
jtv::std::AutoPtr<TYPE>::decrement_ref()
{
    if (_ref_count->decrement() == 0)
    {
        delete _ptr;
        _ptr = nullptr;

        delete _ref_count;
        _ref_count = new jtv::std::RefCount();
        _ref_count->increment();
    }
}
