#include "AppMain.h"

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <vector>


AppMain::AppMain()
{
	running = false;
	showDebugger = false;
	dameEmu = std::make_unique<DameEmu>();
	debuggerWidget = new DebuggerWidget(dameEmu->GetDebugger(), running);
	MainCanvas::Init();
}

AppMain::~AppMain()
{
	delete debuggerWidget;
}

void SetUpImGuiStyle();

void AppMain::MainLoop()
{
	app.create(sf::VideoMode(160 * 3, 144 * 3 + 37), "DameEmu");
    //app.setVerticalSyncEnabled(true);
    ImGui::SFML::Init(app);
	SetUpImGuiStyle();

	fileDialog.SetTitle("Select ROM");
	fileDialog.SetTypeFilters({".gb", ".GB"});
	fileDialog.SetWindowSize(480, 432);

    sf::Clock deltaClock;
    while (app.isOpen()) {

		if (running)
		{
			dameEmu->Step();

			if (debuggerWidget->BreakExecution())
			{
				running = false;
			}
		}

		if (MainCanvas::IsVblank() || !running)
		{
			sf::Event event;
			while (app.pollEvent(event)) {
				ImGui::SFML::ProcessEvent(event);

				if (event.type == sf::Event::Closed) {
					app.close();
				}
			}

			ImGui::SFML::Update(app, deltaClock.restart());

			app.clear(sf::Color::Black);
			MenuBar();

			{
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
				if (showDebugger)
				{
					ImGui::SetNextWindowPos(ImVec2(320.f, 720.f - (144.f * 3.f + ImGui::GetFrameHeight())));
				}
				else
				{
					ImGui::SetNextWindowPos(ImVec2(2.5f, ImGui::GetFrameHeight()));
				}

				ImGui::Begin("DameEmu", NULL,
					ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration |
					ImGuiWindowFlags_NoBringToFrontOnFocus);
				ImGui::Image(MainCanvas::GetSprite());
				ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				ImGui::End();
				ImGui::PopStyleVar();
			}

			ImGui::SFML::Render(app);
			app.display();
		}
    }

    ImGui::SFML::Shutdown();
}

