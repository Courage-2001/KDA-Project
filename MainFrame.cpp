#include "MainFrame.h"

//temporary global variables, will figure out solution to remove eventually
bool panelFlag = true;

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	num_patrons_ = 0;
	event_container_ = {};
	container_ = {};
	listbox_ = nullptr;
	choice_ = nullptr;
	spin_ = nullptr;
	dialog_ = nullptr;
	seafood_ = { "Lobster", "Crab", "Seabass", "Tuna", "Scallops" };
	meat_ = { "Steak", "Veal", "Chicken", "Lamb", "Porkchops" };
	combination_ = { "Steak and Lobster", "Surf and Turf", "Chicken and Steak", "Shrimp over Linguini", "Steak with Shrimp" };

	wxPanel* firstPanel = new wxPanel(this, wxID_ANY, wxPoint(0, 0), wxSize(800,800));
	wxPanel* secondPanel = new wxPanel(this, wxID_ANY, wxPoint(800, 0), wxSize(750, 800));
	firstPanel->SetBackgroundColour(wxColor(0, 0, 200));
	secondPanel->SetBackgroundColour(wxColor(0, 0, 200));


	createButtons(firstPanel); //arg is wxWindows, but panel is accepted (because subclass of wxWindows maybe?)
	CreateStatusBar();

	wxButton* switchScreenButton = new wxButton(secondPanel, wxID_ANY, "Switch Screens", wxPoint(500, 700), wxSize(100, 50));
	switchScreenButton->Bind(wxEVT_BUTTON, &MainFrame::switchButtonClicked, this);

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
void MainFrame::createListBox(wxWindow* panel) {
	//Unique id = Table1;
	int id = 2;
	int x = 150;
	int y = 50;
	int index = 0;
	for (int i = 0; i < 15; i++) {
		index = findIndex(id);
		new wxListBox(panel, id, wxPoint(x, y), wxSize(120, 100), {});

		if (index != -1) {
			if (getContainer()[index].s_has_ordered == true) {
				listbox_ = (wxListBox*)panel->GetChildren()[i];
				listbox_->InsertItems(getContainer()[index].s_order, 0);
			}
		}

		panel->GetChildren()[i]->SetBackgroundColour(wxColor(0, 0, 200));
		id++;
		y += 140;
		if (y == 750) {
			x += 150;
			y = 50;
		}
	}
}


void MainFrame::createButtons(wxWindow* panel) {
	//Unique id = Table1;
	int id = 2;
	int x = 150;
	int y = 100;
	int index = 0;
	for (int i = 0; i < 15; i++) {
		new wxButton(panel, id, "", wxPoint(x, y), wxSize(75, 75));
		index = findIndex(id);

		if (index != -1) {
			if (getContainer()[index].s_has_ordered == true) {
				panel->GetChildren()[i]->SetBackgroundColour(wxColor(255, 165, 0));
			}
			else if (getContainer()[index].s_has_people == true) {
				panel->GetChildren()[i]->SetBackgroundColour(wxColor(0, 255, 0));
			}
		}
		else {
			panel->GetChildren()[i]->SetBackgroundColour(wxColor(0, 0, 0));
		}

		panel->GetChildren()[i]->Bind(wxEVT_BUTTON, &MainFrame::onButtonClick, this);
		id++;
		y += 120;
		if (y == 700) {
			x += 150;
			y = 100;
		}
	}
}

/*
	Function that creates a dialog, intended purpose is to take an input integer by user, then write to the temp dataset
	id, number of patrons, and people_sat_, which is then fed into OnButtonClick event, 
	changing color of button and pushing back data.

	Returns false if people_sat_ is true, otherwise return true once process is completed once.
*/
bool MainFrame::hasPatrons(int& id) {
	int index = findIndex(id);
	if(index != -1)
		if (container_[index].s_has_people == true) return false;

	wxDialog* dialog = new wxDialog(this, 25, "Enter how many patrons are being sat", wxPoint(500, 300), wxDefaultSize);
	wxButton* button = new wxButton(dialog, wxID_ANY, "Confirm", wxPoint(150, 100), wxSize(75, 50));
	wxSpinCtrl* spinCtrl = new wxSpinCtrl(dialog, 50, wxEmptyString, wxPoint(165, 75), wxDefaultSize, 16384L, 1, 4);
	dataset temp = { 0, {}, 0, false, false};
	button->Bind(wxEVT_BUTTON, &MainFrame::updatePatronNumberOnClick, this);

	//if UpdatePatronOnClick event terminates dialog, update variables
	if (dialog->ShowModal() != wxID_OK) {
		temp.s_id = id;
		temp.s_patrons_sat = num_patrons_;
		temp.s_has_people = true;
	}
	
	container_.push_back(temp);
	return true;
}

