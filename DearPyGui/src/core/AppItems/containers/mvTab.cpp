#include "mvTab.h"
#include "mvTabBar.h"
#include "mvApp.h"
#include "mvItemRegistry.h"

namespace Marvel {

	void mvTab::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds a tab to a tab bar. Must be closed with thes end command.", { "Containers", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_FILTER |
			MV_PARSER_ARG_DROP_CALLBACK |
			MV_PARSER_ARG_DRAG_CALLBACK |
			MV_PARSER_ARG_PAYLOAD_TYPE |
			MV_PARSER_ARG_SEARCH_DELAY |
			MV_PARSER_ARG_TRACKED |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::Bool>("closable", mvArgType::KEYWORD_ARG, "False", "Creates a button on the tab that can hide the tab.");
		parser.addArg<mvPyDataType::Bool>("no_reorder", mvArgType::KEYWORD_ARG, "False", "Disable reordering this tab or having another tab cross over this tab.");
		parser.addArg<mvPyDataType::Bool>("leading", mvArgType::KEYWORD_ARG, "False", "Enforce the tab position to the left of the tab bar (after the tab list popup button).");
		parser.addArg<mvPyDataType::Bool>("trailing", mvArgType::KEYWORD_ARG, "False", "Enforce the tab position to the right of the tab bar (before the scrolling buttons).");
		parser.addArg<mvPyDataType::Bool>("no_tooltip", mvArgType::KEYWORD_ARG, "False", "Disable tooltip for the given tab.");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}
	mvTab::mvTab(mvUUID uuid)
		: 
		mvBoolPtrBase(uuid)
	{
	}

	void mvTab::addFlag(ImGuiTabItemFlags flag)
	{
		m_flags |= flag;
	}

	void mvTab::removeFlag(ImGuiTabItemFlags flag)
	{
		m_flags &= ~flag;
	}

	void mvTab::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id(m_uuid);

		// cast parent to mvTabBar
		auto parent = (mvTabBar*)m_parentPtr;

		// check if this is first tab
		if (parent->getSpecificValue() == 0)
		{
			// set mvTabBar value to the first tab name
			parent->setValue(m_uuid);
			*m_value = true;
		}

		// create tab item and see if it is selected
		if (ImGui::BeginTabItem(m_label.c_str(), m_closable ? &m_show : nullptr, m_flags))
		{

			// set other tab's value false
			for (auto& child : parent->m_children[1])
			{
				if (child->getType() == mvAppItemType::mvTab)
					*((mvTab*)child.get())->m_value = false;
			}

			// set current tab value true
			*m_value = true;

			// run call back if it exists
			if (parent->getSpecificValue() != m_uuid)
				mvApp::GetApp()->getCallbackRegistry().addCallback(parent->getCallback(), m_uuid, nullptr, parent->getCallbackData());

			parent->setValue(m_uuid);

			for (auto& item : m_children[1])
			{
				if (!item->preDraw())
					continue;

				item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

				item->postDraw();
			}

			ImGui::EndTabItem();
		}

	}

	void mvTab::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		if (PyObject* item = PyDict_GetItemString(dict, "closable")) m_closable = ToBool(item);

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		// window flags
		flagop("no_reorder", ImGuiTabItemFlags_NoReorder, m_flags);
		flagop("leading", ImGuiTabItemFlags_Leading, m_flags);
		flagop("trailing", ImGuiTabItemFlags_Trailing, m_flags);
		flagop("no_tooltip", ImGuiTabItemFlags_NoTooltip, m_flags);

		if (m_flags & ImGuiTabItemFlags_Leading && m_flags & ImGuiTabItemFlags_Trailing)
			m_flags &= ~ImGuiTabItemFlags_Leading;

	}

	void mvTab::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		PyDict_SetItemString(dict, "closable", ToPyBool(m_closable));

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};

		// window flags
		checkbitset("no_reorder", ImGuiTabBarFlags_Reorderable, m_flags);
		checkbitset("leading", ImGuiTabItemFlags_Leading, m_flags);
		checkbitset("trailing", ImGuiTabItemFlags_Trailing, m_flags);
		checkbitset("no_tooltip", ImGuiTabItemFlags_NoTooltip, m_flags);

	}

}