void AppMain::MenuBar()
{
	bool showAbout = false;

	if (showDebugger)
	{
		ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.79f, 0.32f, 0.0f, 1.00f));
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImGui::GetStyle().Colors[ImGuiCol_MenuBarBg]);
	}

	if(ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("System"))
		{
			if(ImGui::MenuItem("Load ROM"))
			{
				fileDialog.Open();
			}

			if(ImGui::MenuItem("Reset"))
			{
				running = true;
				dameEmu->Reset();
			}

			ImGui::Separator();

			if(ImGui::MenuItem("Exit"))
			{
				app.close();
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Emulation"))
		{
			if(ImGui::MenuItem("Settings"))
			{
				//Do something
			}

			if(ImGui::MenuItem("Debugger"))
			{
				if (!showDebugger)
				{
					delete debuggerWidget;
					debuggerWidget = new DebuggerWidget(dameEmu->GetDebugger(), running);
					showDebugger = true;
					app.setSize({ 800, 720 });
				}
				else
				{
					showDebugger = false;
					app.setSize({ 160 * 3, 144 * 3 + 37});
				}
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Misc"))
		{
			if(ImGui::MenuItem("About"))
			{
				
				showAbout = true;
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	ImGui::PopStyleColor();

	fileDialog.Display();
	if(fileDialog.HasSelected())
	{
		dameEmu = std::make_unique<DameEmu>(fileDialog.GetSelected().string());
		fileDialog.ClearSelected();
	}

	if (showDebugger)
	{
		debuggerWidget->Show();
	}

	if(showAbout)
	{
		ImGui::OpenPopup("About DameEmu");
		showAbout = false;
	}

	if(ImGui::BeginPopupModal("About DameEmu"))
	{
		ImGui::Text("DameEmu");
		ImGui::Text("https://github.com/MonJamp/DameEmu");

		if(ImGui::Button("Close"))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

inline void SetUpImGuiStyle()
{
	ImGuiStyle& style = ImGui::GetStyle();

	ImVec4 textColor = { 1.00f, 1.00f, 1.00f, 1.00f };
	ImVec4 bodyColor = { 0.15f, 0.30f, 0.40f, 1.00f };
	ImVec4 areaColor = { 0.16f, 0.49f, 0.70f, 1.00f };
	ImVec4 headColor = { 0.06f, 0.10f, 0.15f, 1.00f };
	ImVec4 popsColor = { 0.11f, 0.25f, 0.35f, 1.00f };


	style.Colors[ImGuiCol_Text]					= ImVec4(textColor.x, textColor.y, textColor.z, 1.00f);
	style.Colors[ImGuiCol_TextDisabled]			= ImVec4(textColor.x, textColor.y, textColor.z, 0.58f);
	style.Colors[ImGuiCol_WindowBg]				= ImVec4(bodyColor.x, bodyColor.y, bodyColor.z, 0.95f);
	style.Colors[ImGuiCol_Border]				= ImVec4(bodyColor.x, bodyColor.y, bodyColor.z, 0.00f);
	style.Colors[ImGuiCol_BorderShadow]			= ImVec4(bodyColor.x, bodyColor.y, bodyColor.z, 0.00f);
	style.Colors[ImGuiCol_FrameBg]				= ImVec4(areaColor.x, areaColor.y, areaColor.z, 0.50f);
	style.Colors[ImGuiCol_FrameBgHovered]		= ImVec4(headColor.x, headColor.y, headColor.z, 0.78f);
	style.Colors[ImGuiCol_FrameBgActive]		= ImVec4(headColor.x, headColor.y, headColor.z, 1.00f);
	style.Colors[ImGuiCol_TitleBg]				= ImVec4(areaColor.x, areaColor.y, areaColor.z, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed]		= ImVec4(areaColor.x, areaColor.y, areaColor.z, 0.75f);
	style.Colors[ImGuiCol_TitleBgActive]		= ImVec4(headColor.x, headColor.y, headColor.z, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg]			= ImVec4(bodyColor.x, bodyColor.y, bodyColor.z, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg]			= ImVec4(areaColor.x, areaColor.y, areaColor.z, 0.20f);
	style.Colors[ImGuiCol_ScrollbarGrab]		= ImVec4(headColor.x, headColor.y, headColor.z, 0.60f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(headColor.x, headColor.y, headColor.z, 0.78f);
	style.Colors[ImGuiCol_ScrollbarGrabActive]	= ImVec4(headColor.x, headColor.y, headColor.z, 1.00f);
	style.Colors[ImGuiCol_CheckMark]			= ImVec4(headColor.x, headColor.y, headColor.z, 1.00f);
	style.Colors[ImGuiCol_SliderGrab]			= ImVec4(headColor.x, headColor.y, headColor.z, 0.50f);
	style.Colors[ImGuiCol_SliderGrabActive]		= ImVec4(headColor.x, headColor.y, headColor.z, 1.00f);
	style.Colors[ImGuiCol_Button]				= ImVec4(headColor.x, headColor.y, headColor.z, 0.50f);
	style.Colors[ImGuiCol_ButtonHovered]		= ImVec4(headColor.x, headColor.y, headColor.z, 0.86f);
	style.Colors[ImGuiCol_ButtonActive]			= ImVec4(headColor.x, headColor.y, headColor.z, 1.00f);
	style.Colors[ImGuiCol_Header]				= ImVec4(headColor.x, headColor.y, headColor.z, 0.76f);
	style.Colors[ImGuiCol_HeaderHovered]		= ImVec4(headColor.x, headColor.y, headColor.z, 0.86f);
	style.Colors[ImGuiCol_HeaderActive]			= ImVec4(headColor.x, headColor.y, headColor.z, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip]			= ImVec4(headColor.x, headColor.y, headColor.z, 0.15f);
	style.Colors[ImGuiCol_ResizeGripHovered]	= ImVec4(headColor.x, headColor.y, headColor.z, 0.78f);
	style.Colors[ImGuiCol_ResizeGripActive]		= ImVec4(headColor.x, headColor.y, headColor.z, 1.00f);
	style.Colors[ImGuiCol_PlotLines]			= ImVec4(textColor.x, textColor.y, textColor.z, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered]		= ImVec4(headColor.x, headColor.y, headColor.z, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram]		= ImVec4(textColor.x, textColor.y, textColor.z, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered]	= ImVec4(headColor.x, headColor.y, headColor.z, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg]		= ImVec4(headColor.x, headColor.y, headColor.z, 0.43f);
	style.Colors[ImGuiCol_PopupBg]				= ImVec4(popsColor.x, popsColor.y, popsColor.z, 0.92f);
}

int main()
{
    AppMain app;
	app.MainLoop();
	return 0;
}