#include "MainFrame.h"

//temporary global variables, will figure out solution to remove eventually
bool panelFlag = true;

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	SetClientSize(800, 600);
	Center();
	Show();
	num_patrons_ = 0;
	table_order_ = {};
	restaurant_data_ = {};
	frame_ = this; //initialize frame_ to point to MainFrame to pass as a param into admin constructor
	listbox_ = nullptr;
	choice_ = nullptr;
	spin_ = nullptr;
	dialog_ = nullptr;
	hasLogin_ = false;
	seafood_ = { {{"Lobster", "Crab", "Seabass", "Tuna", "Scallops"}, {0, 0, 0, 0, 0}} };
	meat_ = { {{"Steak", "Veal", "Chicken", "Lamb", "Porkchops"}, {0, 0, 0, 0, 0}} };
	combination_ = { {{"Steak and Lobster", "Surf and Turf", "Chicken and Steak", "Shrimp over Linguini", "Steak with Shrimp"},
						{0, 0, 0, 0, 0}} };


	wxPanel* firstPanel = new wxPanel(this, wxID_ANY, wxPoint(0, 0), wxSize(800, 800));
	wxPanel* secondPanel = new wxPanel(this, wxID_ANY, wxPoint(800, 0), wxSize(750, 800));
	firstPanel->SetBackgroundColour(wxColor(0, 0, 200));
	secondPanel->SetBackgroundColour(wxColor(0, 0, 200));

	createButtons(firstPanel); //arg is wxWindows, but panel is accepted (because subclass of wxWindows maybe?)
	CreateStatusBar();

	wxButton* switchScreenButton = new wxButton(secondPanel, wxID_ANY, "Switch Screens", wxPoint(500, 700), wxSize(100, 50));
	switchScreenButton->Bind(wxEVT_BUTTON, &MainFrame::switchButtonClicked, this);

	wxButton* settingsButton = new wxButton(secondPanel, wxID_ANY, "Settings", wxPoint(650, 700), wxSize(50, 50));
	settingsButton->Bind(wxEVT_BUTTON, &MainFrame::onSettingClicked, this);

	this->Bind(wxEVT_CLOSE_WINDOW, &MainFrame::mainframeOnClose, this); //binds event when closing the window
}

/*
	Function that creates boxes only if conditions have been match, where each listbox will be binded to onListBoxClicked() on runtime
*/
void MainFrame::createListBox(wxWindow* panel) {
	int x = 150;
	int y = 50;
	for (auto it = restaurant_data_.begin(); it != restaurant_data_.end(); ++it) {
		if (it->s_has_ordered == true && it->s_food_served == false) {
			listbox_ = new wxListBox(panel, it->s_table_id, wxPoint(x, y), wxSize(120, 100), {});
			listbox_->SetBackgroundColour(wxColor(0, 0, 200));
			listbox_->InsertItems(it->s_order, 0);
			listbox_->Bind(wxEVT_LISTBOX_DCLICK, &MainFrame::onListBoxClicked, this);
			y += 140;
			if (y == 750) {
				x += 150;
				y = 50;
			}
		}
	}
}

/*
	Loop that adds inline buttons, where y(location of button) increments until reaches a certain point,
	moves to the next column of buttons.

	CreateButtons() will remember color from previous instance, if id on creation is present already in container. Every
	button is also binded on runtime, to OnButtonClick().

	NOTE: id represent table number, which is of type integer 
		  (where ID of 2 represents table 1 , ID of 3 is table 2, and so on because numbers 0 & 1 are offlimits for framework)
*/
void MainFrame::createButtons(wxWindow* panel) {
	int id = 2;
	int x = 150;
	int y = 100;
	int index = 0;
	for (int i = 0; i < 15; i++) {
		wxButton* table = new wxButton(panel, id, "", wxPoint(x, y), wxSize(75, 75));
		index = findIndexOfTable(id);

		if (index != -1) {
			if (restaurant_data_[index].s_food_served == true) {
				table->SetBackgroundColour(wxColor(255, 0, 0));
			}
			else if (restaurant_data_[index].s_has_ordered == true) {
				table->SetBackgroundColour(wxColor(255, 165, 0));
			}
			else if (restaurant_data_[index].s_has_people == true) {
				table->SetBackgroundColour(wxColor(0, 255, 0));
			}
		}
		else {
			table->SetBackgroundColour(wxColor(0, 0, 0));
		}

		table->Bind(wxEVT_BUTTON, &MainFrame::onButtonClick, this);
		id++;
		y += 120;
		if (y == 700) {
			x += 150;
			y = 100;
		}
		if (i == 14) table = nullptr;
	}
}

