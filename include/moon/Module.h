/**
* Copyright (c) 2006-2016 LOVE Development Team
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgment in the product documentation would be
*    appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
*    misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
**/

#pragma once

#include "moon/Object.h"

namespace moon
{

using ModuleTypeID = size_t;

namespace Internal
{
inline size_t GetUniqueModuleTypeID() noexcept
{
    static ModuleTypeID id{ 0u };
    return id++;
}
}

template <typename T>
inline ModuleTypeID GetModuleTypeID() noexcept
{
    static_assert(std::is_base_of<Module, T>::value,
        "T must inherit from Module");

    static ModuleTypeID type_id{Internal::GetUniqueModuleTypeID()};
    return type_id;
}

class Module : public Object
{
public:
	// todo rm from mgr
	virtual ~Module() {}

	virtual ModuleTypeID TypeID() const = 0;

	virtual const char* GetName() const = 0;

}; // Module

}