/*
	Function that uses another dialog to activate function process.
	Creates a dialog modal that containers a choicebox of food category, based on how many people were sat at the table.
	All choice boxes created are binded to event related to AddOrderOfPatrons.
	Once selected, event tied to function creates listBoxes related to category selected, and once the modal ends,
	updates the index of the container that matches the table's id, and changes color on pressing confirm.

	Return false if order_placed_ is true, otherwise return true once process is completed once.
*/
bool MainFrame::hasOrders(int& id) {
	event_container_.clear();
	int index = findIndex(id);
	if (container_[index].s_has_ordered == true) return false;

	int x = 10;
	int tempId = 40;
	wxDialog* dialog = new wxDialog(this, 27, "Order Menu", wxPoint(500, 300), wxSize(720, 400));
	wxButton* orderButton = new wxButton(dialog, 26, "Confirm", wxPoint(300, 300), wxSize(75, 50));
	orderButton->Bind(wxEVT_BUTTON, &MainFrame::updateOrdersOnClick, this);

	if (container_[index].s_patrons_sat == 3) {
		dialog->SetSize(wxSize(540, 400));
		orderButton->SetSize(wxSize(75, 50));
		orderButton->SetPosition(wxPoint(200, 300));
	}
	else if (container_[index].s_patrons_sat == 2) {
		dialog->SetSize(wxSize(365, 400));
		orderButton->SetSize(wxSize(75, 50));
		orderButton->SetPosition(wxPoint(140, 300));
	}
	else if (container_[index].s_patrons_sat == 1) {
		dialog->SetSize(wxSize(190, 400));
		orderButton->SetSize(wxSize(75, 50));
		orderButton->SetPosition(wxPoint(40, 300));
	}


	for (int i = 0; i < container_[index].s_patrons_sat; i++) {
		new wxChoice(dialog, tempId, wxPoint(x, 50), wxDefaultSize,
			{ "Seafood", "Meat", "Combination" }, 0L, wxDefaultValidator, "Choose category of dishes");
		choice_ = (wxChoice*)this->FindWindowById(tempId);
		choice_->Bind(wxEVT_CHOICE, &MainFrame::createOptionsOnClick, this);
		x += 175;
		tempId++;
	}

	if (dialog->ShowModal() != wxID_OK) {
		container_[index].s_order = event_container_;
		container_[index].s_has_ordered = true;
		event_container_.clear();
	}
	return true;
}

int MainFrame::findIndex(int& id) const {
	int i = 0;
	for (auto it = container_.begin(); it != container_.end(); ++it) {
		if (it->s_id == id) return i;
		i++;
	}
	return -1;
}

std::vector<MainFrame::dataset> MainFrame::getContainer() const {
	return container_;
}

wxArrayString MainFrame::getSeafood() const {
	return seafood_;
}

wxArrayString MainFrame::getMeat() const {
	return meat_;
}

wxArrayString MainFrame::getCombination() const {
	return combination_;
}


/*
	Uses global flag to determine which function to use on button click.
	Destroys all current children of panel, creates new children on each instance
	Sets flag to opposite value on each click
*/
void MainFrame::switchButtonClicked(wxCommandEvent& evt) {

	if (panelFlag == false) {
		this->GetChildren()[0]->DestroyChildren();
		createButtons(this->GetChildren()[0]);
		panelFlag = true;
		wxLogStatus("Buttons created");
	}
	else if (panelFlag == true) {
		this->GetChildren()[0]->DestroyChildren();
		createListBox(this->GetChildren()[0]);
		panelFlag = false;
		wxLogStatus("Listboxes created");
	}
}

