#include "DiningRoom.h"

// Event that updates num_patrons_ with value of spinctrl and calls dialog method EndModal() when user clicks confirm
// used to determine action during Modal of hasPatrons()
void DiningRoom::updatePatronNumberOnClick(wxCommandEvent& evt) {
	num_patrons_ = spin_->GetValue();
	dialog_->EndModal(0);
	delete dialog_;
	dialog_ = nullptr;
	spin_ = nullptr;
}

/*
	Event that creates ptrs pointing to exisitng controls within mainframe
	Creates lixBoxes tied to eventId of the choicebox, where strings are retrived from respective arrays
	terminates window whenver user selects new category of food, instantiating a new window right after, reflecting choice
	*Destroys other windows if it exist unpon selecting new choice
*/
void DiningRoom::createFoodOptionsOnSelection(wxCommandEvent& evt) {
	choice_ = (wxChoice*)this->FindWindowById(evt.GetId());
	wxArrayString choices = {};
	if (this->FindWindowById(evt.GetId() + 5) != NULL) this->FindWindowById(evt.GetId() + 5)->Destroy();
	if (this->FindWindowById(evt.GetId() + 10) != NULL) this->FindWindowById(evt.GetId() + 10)->Destroy();
	if (this->FindWindowById(evt.GetId() + 15) != NULL) this->FindWindowById(evt.GetId() + 15)->Destroy();

	if (choice_->GetSelection() == 0) {
		choices.Clear();
		for (auto it = seafood_.begin(); it != seafood_.end(); ++it) {
			choices.push_back(it->first);
		}
		listbox_ = new wxListBox(dialog_, evt.GetId() + 5, wxPoint(choice_->GetPosition().x, 150), wxDefaultSize, choices);
		wxLogStatus("Seafood options displayed!");
	}
	else if (choice_->GetSelection() == 1) {
		choices.Clear();
		for (auto it = meat_.begin(); it != meat_.end(); ++it) {
			choices.push_back(it->first);
		}
		listbox_ = new wxListBox(dialog_, evt.GetId() + 5, wxPoint(choice_->GetPosition().x, 150), wxDefaultSize, choices);
		wxLogStatus("Meat options displayed!");
	}
	else if (choice_->GetSelection() == 2) {
		choices.Clear();
		for (auto it = combination_.begin(); it != combination_.end(); ++it) {
			choices.push_back(it->first);
		}
		listbox_ = new wxListBox(dialog_, evt.GetId() + 5, wxPoint(choice_->GetPosition().x, 150), wxDefaultSize, choices);
		wxLogStatus("Combination options displayed!");
	}
	listbox_->Bind(wxEVT_LISTBOX, &DiningRoom::createOptionsOnSelection, this);
}

// Id of choice is always 5 behind the listbox Id during the modal window of taking orders
void DiningRoom::createOptionsOnSelection(wxCommandEvent& evt) {
	listbox_ = (wxListBox*)this->FindWindowById(evt.GetId());
	choice_ = (wxChoice*)this->FindWindowById(evt.GetId() - 5);
	if (this->FindWindowById(evt.GetId() + 5) != NULL) this->FindWindowById(evt.GetId() + 5)->Destroy();
	if (this->FindWindowById(evt.GetId() + 10) != NULL) this->FindWindowById(evt.GetId() + 10)->Destroy();

	if (choice_->GetSelection() == 0) {
		if (seafood_.find(listbox_->GetStringSelection())->second.s_dish_type == "Shellfish" && seafood_.find(listbox_->GetStringSelection())->second.isChoiceMandatory == true) {
			choice_ = new wxChoice(dialog_, evt.GetId() + 5, wxPoint(listbox_->GetPosition().x, 250), wxDefaultSize,
				{ "Cooking method", "Other" }, 0L, wxDefaultValidator, "Options");
			wxLogStatus("Displaying actions needed for lobster");
		}
	}
	else if (choice_->GetSelection() == 1) {
		if (meat_.find(listbox_->GetStringSelection())->second.s_dish_type == "Beef" && meat_.find(listbox_->GetStringSelection())->second.isChoiceMandatory == true) {
			choice_ = new wxChoice(dialog_, evt.GetId() + 5, wxPoint(listbox_->GetPosition().x, 250), wxDefaultSize,
				{ "Temperature", "Other" }, 0L, wxDefaultValidator, "Options");
			wxLogStatus("Displaying actions needed for steak");
		}
	}
	else {
		wxLogStatus("Nothing to be done");
	}
	choice_->Bind(wxEVT_CHOICE, &DiningRoom::createSubOptionsOnSelection, this);
}

