#include "mvCollapsingHeader.h"
#include "mvInput.h"
#include "mvPythonTranslator.h"
#include "mvGlobalIntepreterLock.h"
#include "mvValueStorage.h"
#include "mvApp.h"

namespace Marvel {

	void mvCollapsingHeader::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_collapsing_header", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "label", "", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip", "''"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Bool, "closable", "", "False"},
			{mvPythonDataType::Bool, "default_open", "", "False"},
			{mvPythonDataType::Bool, "open_on_double_click", "Need double-click to open node", "False"},
			{mvPythonDataType::Bool, "open_on_arrow", "Only open when clicking on the arrow part.", "False"},
			{mvPythonDataType::Bool, "leaf", "No collapsing, no arrow (use as a convenience for leaf nodes).", "False"},
			{mvPythonDataType::Bool, "bullet", "Display a bullet instead of arrow", "False"},
		}, "Adds a collapsing header to add items to. Must be closed with the end command.",
			"None", "Containers") });
	}

	mvCollapsingHeader::mvCollapsingHeader(const std::string& name)
		: mvBoolPtrBase(name, true, name)
	{
		m_description.container = true;
	}

	void mvCollapsingHeader::draw()
	{
		auto styleManager = m_styleManager.getScopedStyleManager();
		ScopedID id;

		bool* toggle = nullptr;
		if (m_closable)
			toggle = &m_show;
		*m_value = ImGui::CollapsingHeader(m_label.c_str(), toggle, m_flags);
		if (*m_value)
		{

			// Regular Tooltip (simple)
			if (!m_tip.empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", m_tip.c_str());

			for (auto& item : m_children)
			{
				// skip item if it's not shown
				if (!item->m_show)
					continue;

				// set item width
				if (item->m_width != 0)
					ImGui::SetNextItemWidth((float)item->m_width);

				item->draw();

				// Regular Tooltip (simple)
				if (!item->m_tip.empty() && ImGui::IsItemHovered())
					ImGui::SetTooltip("%s", item->m_tip.c_str());

				item->getState().update();
			}
		}

		else
		{
			// Regular Tooltip (simple)
			if (!m_tip.empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", m_tip.c_str());
		}
	}

	void mvCollapsingHeader::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		if (PyObject* item = PyDict_GetItemString(dict, "closable")) m_closable = ToBool(item);

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		// flags
		flagop("default_open", ImGuiTreeNodeFlags_DefaultOpen, m_flags);
		flagop("open_on_double_click", ImGuiTreeNodeFlags_OpenOnDoubleClick, m_flags);
		flagop("open_on_arrow", ImGuiTreeNodeFlags_OpenOnArrow, m_flags);
		flagop("leaf", ImGuiTreeNodeFlags_Leaf, m_flags);
		flagop("bullet", ImGuiTreeNodeFlags_Bullet, m_flags);


	}

	void mvCollapsingHeader::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		PyDict_SetItemString(dict, "closable", ToPyBool(m_closable));

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};

		// flags
		checkbitset("default_open", ImGuiTreeNodeFlags_DefaultOpen, m_flags);
		checkbitset("open_on_double_click", ImGuiTreeNodeFlags_OpenOnDoubleClick, m_flags);
		checkbitset("open_on_arrow", ImGuiTreeNodeFlags_OpenOnArrow, m_flags);
		checkbitset("leaf", ImGuiTreeNodeFlags_Leaf, m_flags);
		checkbitset("bullet", ImGuiTreeNodeFlags_Bullet, m_flags);

	}

	PyObject* add_collapsing_header(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* label = "";
		int show = true;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		int closable = false;
		int default_open = false;
		int open_on_double_click = false;
		int open_on_arrow = false;
		int leaf = false;
		int bullet = false;


		if (!(*mvApp::GetApp()->getParsers())["add_collapsing_header"].parse(args, kwargs, __FUNCTION__, &name,
			&label, &show, &tip, &parent, &before, &closable, &default_open, &open_on_double_click, &open_on_arrow, &leaf, &bullet))
			return ToPyBool(false);

		auto item = CreateRef<mvCollapsingHeader>(name);
		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		auto fut = mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				std::string returnMessage = mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before);
				if (returnMessage.empty())
					mvApp::GetApp()->getItemRegistry().pushParent(item);

				return returnMessage;
			});

		std::string returnMessage = fut.get();
		if (!returnMessage.empty())
			ThrowPythonException(returnMessage);

		return GetPyNone();
	}

}