std::vector<int> MainFrame::getSeafoodCount() const {
	return seafood_[0].s_count;
}

std::vector<int> MainFrame::getMeatCount() const {
	return meat_[0].s_count;
}

std::vector<int> MainFrame::getCombinationCount() const {
	return combination_[0].s_count;
}

//need to change name for more clarity
int MainFrame::findIndexOfTable(int& id) const {
	int i = 0;
	for (auto it = restaurant_data_.begin(); it != restaurant_data_.end(); ++it) {
		if (it->s_table_id == id) return i;
		i++;
	}
	return -1;
}

// Function that creates a dialog, intended purpose is to take an input integer by user, then write to the table_data
// the table id, number of patrons, and if people have been sat, which is then fed into OnButtonClick event for the table
// for the purposes of changing the color of button and pushing back data.
// Returns false if s_has_people is true OR if num_patrons = 0, otherwise return true once process is completed once.
bool MainFrame::hasPatrons(int& id) {
	num_patrons_ = 0;
	int index = findIndexOfTable(id);
	if(index != -1)
		if (restaurant_data_[index].s_has_people == true) return false;

	dialog_ = new wxDialog(this, wxID_ANY, "Enter how many patrons are being sat", wxPoint(500, 300), wxDefaultSize);
	wxButton* button = new wxButton(dialog_, wxID_ANY, "Confirm", wxPoint(150, 100), wxSize(75, 50));
	spin_ = new wxSpinCtrl(dialog_, wxID_ANY, wxEmptyString, wxPoint(165, 75), wxDefaultSize, 16384L, 1, 4);
	TableData table_data;
	button->Bind(wxEVT_BUTTON, &MainFrame::updatePatronNumberOnClick, this);

	//if UpdatePatronOnClick event terminates dialog, update variables (only if successful process)
	if (dialog_->ShowModal() != wxID_OK) {
		if (num_patrons_ != 0) {
			table_data.s_table_id = id;
			table_data.s_patrons_sat = num_patrons_;
			table_data.s_has_people = true;
		}
		else {
			delete dialog_;
			dialog_ = nullptr;
			spin_ = nullptr;
			return false;
		}
	}
	restaurant_data_.push_back(std::move(table_data));
	return true;
}


// Event that updates num_patrons_ with value of spinctrl and calls dialog method EndModal() when user clicks confirm
// used to determine action during Modal of hasPatrons()
void MainFrame::updatePatronNumberOnClick(wxCommandEvent& evt) {
	num_patrons_ = spin_->GetValue();
	dialog_->EndModal(0);
	delete dialog_;
	dialog_ = nullptr;
	spin_ = nullptr;
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
	table_order_.clear();
	int index = findIndexOfTable(id);
	if (index == -1) return false;
	else if (restaurant_data_[index].s_has_ordered == true) return false;

	dialog_ = new wxDialog(this, wxID_ANY, "Order Menu", wxPoint(500, 300), wxSize(720, 400));
	wxButton* orderButton = new wxButton(dialog_, 26, "Confirm", wxPoint(300, 300), wxSize(75, 50));
	orderButton->Bind(wxEVT_BUTTON, &MainFrame::updateOrdersOnClick, this);

	if (restaurant_data_[index].s_patrons_sat == 3) {
		dialog_->SetSize(wxSize(540, 400));
		orderButton->SetSize(wxSize(75, 50));
		orderButton->SetPosition(wxPoint(200, 300));
	}
	else if (restaurant_data_[index].s_patrons_sat == 2) {
		dialog_->SetSize(wxSize(365, 400));
		orderButton->SetSize(wxSize(75, 50));
		orderButton->SetPosition(wxPoint(140, 300));
	}
	else if (restaurant_data_[index].s_patrons_sat == 1) {
		dialog_->SetSize(wxSize(190, 400));
		orderButton->SetSize(wxSize(75, 50));
		orderButton->SetPosition(wxPoint(40, 300));
	}

	int choiceId = 40;
	int x = 10;
	for (int i = 0; i < restaurant_data_[index].s_patrons_sat; i++) {
		choice_ = new wxChoice(dialog_, choiceId, wxPoint(x, 50), wxDefaultSize,
			{ "Seafood", "Meat", "Combination" }, 0L, wxDefaultValidator, "Choose category of dishes");
		choice_->Bind(wxEVT_CHOICE, &MainFrame::createOptionsOnClick, this);
		x += 175;
		choiceId++;
	}

	// if updateOrdersOnClick ends abruptly and all orders were not selected for number of patrons sat, return false
	if (dialog_->ShowModal() != wxID_OK) {
		if (table_order_.size() == restaurant_data_[index].s_patrons_sat) {
			restaurant_data_[index].s_order = table_order_;
			restaurant_data_[index].s_has_ordered = true;
		}
		else {
			delete dialog_;
			dialog_ = nullptr;
			return false;
			listbox_ = nullptr;
			choice_ = nullptr;
		}
	}
	return true;
}