void DiningRoom::createSubOptionsOnSelection(wxCommandEvent& evt) {
	if (this->FindWindowById(evt.GetId() + 5) != NULL) this->FindWindowById(evt.GetId() + 5)->Destroy();
	choice_ = (wxChoice*)this->FindWindowById(evt.GetId());

	if (choice_->GetStringSelection() == "Temperature") {
		listbox_ = new wxListBox(dialog_, evt.GetId() + 5, wxPoint(choice_->GetPosition().x, 300), wxDefaultSize, { "Rare", "Medium Rare", "Welldone" });
		wxLogStatus("Showing options for Temperature");
	}
	else if (choice_->GetStringSelection() == "Cooking method") {
		listbox_ = new wxListBox(dialog_, evt.GetId() + 5, wxPoint(choice_->GetPosition().x, 300), wxDefaultSize, { "Steamed", "Broiled" });
		wxLogStatus("Showing options for Cooking method");
	}
	else if (choice_->GetStringSelection() == "Other") {
		choice_ = (wxChoice*)this->FindWindowById(evt.GetId() - 10);
		if (choice_->GetSelection() == 0) {
			listbox_ = new wxListBox(dialog_, evt.GetId() + 5, wxPoint(choice_->GetPosition().x, 300), wxDefaultSize, { "No butter", "Extra butter", "No lemons", "Extra lemons", "Plain" });
			wxLogStatus("Options for shellfish displayed");
		}
		else if (choice_->GetSelection() == 1) {
			listbox_ = new wxListBox(dialog_, evt.GetId() + 5, wxPoint(choice_->GetPosition().x, 300), wxDefaultSize, { "No salt", "No butter", "Sliced" });
			wxLogStatus("Options for beef displayed");
		}
	}
	listbox_->Bind(wxEVT_LISTBOX, &DiningRoom::storeOptionsFromSelection, this);
}

// The original choice id starts at 40, where we only create 3 more controls using increments of 5 on the choice id
// modulo will return the remainder, which will act as the index of s_options (corresponding to each person)
void DiningRoom::storeOptionsFromSelection(wxCommandEvent& evt) {
	listbox_ = (wxListBox*)this->FindWindowById(evt.GetId());
	choice_ = (wxChoice*)this->FindWindowById(evt.GetId() - 5);
	if (choice_->GetSelection() == 0 && this->FindWindowById(evt.GetId() - 15) != NULL) {
		int patron = (evt.GetId() - 15) % 10;
		int option = choice_->GetSelection();
		restaurant_data_[cur_table_index_].s_options[patron][option] = listbox_->GetStringSelection();
		wxLogStatus("option1 saved");
	}
	else if (choice_->GetSelection() == 1 && this->FindWindowById(evt.GetId() - 15) != NULL) {
		int patron = (evt.GetId() - 15) % 10;
		int option = choice_->GetSelection();
		restaurant_data_[cur_table_index_].s_options[patron][option] = listbox_->GetStringSelection();
		wxLogStatus("option2 saved");
	}
}

