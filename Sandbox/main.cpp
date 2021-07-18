#include <imgui_impl_dx11.h>
#include <thread>
#include "mvMath.h"
#include "mvWindow.h"
#include "mvGraphics.h"
#include "mvCommonBindables.h"
#include "mvCommonDrawables.h"
#include "mvPointShadowMappingPass.h"
#include "mvDirectionalShadowMappingPass.h"
#include "mvLambertianPass.h"
#include "mvCamera.h"
#include "mvTimer.h"
#include "mvPointLight.h"
#include "mvDirectionLight.h"
#include "mvModel.h"
#include "mvBaseRenderGraph.h"
#include "mvModelProbe.h"
#include "mvComputeShader.h"
#include "mvObjMaterial.h"
#include "mvClearBufferPass.h"
#include "mvOverlayPass.h"
#include "mvSkyboxPass.h"

using namespace Marvel;

static std::shared_ptr<mvClearBufferPass>              clear_target;
static std::shared_ptr<mvClearBufferPass>              clear_depth;
static std::shared_ptr<mvPointShadowMappingPass>       point_shadow;
static std::shared_ptr<mvDirectionalShadowMappingPass> direction_shadow;
static std::shared_ptr<mvLambertianPass>               lambertian;
static std::shared_ptr<mvSkyboxPass>                   skybox_pass;
static std::shared_ptr<mvOverlayPass>                  overlay;

void HandleEvents(mvWindow& window, float dt, mvCamera& camera);

void AddPasses(mvGraphics& graphics, mvBaseRenderGraph& graph)
{
    clear_target = std::make_shared<mvClearBufferPass>("clear_target");
    clear_target->m_renderTarget = graph.m_renderTarget;
    graph.addPass(clear_target);

    clear_depth = std::make_shared<mvClearBufferPass>("clear_depth");
    clear_depth->m_depthStencil = graph.m_depthStencil;
    graph.addPass(clear_depth);

    point_shadow = std::make_shared<mvPointShadowMappingPass>(graphics, "shadow", 3);
    graph.addPass(point_shadow);

    direction_shadow = std::make_shared<mvDirectionalShadowMappingPass>(graphics, "directional_shadow", 4);
    graph.addPass(direction_shadow);

    lambertian = std::make_shared<mvLambertianPass>(graphics, "lambertian");
    lambertian->m_depthStencil = clear_depth->m_depthStencil;
    lambertian->m_renderTarget = clear_target->m_renderTarget;
    lambertian->m_depthCube = point_shadow->m_depthCube;
    lambertian->m_depthTexture = direction_shadow->m_depthTexture;
    graph.addPass(lambertian);

    skybox_pass = std::make_shared<mvSkyboxPass>(graphics, "skybox", "../../Resources/SkyBox");
    skybox_pass->m_depthStencil = lambertian->m_depthStencil;
    skybox_pass->m_renderTarget = lambertian->m_renderTarget;
    graph.addPass(skybox_pass);

    overlay = std::make_shared<mvOverlayPass>(graphics, "overlay");
    overlay->m_renderTarget = skybox_pass->m_renderTarget;
    graph.addPass(overlay);
}

void ResetPasses()
{
    clear_target = nullptr;
    clear_depth = nullptr;
    point_shadow = nullptr;
    direction_shadow = nullptr;
    lambertian = nullptr;
    skybox_pass = nullptr;
    overlay = nullptr;
}

