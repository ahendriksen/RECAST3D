#include <iostream>

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>
#include <imgui.h>

#include "graphics/components/movie_component.hpp"
#include "graphics/scene_camera_3d.hpp"

namespace tomovis {

MovieComponent::MovieComponent(SceneObject& object, int scene_id,
                               std::string file)
    : object_(object), scene_id_(scene_id), model_(file) {}

MovieComponent::~MovieComponent() {}

void MovieComponent::describe() {
    if (model_.load_progress() < 1.0f) {
        ImGui::OpenPopup("Loading movie");
        if (ImGui::BeginPopupModal("Loading movie", NULL,
                                   ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::ProgressBar(model_.load_progress(), ImVec2(0.0f, 0.0f));
            ImGui::SameLine();
            ImGui::Text("Loading..");

            ImGui::EndPopup();
        }
    }

    if (ImGui::Button("Toggle")) {
        model_.toggle_pause();
    }

    if (ImGui::Button("Rotate")) {
        model_.toggle_rotate();
    }

    ImGui::SliderFloat("Scale", &model_.scale(), 0.05f, 1.0f);

    ImGui::Separator();

    recorder_.describe();

    ImGui::Separator();

    projection_.describe();
}

void MovieComponent::tick(float time_elapsed) {
    time_ += time_elapsed;
    model_.tick(time_elapsed);
    projection_.tick(time_elapsed);
}

void MovieComponent::draw(glm::mat4 world_to_screen) {
    auto camera_position = object_.camera().position();
    model_.draw(world_to_screen, camera_position);
    projection_.draw(world_to_screen, model_, camera_position);
    recorder_.capture();
}

} // namespace tomovis
