#include <utility>
#include "mvListbox.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvListbox::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String, "Undocumented function", { "Widgets" });
		mvAppItem::AddCommonArgs(parser);
		parser.removeArg("height");

		parser.addArg<mvPyDataType::StringList>("items", mvArgType::POSITIONAL_ARG, "()");

		parser.addArg<mvPyDataType::String>("default_value", mvArgType::KEYWORD_ARG, "''");
		parser.addArg<mvPyDataType::Integer>("num_items", mvArgType::KEYWORD_ARG, "3", "number of items to show");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvListbox::mvListbox(const std::string& name)
		: mvStringPtrBase(name)
	{
	}

	void mvListbox::setPyValue(PyObject* value)
	{
		*m_value = ToString(value);
		updateIndex();
	}

	void mvListbox::updateIndex()
	{
		m_index = 0;
		m_disabledindex = 0;

		int index = 0;
		for (const auto& name : m_names)
		{
			if (name == *m_value)
			{
				m_index = index;
				m_disabledindex = index;
				break;
			}
			index++;
		}
	}

	void mvListbox::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id;
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		if (!m_enabled)
		{
			m_disabled_value = *m_value;
			m_disabledindex = m_index;
		}

		if (ImGui::ListBox(m_label.c_str(), m_enabled ? &m_index : &m_disabledindex, m_charNames.data(), (int)m_names.size(), m_itemsHeight))
		{
			*m_value = m_name[m_index];
			m_disabled_value = m_name[m_index];
			mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callback_data);
		}
	}

	void mvListbox::handleSpecificPositionalArgs(PyObject* dict)
	{
		if (!mvApp::GetApp()->getParsers()[s_command].verifyPositionalArguments(dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				m_names = ToStringVect(item);
				m_charNames.clear();
				for (const std::string& item : m_names)
					m_charNames.emplace_back(item.c_str());
				break;

			default:
				break;
			}
		}
	}

	void mvListbox::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		if (PyObject* item = PyDict_GetItemString(dict, "num_items")) m_itemsHeight = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "items"))
		{
			m_names = ToStringVect(item);
			m_charNames.clear();
			for (const std::string& item : m_names)
				m_charNames.emplace_back(item.c_str());
			updateIndex();
		}
	}

	void mvListbox::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		PyDict_SetItemString(dict, "items", ToPyList(m_names));
		PyDict_SetItemString(dict, "num_items", ToPyInt(m_itemsHeight));
	}

}