int main()
{

    int width = 1850;
    int height = 900;

    // create window
    mvWindow window("Marvel", width, height);

    // create graphics
    mvGraphics graphics(window.getHandle(), width, height);

    // create render graph
    auto graph = std::make_unique<mvBaseRenderGraph>(graphics);
    AddPasses(graphics, *graph);

    auto directionLight = mvDirectionLight(graphics, { 0.0f, -1.0f, 0.0f });

    mvPointLight pointlight(graphics, "light0", { 0.0f, 5.0f, 0.0f });
    auto lightcamera = pointlight.getCamera();

    // create camera
    mvCamera camera(graphics, "maincamera", { -13.5f, 6.0f, 3.5f }, 0.0f, PI / 2.0f, width, height, 0.5f, 400.0f, false);

    // create model
    std::shared_ptr<mvModel> sponza = nullptr;
    std::shared_ptr<mvModel> gun = nullptr;
    std::shared_ptr<mvModel> ball = nullptr;
    bool showSponza = false;
    bool showGun = false;
    bool showBall = false;
    bool modelsDirty = true;

    // testing compute shaders
    struct BufType { float f; };
    BufType* inputRawBuffer = new BufType[1024];

    for (int i = 0; i < 1024; ++i)
        inputRawBuffer[i].f = 1.5f;

    mvComputeShader computeShader(graphics, graphics.getShaderRoot() + "testcompute.hlsl");
    mvComputeInputBuffer<BufType> inputBuffer(graphics, 0, inputRawBuffer, 1024);
    mvComputeOutputBuffer<BufType> outputBuffer(graphics, 0, 1024);

    // timer
    Marvel::mvTimer timer;

    // Main Loop
    while (true)
    {
        // process all messages pending, but to not block for new messages
        if (const auto ecode = mvWindow::ProcessMessages())
            break;

        if (modelsDirty || window.wantsResize())
        {
            ResetPasses();
            graph.reset();

            graphics.releaseBuffers();
            graphics.resize(window.getClientWidth(), window.getClientHeight());
            camera.updateProjection(window.getClientWidth(), window.getClientHeight());
            window.setResizedFlag(false);

            graph = std::make_unique<mvBaseRenderGraph>(graphics);
            AddPasses(graphics, *graph);

            if (showGun)
            {
                if (!gun)
                {
                    gun = std::make_shared<mvModel>(graphics, "../../Dependencies/MarvelAssets/cerberus/cereberus.obj", 1.0f);
                    gun->setRootTransform(glm::translate(glm::vec3(-5.0f, 5.0f, 0.0f)));
                }
                gun->linkSteps(*graph);
            }

            if (showBall)
            {
                if (!ball)
                {
                    ball = std::make_shared<mvModel>(graphics, "../../Resources/shader_ball/shader_ball.obj", 1.0f);
                    ball->setRootTransform(glm::translate(glm::vec3(5.0f, 5.0f, 0.0f)));
                }
                ball->linkSteps(*graph);
            }

            if (showSponza)
            {
                if (!sponza)
                    sponza = std::make_shared<mvModel>(graphics, "../../Dependencies/MarvelAssets/Sponza/sponza.obj", 1.0f);
                sponza->linkSteps(*graph);
            }

            pointlight.linkSteps(*graph);
            camera.linkSteps(*graph);
            lightcamera->linkSteps(*graph);


            lambertian->bindShadowCamera(*pointlight.getCamera());
            lambertian->bindDirectionalShadowCamera(*directionLight.getCamera());
            point_shadow->bindShadowCamera(*pointlight.getCamera());
            direction_shadow->bindShadowCamera(*directionLight.getCamera());
            modelsDirty = false;
        }

        const auto dt = timer.mark() * 1.0f;

        HandleEvents(window, dt, camera);

        graphics.beginFrame();

        graph->bindMainCamera(camera);
        lambertian->bindMainCamera(camera);
        overlay->bindMainCamera(camera);

        lambertian->bindShadowCamera(*pointlight.getCamera());
        lambertian->bindDirectionalShadowCamera(*directionLight.getCamera());
        point_shadow->bindShadowCamera(*pointlight.getCamera());
        direction_shadow->bindShadowCamera(*directionLight.getCamera());

        graph->bind(graphics);

        pointlight.bind(graphics, camera.getMatrix());
        directionLight.bind(graphics, camera.getMatrix());

        if (showSponza)
            sponza->submit(*graph);
        if (showGun)
            gun->submit(*graph);
        if (showBall)
            ball->submit(*graph);
        pointlight.submit(*graph);

        graph->execute(graphics);

        static mvModelProbe probe1(graphics, "Sponza Model Probe");
        static mvModelProbe probe2(graphics, "Gun Model Probe");
        static mvModelProbe probe3(graphics, "Ball Model Probe");
        if (showSponza)
            probe1.spawnWindow(*sponza);
        if (showGun)
            probe2.spawnWindow(*gun);
        if (showBall)
            probe3.spawnWindow(*ball);
        pointlight.show_imgui_window();
        directionLight.show_imgui_window();
        graph->show_imgui_window();
        directionLight.getCamera()->show_imgui_windows();

        ImGuiIO& io = ImGui::GetIO();
        ImGui::GetForegroundDrawList()->AddText(ImVec2(45, 45),
            ImColor(0.0f, 1.0f, 0.0f), std::string(std::to_string(io.Framerate) + " FPS").c_str());
        camera.show_pos();

        if (ImGui::Begin("Sandbox Options"))
        {
            if (ImGui::Button("Execute"))
            {
                std::thread t([&]() {
                    inputBuffer.bind(graphics);
                    outputBuffer.bind(graphics);
                    computeShader.dispatch(graphics, 1024, 1, 1);
                    graphics.finishRecording();

                    });
                t.detach();
            }
            if (ImGui::Checkbox("Sponza", &showSponza))
                modelsDirty = true;
            if (ImGui::Checkbox("Gun", &showGun))
                modelsDirty = true;
            if (ImGui::Checkbox("Ball", &showBall))
                modelsDirty = true;
        }
        ImGui::End();

        graph->reset();

        graphics.endFrame();

        if (graphics.isCommandListReady())
        {
            BufType* p = outputBuffer.getDataFromGPU(graphics);

            float a = p[0].f;
            float b = p[1].f;
            float c = p[2].f;

            graphics.resetCommandList();
        }
    }

}

void HandleEvents(mvWindow& window, float dt, mvCamera& camera)
{
    while (const auto e = window.kbd.readKey())
    {
        if (!e->isPress())
            continue;

        switch (e->getCode())
        {
        case VK_ESCAPE:
            if (window.cursorEnabled())
            {
                window.disableCursor();
                window.mouse.enableRaw();
            }
            else
            {
                window.enableCursor();
                window.mouse.disableRaw();
            }
            break;
        }
    }

    if (!window.cursorEnabled())
    {
        if (window.kbd.keyIsPressed('W'))
            camera.translate(0.0f, 0.0f, dt);

        if (window.kbd.keyIsPressed('A'))
            camera.translate(-dt, 0.0f, 0.0f);

        if (window.kbd.keyIsPressed('S'))
            camera.translate(0.0f, 0.0f, -dt);

        if (window.kbd.keyIsPressed('D'))
            camera.translate(dt, 0.0f, 0.0f);

        if (window.kbd.keyIsPressed('R'))
            camera.translate(0.0f, dt, 0.0f);

        if (window.kbd.keyIsPressed('F'))
            camera.translate(0.0f, -dt, 0.0f);
    }

    while (const auto delta = window.mouse.readRawDelta())
    {
        if (!window.cursorEnabled())
            camera.rotate((float)delta->x, (float)delta->y);
    }
}