
#include <Core/CoreAll.h>
#include <Fusion/FusionAll.h>
#include <CAM/CAMAll.h>


using namespace adsk::core;
using namespace adsk::fusion;
using namespace adsk::cam;

Ptr<Application> app;
Ptr<UserInterface> ui;


bool checkReturn(Ptr<Base> returnObj)
{
    if (returnObj)
        return true;
    else
        if (app && ui)
        {
            std::string errDesc;
            app->getLastError(&errDesc);
            ui->messageBox(errDesc);
            return false;
        }
        else
            return false;
}

extern "C" XI_EXPORT bool run(const char* context)
{
	app = Application::get();
	if (!app)
		return false;

	ui = app->userInterface();
	if (!ui)
		return false;

	ui->messageBox("Czesc, Tomasz");
    // Create a command definition and add a button to the CREATE panel.
    Ptr<CommandDefinition> cmdDef = ui->commandDefinitions()->addButtonDefinition("adskMill reliefCPPAddIn", "Mill relief", "Creates a Mill relief component", "Resources/Mill relief");
    if (!checkReturn(cmdDef))
        return false;

    Ptr<ToolbarPanel> createPanel = ui->allToolbarPanels()->itemById("SolidCreatePanel");
    if (!checkReturn(createPanel))
        return false;

    Ptr<CommandControl> gearButton = createPanel->controls()->addCommand(cmdDef);
    if (!checkReturn(gearButton))
        return false;

    // Connect to the command created event.
    Ptr<CommandCreatedEvent> commandCreatedEvent = cmdDef->commandCreated();
    if (!checkReturn(commandCreatedEvent))
        return false;

    ui->messageBox("Czeœæ");

    std::string strContext = context;
    if (strContext.find("IsApplicationStartup", 0) != std::string::npos)
    {
        if (strContext.find("false", 0) != std::string::npos)
        {
            ui->messageBox("The \"Mill relief\" command has been added\nto the CREATE panel of the MODEL workspace.");
        }
    }


	return true;
}

extern "C" XI_EXPORT bool stop(const char* context)
{
	if (ui)
	{
		ui->messageBox("Stop addin");
		ui = nullptr;
	}

	return true;
}


#ifdef XI_WIN

#include <windows.h>

BOOL APIENTRY DllMain(HMODULE hmodule, DWORD reason, LPVOID reserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

#endif // XI_WIN
