#pragma once

#include <cu/cu_macro.h>

#include <memory>

namespace dust
{

class LuaVM;

class Blackboard
{
public:
	std::shared_ptr<LuaVM> vm = nullptr;

private:
	CU_SINGLETON_DECLARATION(Blackboard);

}; // Blackboard

}