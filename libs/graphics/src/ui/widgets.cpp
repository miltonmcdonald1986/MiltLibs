#include <graphics/ui/widgets.h>

#include <string>

#include <imgui.h>

#include <graphics/components/color.h>
#include <graphics/components/flash.h>

using graphics::app::app::App;
using graphics::components::color::Color;
using graphics::components::flash::Flash;

namespace graphics::ui::widgets
{

    void draw_flash_widget(app::app::App& app)
    {
        ImGui::SetNextWindowSize(ImVec2(300, 0), ImGuiCond_FirstUseEver);
        ImGui::Begin("Flash Settings");
        auto view = app.reg.view<Flash>();
        for (auto [entity, flash] : view.each()) {
			float speed = static_cast<float>(flash.speed);
            std::string label = "entity " + std::to_string((uint32_t)entity);
            ImGui::SliderFloat(label.c_str(), &speed, 0.1f, 10.0f);
            flash.speed = speed;
        }
		ImGui::End();
    }

    void draw_per_entity_color_widget(app::app::App& app)
    {
        ImGui::SetNextWindowSize(ImVec2(300, 0), ImGuiCond_FirstUseEver);
        ImGui::Begin("Entity Colors");
        auto view = app.reg.view<Color>();
        for (auto [entity, color] : view.each()) {
            std::string label = "entity " + std::to_string((uint32_t)entity);
            ImGui::ColorEdit4(label.c_str(), color.base);
        }
        ImGui::End();
    }

	void draw_render_settings_widget(App& app)
	{
        ImGui::SetNextWindowSize(ImVec2(300, 0), ImGuiCond_FirstUseEver);
        ImGui::Begin("Render Settings");
        ImGui::Checkbox("Wireframe", &app.glState.display_wireframe);
        ImGui::ColorEdit3("Background", app.glState.clear_color);
        ImGui::End();
	}

}
