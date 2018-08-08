#include "moon/Gui.h"
#include "moon/runtime.h"
#include "moon/Blackboard.h"
#include "moon/Context.h"
#include "moon/Blackboard.h"
#include "moon/module_cfg.h"

#ifdef EASYEDITOR
#include <wx/filedlg.h>
#endif // EASYEDITOR

namespace
{

#define INSTANCE() (moon::Blackboard::Instance()->GetContext()->GetModuleMgr().GetModule<moon::Gui>())

int w_open_file_dlg(lua_State* L)
{
#ifdef EASYEDITOR
	wxWindow* wnd = moon::Blackboard::Instance()->GetWindow();

	std::string name, filter;

	int args = lua_gettop(L);
	if (args >= 1) {
		name = luaL_checkstring(L, 1);
	}
	if (args >= 2) {
		filter = luaL_checkstring(L, 2);
	}

	wxFileDialog dlg(wnd, name, wxEmptyString, wxEmptyString,
		filter, wxFD_OPEN);
	if (dlg.ShowModal() == wxID_OK)
	{
		lua_pushstring(L, dlg.GetPath().ToStdString().c_str());
		return 1;
	}
#endif // EASYEDITOR

	return 0;
}

}

namespace moon
{

// List of functions to wrap.
static const luaL_Reg functions[] =
{
	{ "open_file_dlg", w_open_file_dlg },

	{ 0, 0 }
};

// Types for this module.
static const lua_CFunction types[] =
{
	0
};

extern "C" int luaopen_moon_gui(lua_State* L)
{
	Gui* instance = INSTANCE();
	if (instance == nullptr) {
		luax_catchexcept(L, [&](){ instance = new Gui(); });
	} else {
		instance->Retain();
	}

	WrappedModule w;
	w.module = instance;
	w.name = "gui";
	w.type = MODULE_ID;
	w.functions = functions;
	w.types = types;

	return luax_register_module(L, w);
}

}