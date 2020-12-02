#pragma once

#include "mvApp.h"
#include "AppItems/mvTypeBases.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvInputText
//
//-----------------------------------------------------------------------------

namespace Marvel
{

	class mvInputText : public mvPtrBase<std::string, 1>
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::InputText, "add_input_text")

		mvInputText(const std::string& name, const std::string& default_value, const std::string& dataSource);

		void setEnabled(bool value) override;
		void draw() override;
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		std::string         m_hint;
		bool                m_multiline = false;
		ImGuiInputTextFlags m_flags = 0;
		ImGuiInputTextFlags m_stor_flags = 0;

	};

}