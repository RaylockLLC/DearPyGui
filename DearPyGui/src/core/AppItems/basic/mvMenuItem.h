#pragma once

#include "mvTypeBases.h"
#include "mvApp.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvMenuItem, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Bool, 1);
	class mvMenuItem : public mvBoolPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPITEM_TYPE(mvAppItemType::mvMenuItem, add_menu_item)

		MV_CREATE_CONSTANT(mvThemeCol_MenuItem_Text				, ImGuiCol_Text			, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_MenuItem_BgHovered		, ImGuiCol_HeaderHovered, 0L);

		MV_CREATE_CONSTANT(ImGuiStyleVar_MenuItem_TextAlignX	, ImGuiStyleVar_SelectableTextAlign, 0L);
		MV_CREATE_CONSTANT(ImGuiStyleVar_MenuItem_TextAlignY	, ImGuiStyleVar_SelectableTextAlign, 1L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_ADD_CONSTANT(mvThemeCol_MenuItem_Text,		mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_MenuItem_BgHovered,	mvColor( 66, 150, 250, 240), mvColor(128, 128, 128, 0)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT(ImGuiStyleVar_MenuItem_TextAlignX, 0, 1),
			MV_ADD_CONSTANT(ImGuiStyleVar_MenuItem_TextAlignY, 0, 1),
		MV_END_STYLE_CONSTANTS

	public:

		explicit mvMenuItem(const std::string& name, bool default_value, const std::string& dataSource);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		std::string m_shortcut;
		bool        m_default_value = false;
		bool        m_check = false;
		std::string m_source = "";

	};

}