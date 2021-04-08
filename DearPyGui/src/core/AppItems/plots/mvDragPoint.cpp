#include <utility>
#include "mvDragPoint.h"
#include "mvCore.h"
#include "mvApp.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvDragPoint::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String);
		mvAppItem::AddCommonArgs(parser);
		parser.removeArg("width");
		parser.removeArg("height");
		parser.removeArg("callback_data");
		parser.removeArg("enabled");

		parser.addArg<mvPyDataType::FloatList>("default_value", mvArgType::KEYWORD, "(0.0, 0.0, 0.0, 0.0)");


		parser.addArg<mvPyDataType::IntList>("color", mvArgType::KEYWORD, "(0, 0, 0, -255)");


		parser.addArg<mvPyDataType::Float>("thickness", mvArgType::KEYWORD, "1.0");

		parser.addArg<mvPyDataType::Bool>("show_label", mvArgType::KEYWORD, "True");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvDragPoint::mvDragPoint(const std::string& name)
		: mvFloat4PtrBase(name)
	{
	}

	bool mvDragPoint::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvPlot)
			return true;

		mvThrowPythonError(1000, "Item's parent must be plot.");
		MV_ITEM_REGISTRY_ERROR("Item's parent must be plot.");
		assert(false);
		return false;
	}

	void mvDragPoint::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id;
		mvFontScope fscope(this);

		static double dummyx = (*m_value.get())[0];
		static double dummyy = (*m_value.get())[1];
		dummyx = (*m_value.get())[0];
		dummyy = (*m_value.get())[1];

		if (ImPlot::DragPoint(m_label.c_str(), &dummyx, &dummyy, m_show_label, m_color, m_radius))
		{
			(*m_value.get())[0] = dummyx;
			(*m_value.get())[1] = dummyy;
			mvApp::GetApp()->getCallbackRegistry().addCallback(m_callback, m_name, nullptr);
		}

	}

	void mvDragPoint::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "color")) m_color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "radius")) m_radius = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "show_label")) m_show_label = ToBool(item);

	}

	void mvDragPoint::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
	}

}