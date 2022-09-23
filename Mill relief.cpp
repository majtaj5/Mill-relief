
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

Ptr<Application> _app;
Ptr<UserInterface> _ui;
Ptr<Product> _product;
Ptr<Design> _design;



// Global command input declarations.
Ptr<BoundingBox3D> _selectedBodies;
Ptr<TextBoxCommandInput> _xDimension;
Ptr<TextBoxCommandInput> _yDimension;
Ptr<TextBoxCommandInput> _zDimension;

Ptr<ValueCommandInput> _setXDimension;
Ptr<ValueCommandInput> _setYDimension;
Ptr<ValueCommandInput> _setZDimension;

Ptr<BRepBody> _body;

Ptr<ValueInput> _scaleFactor;


double CalculatedX;
double CalculatedY;
double CalculatedZ;
double scaleX;
double scaleY;
double scaleZ;
std::string _units = "";


void CalculateDimensions();

class ScaleInputChangedHandler : public adsk::core::InputChangedEventHandler
{
public:
	void notify(const Ptr<InputChangedEventArgs>& eventArgs) override
	{
		Ptr<CommandInput> changedInput = eventArgs->input();
		
		double userInputX = _setXDimension->value();
		double userInputY = _setYDimension->value();
		double userInputZ = _setZDimension->value();
		double scaleX = userInputX / CalculatedX ;
		double scaleY = userInputY / CalculatedY;
		double scaleZ = userInputZ / CalculatedZ;
		_app->log(changedInput->id());
		if (changedInput->id() == "setXDimension") {

			_app->log("X");
			if (userInputX != 0)
			{
				CalculatedX = scaleX * CalculatedX;
				CalculatedY = scaleX * CalculatedY;
				CalculatedZ = scaleX * CalculatedZ;
				_scaleFactor = ValueInput::createByReal(scaleX);

			}
			//return;
			_app->log(std::to_string(userInputX));
			_xDimension->text(std::to_string(userInputX));
			//_xDimension->unitType(_units);
			//units = "mm";
			_app->log(std::to_string(CalculatedY));
			_yDimension->text(std::to_string(CalculatedY));
			_app->log(std::to_string(CalculatedZ));
			_zDimension->text(std::to_string(CalculatedZ));


			Ptr<Design> des = _app->activeProduct();
			std::string CalculatedXText = des->unitsManager()->formatInternalValue(CalculatedX, "mm", true);
			_xDimension->text(CalculatedXText);

			
			std::string CalculatedYText = des->unitsManager()->formatInternalValue(CalculatedY, "mm", true);
			_yDimension->text(CalculatedYText);
			_setYDimension->value(CalculatedY);

			std::string CalculatedZText = des->unitsManager()->formatInternalValue(CalculatedZ, "mm", true);
			_zDimension->text(CalculatedZText);
			_setZDimension->value(CalculatedZ);

			//_setXDimension->value(CalculatedX);
			
		}


		if (changedInput->id() == "setYDimension") {

			_app->log("Y");
			if (userInputY != 0)
			{
				CalculatedX = scaleY * CalculatedX;
				CalculatedY = scaleY * CalculatedY;
				CalculatedZ = scaleY * CalculatedZ;

			}
			//return;

			_app->log(std::to_string(CalculatedX));
			_xDimension->text(std::to_string(CalculatedX));
			//_xDimension->unitType(_units);
			//units = "mm";

			_app->log(std::to_string(userInputY));
			_yDimension->text(std::to_string(userInputY));
			_app->log(std::to_string(CalculatedZ));
			_zDimension->text(std::to_string(CalculatedZ));

			Ptr<Design> des = _app->activeProduct();
			std::string CalculatedXText = des->unitsManager()->formatInternalValue(CalculatedX, "mm", true);
			_xDimension->text(CalculatedXText);
			_setXDimension->value(CalculatedX);

			std::string CalculatedYText = des->unitsManager()->formatInternalValue(CalculatedY, "mm", true);
			_yDimension->text(CalculatedYText);

			std::string CalculatedZText = des->unitsManager()->formatInternalValue(CalculatedZ, "mm", true);
			_zDimension->text(CalculatedZText);
			_setZDimension->value(CalculatedZ);

		}
		if (changedInput->id() == "setZDimension") {

			_app->log("Z");
			if (userInputZ != 0)
			{
				CalculatedX = scaleZ * CalculatedX;
				CalculatedY = scaleZ * CalculatedY;
				CalculatedZ = scaleZ * CalculatedZ;
			}



			//return;
			_app->log(std::to_string(CalculatedX));
			_xDimension->text(std::to_string(CalculatedX));
			//_xDimension->unitType(_units);
			//units = "mm";
			_app->log(std::to_string(CalculatedY));
			_yDimension->text(std::to_string(CalculatedY));
			_app->log(std::to_string(userInputZ));
			_zDimension->text(std::to_string(userInputZ));

			Ptr<Design> des = _app->activeProduct();
			std::string CalculatedXText = des->unitsManager()->formatInternalValue(CalculatedX, "mm", true);
			_xDimension->text(CalculatedXText);
			_setXDimension->value(CalculatedX);

			std::string CalculatedYText = des->unitsManager()->formatInternalValue(CalculatedY, "mm", true);
			_yDimension->text(CalculatedYText);
			_setYDimension->value(CalculatedY);

			std::string CalculatedZText = des->unitsManager()->formatInternalValue(CalculatedZ, "mm", true);
			_zDimension->text(CalculatedZText);
		}
		
	}
};


