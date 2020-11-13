#pragma once

#include "mvAppItemStyleManager.h"

namespace Marvel {

    mvAppItemStyleManagerScope::mvAppItemStyleManagerScope(mvAppItemStyleManager& manager)
        : m_manager(manager)
    {
       
    }

    mvAppItemStyleManagerScope::mvAppItemStyleManagerScope(mvAppItemStyleManagerScope&& other) noexcept
        : m_manager(other.m_manager), m_moved(true)
    {
        m_manager.pushColorStyles();
    }

    mvAppItemStyleManagerScope::~mvAppItemStyleManagerScope()
    {
        if(m_moved)
            m_manager.popColorStyles();
        m_manager.clearTempColors();
    }

    mvAppItemStyleManagerScope mvAppItemStyleManager::getScopedStyleManager()
    {
        return std::move(mvAppItemStyleManagerScope(*this));
    }

    void mvAppItemStyleManagerScope::addColorStyle(ImGuiCol item, ImVec4 color)
    {
        m_manager.addTempColorStyle(item, mvColor(color));
        ImGui::PushStyleColor(item, color);
    }

    void mvAppItemStyleManager::addColorStyle(ImGuiCol item, mvColor color)
    {
        m_colors.push_back({ item, color });
    }

    void mvAppItemStyleManager::addTempColorStyle(ImGuiCol item, mvColor color)
    {
        m_colors_temp.push_back({ item, color });
    }

    void mvAppItemStyleManager::pushColorStyles()
    {
        for (auto& item : m_colors)
            ImGui::PushStyleColor(item.idx, item.color);
    }

    void mvAppItemStyleManager::popColorStyles()
    {
        if (!m_colors.empty())
            ImGui::PopStyleColor((int)m_colors.size());

        ImGui::PopStyleColor((int)m_colors_temp.size());
    }

    void mvAppItemStyleManager::clearColors()
    {
        m_colors.clear();
    }

    void mvAppItemStyleManager::clearTempColors()
    {
        m_colors_temp.clear();
    }

}