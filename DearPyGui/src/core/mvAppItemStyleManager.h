#pragma once

#include <vector>
#include <imgui.h>
#include "mvCore.h"


namespace Marvel {

    //-----------------------------------------------------------------------------
    // Forward Declarations
    //-----------------------------------------------------------------------------
    class mvAppItemStyleManager;

    //-----------------------------------------------------------------------------
    // mvAppItemStyleManagerScope: Automates popping styles
    //-----------------------------------------------------------------------------
    class mvAppItemStyleManagerScope
    {

    public:

        explicit mvAppItemStyleManagerScope(mvAppItemStyleManager& manager);
        mvAppItemStyleManagerScope(mvAppItemStyleManagerScope&& other) noexcept;

        mvAppItemStyleManagerScope(const mvAppItemStyleManagerScope& other) = delete;
        mvAppItemStyleManagerScope operator=(const mvAppItemStyleManagerScope& other) = delete;
        mvAppItemStyleManagerScope& operator=(mvAppItemStyleManagerScope&& other) = delete;

        ~mvAppItemStyleManagerScope();

        void addColorStyle(ImGuiCol item, ImVec4 color);

    private:

        mvAppItemStyleManager& m_manager;
        bool                   m_moved = false;

    };

    //-----------------------------------------------------------------------------
    // mvAppItemStyleManager
    //-----------------------------------------------------------------------------
    class mvAppItemStyleManager
    {

        struct StyleColor
        {
            ImGuiCol idx;
            mvColor color;
        };

    public:

        mvAppItemStyleManagerScope getScopedStyleManager();
        void addColorStyle        (ImGuiCol item, mvColor color);
        void addTempColorStyle    (ImGuiCol item, mvColor color);
        void pushColorStyles      ();
        void popColorStyles       ();
        void clearColors          ();
        void clearTempColors      ();

    private:

        std::vector<StyleColor> m_colors;
        std::vector<StyleColor> m_colors_temp;

    };
}