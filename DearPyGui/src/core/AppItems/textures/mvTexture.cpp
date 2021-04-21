#include "mvTexture.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"

namespace Marvel {

	void mvTexture::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::None);
		mvAppItem::AddCommonArgs(parser);
		parser.removeArg("source");
		parser.removeArg("before");
		parser.removeArg("label");
		parser.removeArg("callback");
		parser.removeArg("callback_data");
		parser.removeArg("show");
		parser.removeArg("enabled");
		parser.removeArg("width");
		parser.removeArg("height");

		parser.addArg<mvPyDataType::Integer>("width");
		parser.addArg<mvPyDataType::Integer>("height");
		parser.addArg<mvPyDataType::FloatList>("default_value");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvTexture::mvTexture(const std::string& name)
		:
		mvFloatVectPtrBase(name)
	{

	}

	mvTexture::~mvTexture()
	{
		//UnloadTexture(m_name);
		FreeTexture(m_texture);
	}


	bool mvTexture::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvTextureContainer) return true;

		mvThrowPythonError(1000, "Drawing item parent must be a drawing.");
		MV_ITEM_REGISTRY_ERROR("Drawing item parent must be a drawing.");
		assert(false);
		return false;
	}

	void mvTexture::draw(ImDrawList* drawlist, float x, float y)
	{
		if (!m_dirty)
			return;

		if (m_name == "INTERNAL_DPG_FONT_ATLAS")
		{
			m_texture = ImGui::GetIO().Fonts->TexID;
			m_width = ImGui::GetIO().Fonts->TexWidth;
			m_height = ImGui::GetIO().Fonts->TexHeight;
		}
		else
			m_texture = LoadTextureFromArray(m_width, m_height, m_value->data());

		m_dirty = false;
	}

	void mvTexture::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!mvApp::GetApp()->getParsers()[s_command].verifyRequiredArguments(dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				m_width = ToInt(item);
				break;

			case 1:
				m_height = ToInt(item);
				break;

			case 2:
				*m_value = ToFloatVect(item);
				break;

			default:
				break;
			}
		}
	}

	void mvTexture::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
	}


}