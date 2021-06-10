#include "mvThemeColor.h"
#include "mvApp.h"
#include <array>
#include <implot.h>
#include <imnodes.h>
#include "mvItemRegistry.h"
//#include "mvImGuiThemeScope.h"
#include "mvLog.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvThemeColor::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Undocumented function", { "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_LABEL)
		);

		parser.addArg<mvPyDataType::Integer>("target", mvArgType::POSITIONAL_ARG, "0");
		parser.addArg<mvPyDataType::IntList>("default_value", mvArgType::POSITIONAL_ARG, "(0, 0, 0, 255)");
		parser.addArg<mvPyDataType::Integer>("category", mvArgType::KEYWORD_ARG, "0");

		parser.finalize();

		parsers->insert({ s_command, parser });

	}

	mvThemeColor::mvThemeColor(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
	}

	bool mvThemeColor::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvTheme) return true;
		mvThrowPythonError(1000, "Item's parent must be plot.");
		MV_ITEM_REGISTRY_ERROR("Item's parent must be plot.");
		assert(false);
		return false;
	}

	void mvThemeColor::draw(ImDrawList* drawlist, float x, float y)
	{
		if (m_libType == mvLibType::MV_IMGUI)
			ImGui::PushStyleColor(m_targetColor, m_color.toVec4());
		else if (m_libType == mvLibType::MV_IMPLOT)
			ImPlot::PushStyleColor(m_targetColor,m_color.toVec4());
		else if (m_libType == mvLibType::MV_IMNODES)
			imnodes::PushColorStyle((imnodes::ColorStyle)m_targetColor, mvColor::ConvertToUnsignedInt(m_color));
	}

	void mvThemeColor::customAction()
	{
		if (m_libType == mvLibType::MV_IMGUI)
			ImGui::PopStyleColor();
		else if (m_libType == mvLibType::MV_IMPLOT)
			ImPlot::PopStyleColor();
		else if (m_libType == mvLibType::MV_IMNODES)
			imnodes::PopColorStyle();
	}

	void mvThemeColor::handleSpecificPositionalArgs(PyObject* dict)
	{
		if (!mvApp::GetApp()->getParsers()[s_command].verifyPositionalArguments(dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				m_targetColor = ToInt(item);
				break;

			case 1:
				m_color = ToColor(item);
				break;

			default:
				break;
			}
		}
	}

	void mvThemeColor::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "category")) m_libType = (mvLibType)ToInt(item);
	}

}
