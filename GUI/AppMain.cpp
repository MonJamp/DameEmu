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

void AppMain::MainLoop()
{
	app.create(sf::VideoMode(800, 720), "");
    //app.setVerticalSyncEnabled(true);
    ImGui::SFML::Init(app);

	fileDialog.SetTitle("Select ROM");
	fileDialog.SetTypeFilters({".gb", ".GB"});
	fileDialog.SetWindowSize(640, 480);

    sf::Color bgColor;

    float color[3] = { 0.f, 0.f, 0.f };

    // let's use char array as buffer, see next part
    // for instructions on using std::string with ImGui
    char windowTitle[255] = "ImGui + SFML = <3";

    app.setTitle(windowTitle);
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
				ImGui::SetNextWindowPos(ImVec2(0.f, 20.f));
				ImGui::Begin("DameEmu", NULL,
					ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar |
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
				}
				else
				{
					showDebugger = false;
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

int main()
{
    AppMain app;
	app.MainLoop();
	return 0;
}