class MySelectHandler : public SelectionEventHandler
{
public:
	void notify(const Ptr<SelectionEventArgs>& eventArgs) override
	{
		if (!eventArgs)
			return;
		Ptr<Selection> selection = eventArgs->selection();

		Ptr<BRepBody> body = selection->entity();

		_body = body;

		_selectedBodies = body->boundingBox();
		
		CalculateDimensions();

		Ptr<Design> des = _app->activeProduct();
		std::string CalculatedXText = des->unitsManager()->formatInternalValue(CalculatedX,"mm", true);
		_xDimension->text(CalculatedXText);
		_setXDimension->value(CalculatedX);

		std::string CalculatedYText = des->unitsManager()->formatInternalValue(CalculatedY, "mm", true);
		_yDimension->text(CalculatedYText);
		_setYDimension->value(CalculatedY);

		std::string CalculatedZText = des->unitsManager()->formatInternalValue(CalculatedZ, "mm", true);
		_zDimension->text(CalculatedZText);
		_setZDimension->value(CalculatedZ);

	}
};

class MyUnSelectHandler : public SelectionEventHandler
{
public:
	void notify(const Ptr<SelectionEventArgs>& eventArgs) override
	{
		//Ptr<Command> cmd = eventArgs->command();
		CalculatedX = 0;
		CalculatedY = 0;
		CalculatedZ = 0;

		Ptr<Design> des = _app->activeProduct();
		std::string CalculatedXText = des->unitsManager()->formatInternalValue(CalculatedX, "mm", true);
		_xDimension->text(CalculatedXText);
		_setXDimension->value(CalculatedX);

		std::string CalculatedYText = des->unitsManager()->formatInternalValue(CalculatedY, "mm", true);
		_yDimension->text(CalculatedYText);
		_setYDimension->value(CalculatedY);

		std::string CalculatedZText = des->unitsManager()->formatInternalValue(CalculatedZ, "mm", true);
		_zDimension->text(CalculatedZText);
		_setZDimension->value(CalculatedZ);
		
	}
};

