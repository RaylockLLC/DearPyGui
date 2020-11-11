#pragma once
#include "core/AppItems/mvTypeBases.h"
#include "mvAppItem.h"
#include "core/mvInput.h"

namespace Marvel {

	class mvMetricsWindow : public mvBaseWindowAppitem
	{

		MV_APPITEM_TYPE(mvAppItemType::MetricsWindow, "add_metrics_window")

	public:

		mvMetricsWindow(const std::string& name) 
			: mvBaseWindowAppitem(name)
		{
			m_description.deleteAllowed = false;
		}

		void draw() override
		{
			if (!prerender())
				return;

			ImGuiIO& io = ImGui::GetIO();
			ImGui::Text("Dear PyGui %s", mvApp::GetVersion());
			ImGui::Text("Dear ImGui %s", ImGui::GetVersion());
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::Text("%d vertices, %d indices (%d triangles)", io.MetricsRenderVertices, io.MetricsRenderIndices, io.MetricsRenderIndices / 3);
			ImGui::Text("%d active windows (%d visible)", io.MetricsActiveWindows, io.MetricsRenderWindows);
			ImGui::Text("%d active allocations", io.MetricsActiveAllocations);

			if (ImGui::IsWindowFocused())
			{

				float titleBarHeight = ImGui::GetStyle().FramePadding.y * 2 + ImGui::GetFontSize();

				// update mouse
				mvVec2 oldMousePos = mvInput::getGlobalMousePosition();
				ImVec2 mousePos = ImGui::GetMousePos();
				mvInput::setGlobalMousePosition(mousePos.x, mousePos.y);
				float x = mousePos.x - ImGui::GetWindowPos().x;
				float y = mousePos.y - ImGui::GetWindowPos().y - titleBarHeight;
				mvInput::setMousePosition(x, y);

				mvApp::GetApp()->setActiveWindow("metrics##standard");

				// mouse move callback
				if (getMouseMoveCallback() != nullptr)
				{
					if (oldMousePos.x != mousePos.x || oldMousePos.y != mousePos.y)
					{
						mvApp::GetApp()->runCallback(getMouseMoveCallback(), m_name,
							ToPyPair(x, y));
					}
				}

			}

			ImGui::End();
		}

	};

}