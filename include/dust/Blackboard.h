#pragma once

#include <cu/cu_macro.h>

#include <memory>

namespace dust
{

class Context;

class Blackboard
{
public:
	std::shared_ptr<Context> ctx = nullptr;

private:
	CU_SINGLETON_DECLARATION(Blackboard);

}; // Blackboard

}