// CommandExecuted event handler.
class OnExecuteEventHander : public adsk::core::CommandEventHandler
{
public:
	void notify(const Ptr<CommandEventArgs>& eventArgs) override
	{
		Ptr<ObjectCollection> inputUniformColl = ObjectCollection::create();
		
		inputUniformColl->add(_body);

		Ptr<Component> comp = _body->parentComponent();
		
		Ptr<ScaleFeatures> scales = comp->features()->scaleFeatures();

		// Point for scale
		Ptr<ConstructionPoint> centerPoint = comp->originConstructionPoint();

		Ptr<ScaleFeatureInput> scaleUniformInput = scales->createInput(inputUniformColl, centerPoint, _scaleFactor);
	

		Ptr<ScaleFeature> scaleUniform = scales->add(scaleUniformInput);
		

		// Display the results.
		_app->log("Min Point: " + std::to_string(_selectedBodies->minPoint()->x()) + ", " +
			std::to_string(_selectedBodies->minPoint()->y()) + ", " +
			std::to_string(_selectedBodies->minPoint()->z()));

		_app->log("Max Point: " + std::to_string(_selectedBodies->maxPoint()->x()) + ", " +
			std::to_string(_selectedBodies->maxPoint()->y()) + ", " +
			std::to_string(_selectedBodies->maxPoint()->z()));
		// Display the results.
		_app->log(std::to_string(CalculatedX));
		_app->log(std::to_string(CalculatedY));
		_app->log(std::to_string(CalculatedZ));
		
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

				//todo   Define the default values
				CalculatedX = 0;
				CalculatedY = 0;
				CalculatedZ = 0;
				// Add an input to have a body selected.
				Ptr<SelectionCommandInput> bodiesInput = inputs->addSelectionInput("bodiesInput", "Bodies",
					"Select the bodies.");
				bodiesInput->addSelectionFilter("Bodies");

				_setXDimension = inputs->addValueInput("setXDimension", "Set X Dimension", "mm", ValueInput::createByReal(CalculatedX));

				_setYDimension = inputs->addValueInput("setYDimension", "Set Y Dimension", "mm", ValueInput::createByReal(CalculatedY));

				_setZDimension = inputs->addValueInput("setZDimension", "Set Z Dimension", "mm", ValueInput::createByReal(CalculatedZ));

				_xDimension = inputs->addTextBoxCommandInput("xDimension", "Dimension in X", "", 1, true);

				_yDimension = inputs->addTextBoxCommandInput("yDimension", "Dimension in Y", "", 1, true);

				_zDimension = inputs->addTextBoxCommandInput("zDimension", "Dimension in Z", "", 1, true);



				Ptr<SelectionEvent> select = cmd->select();

				select->add(&m_selectHandler);

				Ptr<SelectionEvent> unselect = cmd->unselect();
				unselect->add(&m_unSelectHandler);

				// Connect to the command executed event.
				Ptr<CommandEvent> onExec = cmd->execute();
				bool isOk = onExec->add(&onExecuteHandler_);

				/*Ptr<TextBoxCommandInput>onExec = cmd->execute();
				bool isOk = onExec->add(&onExecuteHandler_);*/

				// Connect to the command related events.
				Ptr<InputChangedEvent> inputChangedEvent = cmd->inputChanged();
				if (!inputChangedEvent)
					return;
				isOk = inputChangedEvent->add(&_ScaleInputChangedHandler);
				if (!isOk)
					return;
			}
		}
	}

private:
	OnExecuteEventHander onExecuteHandler_;
	MySelectHandler m_selectHandler;
	MyUnSelectHandler m_unSelectHandler;
	ScaleInputChangedHandler _ScaleInputChangedHandler;
} _cmdCreated;

bool checkReturn(Ptr<Base> returnObj)
{
	if (returnObj)
		return true;
	else
		if (_app && _ui)
		{
			std::string errDesc;
			_app->getLastError(&errDesc);
			_ui->messageBox(errDesc);
			return false;
		}
		else
			return false;
}

extern "C" XI_EXPORT bool run(const char* context)
{
	_app = Application::get();
	if (!_app)
		return false;

	_ui = _app->userInterface();
	if (!_ui)
		return false;

	_product = _app->activeProduct();
	if (!_product)
		return false;

	Ptr<Design> _design = _product;
	if (!_design)
		return false;


	// Create a command definition and add a button to the CREATE panel.
	Ptr<CommandDefinition> cmdDef = _ui->commandDefinitions()->addButtonDefinition("sreliefCPPAddIn", "MMill relief", "a Mill relief component", "Resources/Mill relief");
	if (!checkReturn(cmdDef))
		return false;

	Ptr<ToolbarPanel> createPanel = _ui->allToolbarPanels()->itemById("SolidModifyPanel");
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
			_ui->messageBox("The \"Mill relief\" command has been added\nto the CREATE panel of the MODEL workspace.");
		}
	}


	return true;
}

extern "C" XI_EXPORT bool stop(const char* context)
{

	Ptr<ToolbarPanel> createPanel = _ui->allToolbarPanels()->itemById("SolidModifyPanel");
	if (!checkReturn(createPanel))
		return false;

	Ptr<CommandControl> gearButton = createPanel->controls()->itemById("sreliefCPPAddIn");
	if (checkReturn(gearButton))
		gearButton->deleteMe();

	Ptr<CommandDefinition> cmdDef = _ui->commandDefinitions()->itemById("sreliefCPPAddIn");
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

	CalculatedX = xmax- xmin;
	CalculatedY = ymax- ymin;
	CalculatedZ = zmax- zmin;

};

//todo scale function
void Calculate() {
	/*double scaleX = _setXDimension / _xDimension;
	double scaleY = _setYDimension / _yDimension;
	double scaleZ = _setZDimension / _zDimension;*/
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