/*
	Event that creates ptrs pointing to exisitng controls within mainframe
	Creates lixBoxes tied to eventId of the choicebox, where strings are retrived from respective arrays
	terminates window whenver user selects new category of food, instantiating a new window right after, reflecting choice
*/
void MainFrame::createOptionsOnClick(wxCommandEvent& evt) {
	choice_ = (wxChoice*)this->FindWindowById(evt.GetId());
	int listboxId = evt.GetId() + 5;

	if (choice_->GetSelection() == 0) {
		if (this->FindWindowById(listboxId) != NULL) this->FindWindowById(listboxId)->Destroy();
		listbox_ = new wxListBox(dialog_, listboxId, wxPoint(choice_->GetPosition().x, 150), wxDefaultSize, seafood_[0].s_array);
	}
	else if (choice_->GetSelection() == 1) {
		if (this->FindWindowById(listboxId) != NULL) this->FindWindowById(listboxId)->Destroy();
		listbox_ = new wxListBox(dialog_, listboxId, wxPoint(choice_->GetPosition().x, 150), wxDefaultSize, meat_[0].s_array);
	}
	else if (choice_->GetSelection() == 2) {
		if (this->FindWindowById(listboxId) != NULL) this->FindWindowById(listboxId)->Destroy();
		listbox_ = new wxListBox(dialog_, listboxId, wxPoint(choice_->GetPosition().x, 150), wxDefaultSize, combination_[0].s_array);
	}
}

/*
	Event that creates ptrs pointing to exisitng controls within mainframe, but event tied to "Confirm" button.
	Loop will run for how many listboxes were created, and retrieve string selection of said listbox (excluding "")
	If size of table_order_ does not match num_of_patrons_, container is cleared and event will restart when button is clicked.
	Once matched, modal will end, activates rest of AddOrderOfPatrons()
*/
void MainFrame::updateOrdersOnClick(wxCommandEvent& evt) {
	int listboxID = 45;
	int size = 0;
	listbox_ = (wxListBox*)this->FindWindowById(listboxID);
	while (listbox_ != nullptr) {
		if (listbox_->GetStringSelection() != "") {
			table_order_.Add(listbox_->GetStringSelection());
		}
		listboxID++;
		size++;
		listbox_ = (wxListBox*)this->FindWindowById(listboxID);
	}

	if (table_order_.size() != size) {
		wxLogStatus("All orders were not selected for the number of patrons present. Try again");
		table_order_.clear();
	}
	else if (table_order_.size() == size) {
		updateCountOfDishes(); //updates the number of times a dishes has been ordered this session
		dialog_->EndModal(0);
		delete dialog_;
		dialog_ = nullptr;
		listbox_ = nullptr;
		choice_ = nullptr;
	}
}

