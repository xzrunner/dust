#pragma once

#include <cu/cu_macro.h>

#include <memory>

namespace dust
{

class Context;

class Blackboard
{
public:
	void SetContext(const std::shared_ptr<Context>& ctx) { m_ctx = ctx; }
	const std::shared_ptr<Context>& GetContext() const { return m_ctx; }

private:
	std::shared_ptr<Context> m_ctx = nullptr;

	CU_SINGLETON_DECLARATION(Blackboard);

}; // Blackboard

}