/*
	All buttons created on firstPanel, are binded on runtime. 
	If AddNumberOfPatrons is true, change color to green
	If AddOrderOfPatrons is true, change color to orange
	else, do nothing
*/
void MainFrame::onButtonClick(wxCommandEvent& evt) {
	int buttonID = evt.GetId();
	if (hasPatrons(buttonID) == true) {
		this->FindWindowById(buttonID)->SetBackgroundColour(wxColor(0, 255, 0));
		wxLogStatus("Color of button has changed, ID and number of patrons have been updated and inserted in container");
	}
	else if (hasOrders(buttonID) == true) {
		this->FindWindowById(buttonID)->SetBackgroundColour(wxColor(255, 165, 0));
		wxLogStatus("Order has been placed! (id exist, and order pushed onto container");
	}
	else
		wxLogStatus("Nothing to be done here");
}

/*
	Event that creates ptrs pointing to existing controls within mainframe
	Updates global var with value of spinctrl, and calls dialog method EndModal() when user clicks confirm,
	which will activate rest of AddNumberOfPatrons()
*/
void MainFrame::updatePatronNumberOnClick(wxCommandEvent& evt) {
	spin_ = (wxSpinCtrl *)this->FindWindowById(50);
	dialog_ = (wxDialog*)this->FindWindowById(25);
	num_patrons_ = spin_->GetValue();

	dialog_->EndModal(0);
	delete dialog_;
	dialog_ = nullptr;
}

/*
	Event that creates ptrs pointing to exisitng controls within mainframe
	Creates lixBoxes tied to eventId of the choicebox, where strings are retrived from respective arrays
	terminates window whenver user selects new category of food, instantiating a new window right after, reflecting choice
*/
void MainFrame::createOptionsOnClick(wxCommandEvent& evt) {
	choice_ = (wxChoice*)this->FindWindowById(evt.GetId());
	int tempId = evt.GetId() + 5;
	dialog_ = (wxDialog*)this->FindWindowById(27);

	if (choice_->GetSelection() == 0) {
		if(this->FindWindowById(tempId) != NULL) this->FindWindowById(tempId)->Destroy();
		listbox_ = new wxListBox(dialog_, tempId, wxPoint(choice_->GetPosition().x, 150), wxDefaultSize, getSeafood());
	}
	else if (choice_->GetSelection() == 1) {
		if (this->FindWindowById(tempId) != NULL) this->FindWindowById(tempId)->Destroy();
		listbox_ = new wxListBox(dialog_, tempId, wxPoint(choice_->GetPosition().x, 150), wxDefaultSize, getMeat());
	}
	else if (choice_->GetSelection() == 2) {
		if (this->FindWindowById(tempId) != NULL) this->FindWindowById(tempId)->Destroy();
		listbox_ = new wxListBox(dialog_, tempId, wxPoint(choice_->GetPosition().x, 150), wxDefaultSize, getCombination());
	}
}

/*
	Event that creates ptrs pointing to exisitng controls within mainframe, but event tied to "Confirm" button.
	Loop will run for how many listboxes were created, and retrieve string selection of said listbox (excluding "")
	If size of event_container_ does not match num_of_patrons_, container is cleared and event will restart when button is clicked.
	Once matched, modal will end, activates rest of AddOrderOfPatrons()
*/
void MainFrame::updateOrdersOnClick(wxCommandEvent& evt) {
	dialog_ = (wxDialog*)this->FindWindowById(27);
	int tempID = 45;
	for (int i = 0; i < num_patrons_; i++) {
		listbox_ = (wxListBox*)this->FindWindowById(tempID);
		if (listbox_ == nullptr) break;
		else if(listbox_->GetStringSelection() != "") //BUG: if no choice is selected, and you press confirm, error thrown (RESOLVED)
			event_container_.Add(listbox_->GetStringSelection());
		tempID++;
	}

	if (event_container_.size() != num_patrons_) {
		wxLogStatus("All orders were not selected for the number of patrons present. Try again");
		event_container_.clear();
	}
	else if(event_container_.size() ==  num_patrons_) {
		dialog_->EndModal(0);
		delete dialog_;
		dialog_ = nullptr;
	}
}

//not being used, may delete eventually
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