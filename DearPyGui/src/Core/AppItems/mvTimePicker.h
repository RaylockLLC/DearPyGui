#pragma once

#include "mvApp.h"
#include "Core/AppItems/mvTypeBases.h"
#include <implot.h>
#include <implot_internal.h>
#include <misc/cpp/imgui_stdlib.h>
#include "mvPythonTranslator.h"


//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvTimePicker
//
//-----------------------------------------------------------------------------

namespace Marvel {

	class mvTimePicker : public mvTimePtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::TimePicker, "add_time_picker")

		mvTimePicker(const std::string& name, tm default_value)
			: mvTimePtrBase(name, default_value, name)
		{
		}

		void draw() override
		{
			auto styleManager = m_styleManager.getScopedStyleManager();
			ImGui::PushID(this);

			ImPlot::GetStyle().Use24HourClock = m_hour24;

			if (ImPlot::ShowTimePicker(m_name.c_str(), m_imvalue))
			{
				ImPlot::GetGmtTime(*m_imvalue, m_value);
				mvApp::GetApp()->addCallback(m_callback, m_name, m_callbackData);
			}

			ImGui::PopID();
		}

		void setExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			if (PyObject* item = PyDict_GetItemString(dict, "hour24")) m_hour24 = ToBool(item);
		}

		void getExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "hour24", ToPyBool(m_hour24));
		}

	private:

		bool m_hour24 = false;

	};

}