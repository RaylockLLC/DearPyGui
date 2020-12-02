#pragma once

#include "core/AppItems/mvTypeBases.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvText
//     * mvLabelText
//
//-----------------------------------------------------------------------------

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvText
	//-----------------------------------------------------------------------------
	class mvText : public mvPtrBase<std::string, 1>
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Text, "add_text")

		mvText(const std::string& name, const std::string& default_value, const std::string& dataSource);

		void draw              () override;
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		mvColor m_color = {1000, 0, 0, 255, false};
		int     m_wrap = 0;
		bool    m_bullet = false;

	};

	//-----------------------------------------------------------------------------
	// mvLabelText
	//-----------------------------------------------------------------------------
	class mvLabelText : public mvPtrBase<std::string, 1>
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::LabelText, "add_label_text")

		mvLabelText(const std::string& name, const std::string& value, const std::string& dataSource);

		void draw              ()               override;
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		mvColor m_color = { 1000, 0, 0, 255, false };

	};

}