
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
Ptr<BoundingBox3D> _selectedBodies;
Ptr<TextBoxCommandInput> _xDimension;
Ptr<TextBoxCommandInput> _yDimension;
Ptr<TextBoxCommandInput> _zDimension;

void CalculateDimensions();


class MySelectHandler : public SelectionEventHandler
{
public:
	void notify(const Ptr<SelectionEventArgs>& eventArgs) override
	{
		if (!eventArgs)
			return;
		Ptr<Selection> selection = eventArgs->selection();

		Ptr<BRepBody> body = selection->entity();


		_selectedBodies = body->boundingBox();
		app->log("test 2 ");

		CalculateDimensions();

	}
};

// CommandExecuted event handler.
class OnExecuteEventHander : public adsk::core::CommandEventHandler
{
public:
	void notify(const Ptr<CommandEventArgs>& eventArgs) override
	{

		// Display the results.
		app->log("Min Point: " + std::to_string(_selectedBodies->minPoint()->x()) + ", " +
			std::to_string(_selectedBodies->minPoint()->y()) + ", " +
			std::to_string(_selectedBodies->minPoint()->z()));

		app->log("Max Point: " + std::to_string(_selectedBodies->maxPoint()->x()) + ", " +
			std::to_string(_selectedBodies->maxPoint()->y()) + ", " +
			std::to_string(_selectedBodies->maxPoint()->z()));
		// Display the results.
		app->log("test ");
		
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

				_xDimension = inputs->addTextBoxCommandInput("xDimension", "Dimension in X", "", 1, true);

				_yDimension = inputs->addTextBoxCommandInput("yDimension", "Dimension in Y", "", 1, true);

				_zDimension = inputs->addTextBoxCommandInput("zDimension", "Dimension in Z", "", 1, true);



				Ptr<SelectionEvent> select = cmd->select();

				select->add(&m_selectHandler);

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

void CalculateDimensions() {

	double xmin = _selectedBodies->minPoint()->x();
	double xmax = _selectedBodies->maxPoint()->x();
	double ymin = _selectedBodies->minPoint()->y();
	double ymax = _selectedBodies->maxPoint()->y();
	double zmin = _selectedBodies->minPoint()->z();
	double zmax = _selectedBodies->maxPoint()->z();

	_xDimension == xmax- xmin;
	_yDimension == ymax - ymin;
	_zDimension == zmax - zmin;



	app->log("test 3 ");
	



};



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