/*
	Function that will update how many dishes have been ordered this session.
	Uses two pointer of Choice and ListBox, and if neither points to nullptr, takes the value and uses it to begin searching
	through the different struct vectors and updates the corresponding index to the dish

	I.E. if Salmon is at index 0, and a customer orders Salmon, then we update/increment s_count of seafood_ at index 0 by 1
*/
void MainFrame::updateCountOfDishes() {
	int choiceId = 40;
	int listId = 45;
	choice_ = (wxChoice*)this->FindWindowById(choiceId);
	listbox_ = (wxListBox*)this->FindWindowById(listId);
	while (choice_ != nullptr) {
		if (choice_->GetSelection() == 0) {
			for (auto it = seafood_[0].s_array.begin(); it != seafood_[0].s_array.end(); ++it) {
				if (listbox_ != nullptr) {
					if (*it == listbox_->GetStringSelection()) {
						seafood_[0].s_count[it - seafood_[0].s_array.begin()] += 1;
						break;
					}
				}
			}
		}
		else if (choice_->GetSelection() == 1) {
			for (auto it = meat_[0].s_array.begin(); it != meat_[0].s_array.end(); ++it) {
				if (listbox_ != nullptr) {
					if (*it == listbox_->GetStringSelection()) {
						meat_[0].s_count[it - meat_[0].s_array.begin()] += 1;
						break;
					}
				}
			}
		}
		else if (choice_->GetSelection() == 2) {
			for (auto it = combination_[0].s_array.begin(); it != combination_[0].s_array.end(); ++it) {
				if (listbox_ != nullptr) {
					if (*it == listbox_->GetStringSelection()) {
						combination_[0].s_count[it - combination_[0].s_array.begin()] += 1;
					}
				}
			}
		}
		choiceId++;
		listId++;
		choice_ = (wxChoice*)this->FindWindowById(choiceId);
		listbox_ = (wxListBox*)this->FindWindowById(listId);
	}
}

/*
	Event that activates when double clicking selection of listbox. A modal will pop up, asking the user if the food has been served. If yes, 
	we will set the param s_food_served to true, which is linked by ID, and destroy the listbox, rearranging the UI upon deletion. The table
	color gets updated accordingly on createButtons(). 
*/
void MainFrame::onListBoxClicked(wxCommandEvent& evt) {
	int tableID = evt.GetId();
	listbox_ = (wxListBox*)this->FindWindowById(tableID);
	dialog_ = new wxDialog(this, 27, "Action Menu", wxPoint(500, 300), wxDefaultSize);
	wxStaticText* text = new wxStaticText(dialog_, wxID_ANY, "Has the food been served?", wxPoint(125, 100));
	wxButton* confirmButton = new wxButton(dialog_, wxID_ANY, "Yes", wxPoint(50, 150), wxSize(100, 50));
	wxButton* rejectionButton = new wxButton(dialog_, wxID_ANY, "No", wxPoint(250, 150), wxSize(100, 50));
	confirmButton->Bind(wxEVT_BUTTON, &MainFrame::updateCurrentOrderStatusOnClick, this);
	rejectionButton->Bind(wxEVT_BUTTON, &MainFrame::updateCurrentOrderStatusOnClick, this);

	if (dialog_->ShowModal() != wxID_OK) {
		if (restaurant_data_[findIndexOfTable(tableID)].s_food_served == true) {
			this->GetChildren()[0]->DestroyChildren();
			createListBox(this->GetChildren()[0]); //first panel
			delete dialog_;
			dialog_ = nullptr;
			listbox_ = nullptr;
			wxLogStatus("Food has been served! The new color of the table on switch is red");
		}
		else {
			delete dialog_;
			dialog_ = nullptr;
			listbox_ = nullptr;
			wxLogStatus("s_food_served was not true or listbox_ was nullptr");
		}
	}
}

/*
	Event that will either update the current order selected if it has been served or not, binded to the "Yes" and "No" buttons in the modal that
	appears durent OnListBoxClicked event. Only updates if "Yes" button has been clicked on
*/
void MainFrame::updateCurrentOrderStatusOnClick(wxCommandEvent& evt) {
	if (this->FindWindowById(evt.GetId())->GetLabel() == "Yes") {
		if (listbox_ != nullptr) {
			int tableID = listbox_->GetId();
			restaurant_data_[findIndexOfTable(tableID)].s_food_served = true;
			dialog_->EndModal(0);
		}
	}
	else {
		dialog_->EndModal(0);
	}
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
	Event that hides the current frame (MainFrame) and creates the Admin frame, passing MainFrame's ptr to admin
	to later be used to display MainFrame once the work in Admin is done.
*/
void MainFrame::onSettingClicked(wxCommandEvent& evt) {
	Hide();
	Admin* admin = new Admin("login", frame_);
	admin->Show();
}

void MainFrame::mainframeOnClose(wxCloseEvent& evt) {
	Admin* admin = new Admin("", frame_);
	if (admin->hasDatabase()) {
		admin->setDataIntoDatabase(getSeafoodCount(), getMeatCount(), getCombinationCount());
		this->Destroy(); //again bad use(unsure why Close() doesn't work), but closes window completely
	}
	else {
		admin->Destroy();
		this->Destroy();
	}
}