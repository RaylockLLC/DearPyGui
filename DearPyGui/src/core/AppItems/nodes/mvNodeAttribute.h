#pragma once
#include "mvAppItem.h"
#include "mvItemRegistry.h"
#include <stdint.h>
#include <imnodes.h>

namespace Marvel {

	MV_REGISTER_WIDGET(mvNodeAttribute, MV_ITEM_DESC_CONTAINER, StorageValueTypes::None, 1);
	class mvNodeAttribute : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvNodeAttribute, add_node_attribute)

		MV_CREATE_CONSTANT(mvNode_PinShape_Circle, imnodes::PinShape::PinShape_Circle, 0L);
		MV_CREATE_CONSTANT(mvNode_PinShape_CircleFilled, imnodes::PinShape::PinShape_CircleFilled, 0L);
		MV_CREATE_CONSTANT(mvNode_PinShape_Triangle, imnodes::PinShape::PinShape_Triangle, 0L);
		MV_CREATE_CONSTANT(mvNode_PinShape_TriangleFilled, imnodes::PinShape::PinShape_TriangleFilled, 0L);
		MV_CREATE_CONSTANT(mvNode_PinShape_Quad, imnodes::PinShape::PinShape_Quad, 0L);
		MV_CREATE_CONSTANT(mvNode_PinShape_QuadFilled, imnodes::PinShape::PinShape_QuadFilled, 0L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
			MV_ADD_GENERAL_CONSTANT(mvNode_PinShape_Circle),
			MV_ADD_GENERAL_CONSTANT(mvNode_PinShape_CircleFilled),
			MV_ADD_GENERAL_CONSTANT(mvNode_PinShape_Triangle),
			MV_ADD_GENERAL_CONSTANT(mvNode_PinShape_TriangleFilled),
			MV_ADD_GENERAL_CONSTANT(mvNode_PinShape_Quad),
			MV_ADD_GENERAL_CONSTANT(mvNode_PinShape_QuadFilled)
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

	public:

		mvNodeAttribute(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		bool isParentCompatible(mvAppItemType type) override;

		int getId() const {return m_id;}

		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:
        int m_id = 0;
		bool m_output = false;
		bool m_static = false;
		imnodes::PinShape m_shape = imnodes::PinShape_CircleFilled;

	};

}