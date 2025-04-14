#include "MainFrame.h"
//#include <wx/wx.h>
#include <wx/editlbox.h>

//temporary global variables, will figure out solution to remove eventually
bool panelFlag = true;
int patronNumber = 0;

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	wxPanel* firstPanel = new wxPanel(this, wxID_ANY, wxPoint(0, 0), wxSize(800,800));
	wxPanel* secondPanel = new wxPanel(this, wxID_ANY, wxPoint(800, 0), wxSize(750, 800));
	firstPanel->SetBackgroundColour(wxColor(0, 0, 200));
	secondPanel->SetBackgroundColour(wxColor(0, 0, 200));


	CreateButtons(firstPanel); //arg is wxWindows, but panel is accepted (because subclass of wxWindows maybe?)
	CreateStatusBar();

	wxButton* switchScreenButton = new wxButton(secondPanel, wxID_ANY, "Switch Screens", wxPoint(500, 700), wxSize(100, 50));
	//may remove listbox for some sort of choice box instead
	//wxEditableListBox* listBox = new wxEditableListBox(secondPanel, wxID_ANY, "Enter entries here", wxPoint(0, 100), wxSize(500, 600));
	switchScreenButton->Bind(wxEVT_BUTTON, &MainFrame::SwitchButtonClicked, this);

}

//may change later into two seperate functions
void MainFrame::WriteAndRead(wxListBox* box) {
}

/*
	Loops that adds inline buttons, where y(location of button) increments until reaches a certain point,
	moves to the next column of buttons.

	CreateButtons() will remember color from previous instance, if id on creation is present already in container. Every
	button is also binded on runtime, to OnButtonClick(). 

	NOTE: Unique enum does not work when retrieving from wxWidgets functions, must figure out alternative. 
		  For now, id of type inter is to represent table number 
		  (where ID of 2 represents table 1 , ID of 3 is table 2, and so on because numbers 0 & 1 are offlimits for framework)
*/
void MainFrame::CreateListBox(wxWindow* panel) {
	//Unique id = Table1;
	int id = 2;
	int x = 150;
	int y = 20;
	for (int i = 0; i < 15; i++) {
		new wxListBox(panel, id, wxPoint(x, y), wxSize(100, 150), {"hello", "bye"});
		panel->GetChildren()[i]->SetBackgroundColour(wxColor(0, 0, 200));
		id++;
		//SwitchID(id);
		y += 200;
		if (y == 1020) {
			x += 150;
			y = 20;
		}
	}
}

void MainFrame::CreateButtons(wxWindow* panel) {
	//Unique id = Table1;
	int id = 2;
	int x = 150;
	int y = 100;
	for (int i = 0; i < 15; i++) {
		new wxButton(panel, id, "", wxPoint(x, y), wxSize(75, 75));

		if(FindIdOfButton(id)) panel->GetChildren()[i]->SetBackgroundColour(wxColor(0, 255, 0));
		else panel->GetChildren()[i]->SetBackgroundColour(wxColor(0, 0, 0));

		panel->GetChildren()[i]->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClick, this);
		id++;
		y += 120;
		if (y == 700) {
			x += 150;
			y = 100;
		}
	}
}

/*
	Function that creates a dialog, intended purpose is to take an input integer by user, then write to the container
	id and number of patrons, which is then fed into OnButtonClick event, changing color of button and pushing back data.

	returns true if id is not present in container, false if id is present in container already
*/
bool MainFrame::AddNumberOfPatrons(int& id) {
	if (FindIdOfButton(id)) return false;

	wxDialog* dialog = new wxDialog(this, 25, "Enter how many patrons are being sat", wxPoint(500, 300), wxDefaultSize);
	wxButton* button = new wxButton(dialog, wxID_ANY, "Confirm", wxPoint(150, 100), wxSize(75, 50));
	wxSpinCtrl* spinCtrl = new wxSpinCtrl(dialog, 50, wxEmptyString, wxPoint(165, 75), wxDefaultSize, 16384L, 1, 4);
	dataset temp = { 0, {}, 0 };
	button->Bind(wxEVT_BUTTON, &MainFrame::UpdatePatronNumberOnClick, this);

	//if UpdatePatronOnClick event terminates dialog, update variables
	if (dialog->ShowModal() != wxID_OK) {
		temp.id_ = id;
		temp.numOfPatrons_ = patronNumber;
	}
	
	container_.push_back(temp);
	return true;
}

//need to incoporate orderButton with a new event that will take in all listBoxes choices, and will move forward
//with only when all boxes have a choice selected, then push into dataset and delete all children of secondPanel (excluding switch)
//need to figure out how bool flag will work for AddOrderOfPatrons() when in tandem with OnButtonClick() !
bool MainFrame::AddOrderOfPatrons(int& id) {
	int x = 200;
	int tempId = 40;
	int index = FindIndexOfId(id);
	wxChoice* choicePtr = nullptr;
	wxButton* orderButton = new wxButton(this->GetChildren()[1], 26, "Confirm", wxPoint(300, 500), wxSize(75, 50));

	for (int i = 0; i < GetContainer()[index].numOfPatrons_; i++) {
		new wxChoice(this->GetChildren()[1], tempId, wxPoint(x, 200), wxDefaultSize,
			{ "Seafood", "Meat", "Combination" }, 0L, wxDefaultValidator, "Choose category of dishes");
		choicePtr = (wxChoice*)this->FindWindowById(tempId);
		choicePtr->Bind(wxEVT_CHOICE, &MainFrame::CreateOptionsOnClick, this);
		x += 100;
		tempId++;
	}
	return true;
}

