
#include <Core/CoreAll.h>
#include <Fusion/FusionAll.h>
#include <CAM/CAMAll.h>
#include <iostream>
#include <format>
#include <string>
#include <Fusion/BRep/BRepBody.h>


using namespace adsk::core;
using namespace adsk::fusion;
using namespace adsk::cam;

Ptr<Application> app;
Ptr<UserInterface> ui;

// Global command input declarations.
std::vector<Ptr<BRepBody> > selectedBodies;


class MySelectHandler : public SelectionEventHandler
{
public:
	void notify(const Ptr<SelectionEventArgs>& eventArgs) override
	{
		if (!eventArgs)
			return;
		Ptr<Selection> selection = eventArgs->selection();
		
		Ptr<BRepBodies> body = selection->entity();

		Ptr<BoundingBox3D> bounds = body->boundingBox();

		// Display the results.
		app->log("Standard Bounding Box");
		app->log("Min Point: " + std::to_string(bounds->minPoint()->x()) + ", " +
			std::to_string(bounds->minPoint()->y()) + ", " +
			std::to_string(bounds->minPoint()->z()));
		
		selectedBodies.emplace_back(body);
	}
};

// CommandExecuted event handler.
class OnExecuteEventHander : public adsk::core::CommandEventHandler
{
public:
	void notify(const Ptr<CommandEventArgs>& eventArgs) override
	{

		
		// Display the results.
		app->log("Stad ");
		// Get the bounding box.

		
		
	}
};
// CommandCreated event handler.
class CommandCreatedEventHandler : public adsk::core::CommandCreatedEventHandler
{
public:
	void notify(const Ptr<CommandCreatedEventArgs>& eventArgs) override
	{
		if (eventArgs)
		{
			Ptr<Command> cmd = eventArgs->command();
			if (cmd)
			{
				Ptr<Command> cmd = eventArgs->command();
				// Define the inputs.
				Ptr<CommandInputs> inputs = cmd->commandInputs();

				// Add an input to have a body selected.
				Ptr<SelectionCommandInput> bodiesInput = inputs->addSelectionInput("bodiesInput", "Bodies",
					"Select the bodies.");
				bodiesInput->addSelectionFilter("Bodies");
				
				
				
			

			
				

			
			
	
				// Connect to the command executed event.
				Ptr<CommandEvent> onExec = cmd->execute();
				bool isOk = onExec->add(&onExecuteHandler_);

			}
		}
	}

private:
	OnExecuteEventHander onExecuteHandler_;
	MySelectHandler m_selectHandler;
} _cmdCreated;
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

	
    // Create a command definition and add a button to the CREATE panel.
    Ptr<CommandDefinition> cmdDef = ui->commandDefinitions()->addButtonDefinition("sreliefCPPAddIn", "MMill relief", "a Mill relief component", "Resources/Mill relief");
    if (!checkReturn(cmdDef))
        return false;

    Ptr<ToolbarPanel> createPanel = ui->allToolbarPanels()->itemById("SolidModifyPanel");
    if (!checkReturn(createPanel))
        return false;

    Ptr<CommandControl> gearButton = createPanel->controls()->addCommand(cmdDef);
    if (!checkReturn(gearButton))
        return false;

	// Connect to the Command Created event.
	     Ptr<CommandCreatedEvent> commandCreatedEvent = cmdDef->commandCreated();
	     commandCreatedEvent->add(&_cmdCreated);

    

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
	
	  Ptr<ToolbarPanel> createPanel = ui->allToolbarPanels()->itemById("SolidModifyPanel");
    if (!checkReturn(createPanel))
        return false;

    Ptr<CommandControl> gearButton = createPanel->controls()->itemById("sreliefCPPAddIn");
    if (checkReturn(gearButton))
        gearButton->deleteMe();
    
    Ptr<CommandDefinition> cmdDef = ui->commandDefinitions()->itemById("sreliefCPPAddIn");
    if (checkReturn(cmdDef))
        cmdDef->deleteMe();
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
