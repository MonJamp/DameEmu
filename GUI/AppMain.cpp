#include "AppMain.h"

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>


AppMain::AppMain()
{
	dameEmu = std::make_unique<DameEmu>();
}

AppMain::~AppMain()
{

}

void AppMain::MainLoop()
{
	app.create(sf::VideoMode(640, 480), "");
    app.setVerticalSyncEnabled(true);
    ImGui::SFML::Init(app);

	fileDialog.SetTitle("Select ROM");
	fileDialog.SetTypeFilters({".gb"});
	fileDialog.SetWindowSize(640, 480);

    sf::Color bgColor;

    float color[3] = { 0.f, 0.f, 0.f };

    // let's use char array as buffer, see next part
    // for instructions on using std::string with ImGui
    char windowTitle[255] = "ImGui + SFML = <3";

    app.setTitle(windowTitle);
    app.resetGLStates(); // call it if you only draw ImGui. Otherwise not needed.
    sf::Clock deltaClock;
    while (app.isOpen()) {
        sf::Event event;
        while (app.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) {
                app.close();
            }
        }

        ImGui::SFML::Update(app, deltaClock.restart());

		MenuBar();

        ImGui::Begin("Sample window"); // begin window

                                       // Background color edit
        if (ImGui::ColorEdit3("Background color", color)) {
            // this code gets called if color value changes, so
            // the background color is upgraded automatically!
            bgColor.r = static_cast<sf::Uint8>(color[0] * 255.f);
            bgColor.g = static_cast<sf::Uint8>(color[1] * 255.f);
            bgColor.b = static_cast<sf::Uint8>(color[2] * 255.f);
        }

        // Window title text edit
        ImGui::InputText("Window title", windowTitle, 255);

        if (ImGui::Button("Update window title")) {
            // this code gets if user clicks on the button
            // yes, you could have written if(ImGui::InputText(...))
            // but I do this to show how buttons work :)
            app.setTitle(windowTitle);
        }
        ImGui::End(); // end window

        app.clear(bgColor); // fill background with color
        ImGui::SFML::Render(app);
        app.display();
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
				//Do something
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
				//Do something
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
		fileDialog.ClearSelected();
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