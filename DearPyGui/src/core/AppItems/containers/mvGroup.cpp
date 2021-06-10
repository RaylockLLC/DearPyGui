#include "mvGroup.h"
#include "mvInput.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
//#include "mvImGuiThemeScope.h"
//#include "mvFontScope.h"

namespace Marvel {

	void mvGroup::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Undocumented function", { "Containers", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_WIDTH |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_FILTER |
			MV_PARSER_ARG_DROP_CALLBACK |
			MV_PARSER_ARG_DRAG_CALLBACK |
			MV_PARSER_ARG_PAYLOAD_TYPE |
			MV_PARSER_ARG_TRACKED |
			MV_PARSER_ARG_SEARCH_DELAY |
			MV_PARSER_ARG_POS)
		);

		parser.addArg<mvPyDataType::Bool>("horizontal", mvArgType::KEYWORD_ARG, "False");
		parser.addArg<mvPyDataType::Float>("horizontal_spacing", mvArgType::KEYWORD_ARG, "-1");


		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvGroup::mvGroup(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	void mvGroup::draw(ImDrawList* drawlist, float x, float y)
	{
		//mvImGuiThemeScope scope(this);
		//mvFontScope fscope(this);

		if (m_width != 0)
			ImGui::PushItemWidth((float)m_width);

		ImGui::BeginGroup();

		//we do this so that the children dont get the theme
		//scope.cleanup();

		for (auto& item : m_children[1])
		{
			if (m_width != 0)
				item->setWidth(m_width);

			if (!item->preDraw())
				continue;

			item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

			item->postDraw();

			if (m_horizontal)
				ImGui::SameLine(0.0, m_hspacing);
		}

		if (m_width != 0)
			ImGui::PopItemWidth();

		ImGui::EndGroup();
	}

	void mvGroup::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		if (PyObject* item = PyDict_GetItemString(dict, "horizontal")) m_horizontal = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "horizontal_spacing")) m_hspacing = ToFloat(item);
	}

	void mvGroup::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		PyDict_SetItemString(dict, "horizontal", ToPyBool(m_horizontal));
		PyDict_SetItemString(dict, "horizontal_spacing", ToPyFloat(m_hspacing));
	}

}