bool MainFrame::FindIdOfButton(int& id) {
	for (auto it = container_.begin(); it != container_.end(); ++it) {
		if (it->id_ == id)
			return true;
	}
	return false;
}

int MainFrame::FindIndexOfId(int& id) const {
	int i = 0;
	for (auto it = container_.begin(); it != container_.end(); ++it) {
		if (it->id_ == id) return i;
		i++;
	}
	return -1;
}

std::vector<MainFrame::dataset> MainFrame::GetContainer() const {
	return container_;
}

wxArrayString MainFrame::GetSeafood() const {
	return seafood_;
}

wxArrayString MainFrame::GetMeat() const {
	return meat_;
}

wxArrayString MainFrame::GetCombination() const {
	return combination_;
}


/*
	Uses global flag to determine which function to use on button click.
	Destroys all current children of panel, creates new children on each instance
	Sets flag to opposite value on each click
*/
void MainFrame::SwitchButtonClicked(wxCommandEvent& evt) {

	if (panelFlag == false) {
		this->GetChildren()[0]->DestroyChildren();
		CreateButtons(this->GetChildren()[0]);
		panelFlag = true;
		wxLogStatus("Buttons created");
	}
	else if (panelFlag == true) {
		this->GetChildren()[0]->DestroyChildren();
		CreateListBox(this->GetChildren()[0]);
		panelFlag = false;
		wxLogStatus("Listboxes created");
	}
}

/*
	All buttons created on firstPanel, are binded on runtime. If AddNumberOfPatrons is true, change color. If false, do nothing
*/
void MainFrame::OnButtonClick(wxCommandEvent& evt) {
	int buttonID = evt.GetId();
	if (AddNumberOfPatrons(buttonID)) {
		this->FindWindowById(buttonID)->SetBackgroundColour(wxColor(0, 255, 0));
		wxLogStatus("Color of button has changed, ID and number of patrons have been updated and inserted in container");
	}
	else
		if (AddOrderOfPatrons(buttonID)) {
			this->FindWindowById(buttonID)->SetBackgroundColour(wxColor(255, 165, 0));
			wxLogStatus("Order has been placed! (id exist, and order pushed onto container");
		}
}

/*
	Event that creates ptrs pointing to existing controls within mainframe
	Updates global var with value of spinctrl, and calls dialog method EndModal() when user clicks confirm
*/
void MainFrame::UpdatePatronNumberOnClick(wxCommandEvent& evt) {
	wxSpinCtrl* tempPtr = (wxSpinCtrl *)this->FindWindowById(50);
	wxDialog* exitPtr = (wxDialog*)this->FindWindowById(25);
	patronNumber = tempPtr->GetValue();

	delete tempPtr;
	tempPtr = nullptr;
	exitPtr->EndModal(0);
	delete exitPtr;
	exitPtr = nullptr;
}

void MainFrame::CreateOptionsOnClick(wxCommandEvent& evt) {
	wxChoice* tempPtr = (wxChoice*)this->FindWindowById(evt.GetId());
	int tempId = evt.GetId() + 5;

	if (tempPtr->GetSelection() == 0) {
		if(this->FindWindowById(tempId) != NULL) this->FindWindowById(tempId)->Destroy();
		wxListBox* seafoodBox = new wxListBox(this->GetChildren()[1], tempId, wxPoint(tempPtr->GetPosition().x, 350), wxDefaultSize, GetSeafood());
	}
	else if (tempPtr->GetSelection() == 1) {
		if (this->FindWindowById(tempId) != NULL) this->FindWindowById(tempId)->Destroy();
		wxListBox* meatBox = new wxListBox(this->GetChildren()[1], tempId, wxPoint(tempPtr->GetPosition().x, 350), wxDefaultSize, GetMeat());
	}
	else if (tempPtr->GetSelection() == 2) {
		if (this->FindWindowById(tempId) != NULL) this->FindWindowById(tempId)->Destroy();
		wxListBox* combinationBox = new wxListBox(this->GetChildren()[1], tempId, wxPoint(tempPtr->GetPosition().x, 350), wxDefaultSize, GetCombination());
	}
}

MainFrame::Unique MainFrame::SwitchID(Unique& id) {
	switch (id) 
	{
	case Table1: 
		id = Table2;
	case Table2:
		id = Table3;
	case Table3:
		id = Table4;
	case Table4:
		id = Table5;
	case Table5:
		id = Table6;
	case Table6:
		id = Table7;
	case Table7:
		id = Table8;
	case Table8:
		id = Table9;
	case Table9:
		id = Table10;
	case Table10:
		id = Table11;
	case Table11:
		id = Table12;
	case Table12:
		id = Table13;
	case Table13:
		id = Table14;
	case Table14:
		id = Table15;
	}
	return id;
}