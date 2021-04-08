#include <utility>
#include "mvShadeSeries.h"
#include "mvCore.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImPlotThemeScope.h"

namespace Marvel {

	void mvShadeSeries::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String);
		mvAppItem::AddCommonArgs(parser);
		parser.removeArg("width");
		parser.removeArg("height");
		parser.removeArg("callback");
		parser.removeArg("callback_data");
		parser.removeArg("enabled");

		parser.addArg<mvPyDataType::FloatList>("x");
		parser.addArg<mvPyDataType::FloatList>("y1");
		parser.addArg<mvPyDataType::FloatList>("y2");

		parser.addArg<mvPyDataType::Integer>("axis", mvArgType::KEYWORD, "0");

		parser.addArg<mvPyDataType::Bool>("contribute_to_bounds", mvArgType::KEYWORD, "True");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvShadeSeries::mvShadeSeries(const std::string& name)
		: mvSeriesBase(name)
	{
	}

	void mvShadeSeries::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id;
		mvImPlotThemeScope scope(this);

		switch (m_axis)
		{
		case ImPlotYAxis_1:
			ImPlot::SetPlotYAxis(ImPlotYAxis_1);
			break;
		case ImPlotYAxis_2:
			ImPlot::SetPlotYAxis(ImPlotYAxis_2);
			break;
		case ImPlotYAxis_3:
			ImPlot::SetPlotYAxis(ImPlotYAxis_3);
			break;
		default:
			break;
		}

		static const std::vector<float>* xptr;
		static const std::vector<float>* y1ptr;
		static const std::vector<float>* y2ptr;

		xptr = &(*m_value.get())[0];
		y1ptr = &(*m_value.get())[1];
		y2ptr = &(*m_value.get())[2];

		ImPlot::PlotShaded(m_label.c_str(), xptr->data(), y1ptr->data(),
			y2ptr->data(), (int)xptr->size());

	}

	void mvShadeSeries::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "axis")) m_axis = (ImPlotYAxis_)ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "contribute_to_bounds")) m_contributeToBounds = ToBool(item);

		bool valueChanged = false;
		if (PyObject* item = PyDict_GetItemString(dict, "x")) { valueChanged = true; (*m_value)[0] = ToFloatVect(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "y1")) { valueChanged = true; (*m_value)[1] = ToFloatVect(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "y2")) { valueChanged = true; (*m_value)[2] = ToFloatVect(item); }

		if (valueChanged)
		{
			if ((*m_value)[1].size() != (*m_value)[2].size())
			{
				(*m_value)[2].clear();
				for (auto& item : (*m_value)[1])
					(*m_value)[2].push_back(0.0f);
			}
			resetMaxMins();
			calculateMaxMins();
		}
	}

	void mvShadeSeries::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
	}

}