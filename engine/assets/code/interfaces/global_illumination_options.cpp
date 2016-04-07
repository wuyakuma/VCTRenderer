#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "global_illumination_options.h"

#include "main_menu.h"
#include "../core/assets_manager.h"
#include "../renderers/deferred_renderer.h"

#include <string>
#include <glm/gtc/type_ptr.hpp>


using namespace ImGui;

void UIGlobalIllumination::Draw()
{
    if (!UIMainMenu::drawGIOptions) { return; }

    static auto &assets = AssetsManager::Instance();
    static auto &deferred = *static_cast<DeferredRenderer *>
                            (assets->renderers["Deferred"].get());

    if (Begin("Global Illumination", &UIMainMenu::drawGIOptions,
              ImGuiWindowFlags_AlwaysAutoResize))
    {
        static auto maxTracingConeDistance = deferred.MaxTracingDistance();
        static auto bounceStrength = deferred.GlobalIlluminationStrength();
        static auto aoFalloff = deferred.AmbientOclussionFalloff();
        static auto aoAlpha = deferred.AmbientOclussionAlpha();
        static auto mode = 0;
        static auto modes = "Direct + Indirect + Occlusion\0Direct + Indirec" \
                            "t\0Direct\0Indirect\0Ambient Occlusion";

        if(SliderFloat("Maximum Trace Distance", &maxTracingConeDistance, 0.0f, 1.0f))
        {
            deferred.MaxTracingDistance(maxTracingConeDistance);
        }

        if(SliderFloat("GI Strength", &bounceStrength, 0.0f, 32.0f))
        {
            deferred.GlobalIlluminationStrength(bounceStrength);
        }

        if(Combo("Render Mode", &mode, modes))
        {
            deferred.RenderMode(mode);
        }

        Text("Ambient Occlusion");
        Indent();

        if (InputFloat("Falloff", &aoFalloff, 0.1))
        {
            deferred.AmbientOclussionFalloff(aoFalloff);
        }

        if (SliderFloat("Alpha", &aoAlpha, 0.0f, 1.0f))
        {
            deferred.AmbientOclussionAlpha(aoAlpha);
        }

        Unindent();
    }

    End();
}
UIGlobalIllumination::UIGlobalIllumination()
{
}
UIGlobalIllumination::~UIGlobalIllumination()
{
}