/*
	Event that creates ptrs pointing to exisitng controls within mainframe, but event tied to "Confirm" button.
	Loop will run for how many listboxes were created, and retrieve string selection of said listbox (excluding "")
	If size of table_order_ does not match num_of_patrons_, container is cleared and event will restart when button is clicked.
	Once matched, modal will end, activates rest of AddOrderOfPatrons()
*/
void DiningRoom::updateOrdersOnClick(wxCommandEvent& evt) {
	wxArrayString table_order;
	int choiceID = ID_CHOICE_BASE;
	int listboxID = ID_LISTBOX_BASE;
	int size = 0; //also acts as the index (if listbox is not nullptr, then value matches index of the person's order)
	int offset = 0; // offset is the actual suboption selected for the patron
	listbox_ = (wxListBox*)this->FindWindowById(listboxID);
	choice_ = (wxChoice*)this->FindWindowById(choiceID);
	while (choice_ != nullptr) {
		if (listbox_ != nullptr && listbox_->GetStringSelection() != "") {
			wxString order = listbox_->GetStringSelection();
			table_order.Add(order);
			if (restaurant_data_[cur_table_index_].s_options[size][offset] != "") {
				order = "  -" + restaurant_data_[cur_table_index_].s_options[size][offset];
				table_order.Add(order);
				offset++;
			}
			if (restaurant_data_[cur_table_index_].s_options[size][offset] != "") {
				order = "  -" + restaurant_data_[cur_table_index_].s_options[size][offset];
				table_order.Add(order);
				offset++;
			}
		}
		choiceID++;
		listboxID++;
		size++;
		listbox_ = (wxListBox*)this->FindWindowById(listboxID);
		choice_ = (wxChoice*)this->FindWindowById(choiceID);
	}
	if (table_order.size() - offset != size) {
		wxLogStatus("All orders were not selected for the number of patrons present. Try again");
		table_order.clear();
	}
	else if (table_order.size() - offset == size) {
		restaurant_data_[cur_table_index_].s_order = std::move(table_order);
		restaurant_data_[cur_table_index_].s_has_ordered = true;
		table_order.clear();
		updateCountOfDishes(); //updates the number of times a dishes has been ordered this session
		dialog_->EndModal(0);
		delete dialog_;
		dialog_ = nullptr;
		listbox_ = nullptr;
		choice_ = nullptr;
	}
}

/*
	Event that activates when double clicking selection of listbox. A modal will pop up, asking the user if the food has been served. If yes,
	we will set the param s_food_served to true, which is linked by ID, and destroy the listbox, rearranging the UI upon deletion. The table
	color gets updated accordingly on createButtons().
*/
void DiningRoom::onListBoxClicked(wxCommandEvent& evt) {
	int tableID = evt.GetId();
	listbox_ = (wxListBox*)this->FindWindowById(tableID);
	dialog_ = new wxDialog(this, ID_ACTION_DIALOG, "Action Menu", wxPoint(500, 300), wxDefaultSize);
	wxStaticText* text = new wxStaticText(dialog_, wxID_ANY, "Has the food been served?", wxPoint(125, 100));
	wxButton* confirmButton = new wxButton(dialog_, wxID_ANY, "Yes", wxPoint(50, 150), wxSize(100, 50));
	wxButton* rejectionButton = new wxButton(dialog_, wxID_ANY, "No", wxPoint(250, 150), wxSize(100, 50));
	confirmButton->Bind(wxEVT_BUTTON, &DiningRoom::updateCurrentOrderStatusOnClick, this);
	rejectionButton->Bind(wxEVT_BUTTON, &DiningRoom::updateCurrentOrderStatusOnClick, this);

	if (dialog_->ShowModal() != wxID_OK) {
		if (restaurant_data_[findIndexOfTable(tableID)].s_food_served == true) {
			this->GetChildren()[0]->DestroyChildren();
			createListBox(this->GetChildren()[0]); // first panel from mainframe 
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
void DiningRoom::updateCurrentOrderStatusOnClick(wxCommandEvent& evt) {
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
void DiningRoom::switchButtonClicked(wxCommandEvent& evt) {
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
void DiningRoom::onButtonClick(wxCommandEvent& evt) {
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
void DiningRoom::onSettingClicked(wxCommandEvent& evt) {
	Hide();
	Admin* admin = new Admin("login", frame_);
	if (admin->hasDatabase()) {
		setDataIntoDatabase(seafood_, meat_, combination_);
	}
	admin->Show();
}

void DiningRoom::mainframeOnClose(wxCloseEvent& evt) {
	Admin* admin = new Admin("", frame_);
	if (admin->hasDatabase()) {
		setDataIntoDatabase(seafood_, meat_, combination_);
	}
	admin->Destroy();
	this->Destroy();

}