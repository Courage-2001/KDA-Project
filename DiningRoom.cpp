#include "DiningRoom.h"

DiningRoom::DiningRoom(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	SetClientSize(800, 600);
	Center();
	Show();
	num_patrons_ = 0;
	cur_table_index_ = -1;
	restaurant_data_ = {};
	frame_ = this; //initialize frame_ to point to MainFrame to pass as a param into admin constructor
	listbox_ = nullptr;
	choice_ = nullptr;
	spin_ = nullptr;
	dialog_ = nullptr;
	hasLogin_ = false;
	panelFlag = true;
	seafood_ = { {"Crab", {0, "Shellfish", false}}, {"Lobster", {0, "Shellfish", true}}, {"Scallops", {0, "Shellfish", false}}, {"Seabass", {0, "Fish", false}}, {"Tuna", {0, "Fish", false}} };
	meat_ = { {"Chicken",{0, "Poultry", false}}, {"Lamb",{0, "Mutton", false}}, {"Porkchops",{0, "Pork", false}}, {"Steak",{0, "Beef", true}}, {"Veal",{0, "Beef", false}} };
	combination_ = { {"Chicken and Steak",{0, "Beef&Poultry", true}}, {"Shrimp over Linguini",{0, "Shellfish&Pasta", false}}, 
		{"Steak and Lobster",{0, "Beef&Shellfish", true}}, {"Steak with Shrimp",{0, "Beef&Shellfish", true}}, {"Surf and Turf",{0, "Beef&Shellfish", true}} };


	wxPanel* firstPanel = new wxPanel(this, wxID_ANY, wxPoint(0, 0), wxSize(800, 800));
	wxPanel* secondPanel = new wxPanel(this, wxID_ANY, wxPoint(800, 0), wxSize(750, 800));
	firstPanel->SetBackgroundColour(wxColor(0, 0, 200));
	secondPanel->SetBackgroundColour(wxColor(0, 0, 200));

	createButtons(firstPanel);
	CreateStatusBar();

	wxButton* switchScreenButton = new wxButton(secondPanel, wxID_ANY, "Switch Screens", wxPoint(500, 700), wxSize(100, 50));
	switchScreenButton->Bind(wxEVT_BUTTON, &DiningRoom::switchButtonClicked, this);

	wxButton* settingsButton = new wxButton(secondPanel, wxID_ANY, "Settings", wxPoint(650, 700), wxSize(50, 50));
	settingsButton->Bind(wxEVT_BUTTON, &DiningRoom::onSettingClicked, this);

	this->Bind(wxEVT_CLOSE_WINDOW, &DiningRoom::mainframeOnClose, this); //binds event when closing the window
}

// Function that creates boxes only if conditions have been match, where each listbox will be binded to onListBoxClicked() on runtime
// For the moment, If s_options exceed the size of the listbox, it does not display due to size constraints. 
void DiningRoom::createListBox(wxWindow* panel) {
	int x = 150;
	int y = 50;
	for (auto it = restaurant_data_.begin(); it != restaurant_data_.end(); ++it) {
		if (it->s_has_ordered == true && it->s_food_served == false) {
			listbox_ = new wxListBox(panel, it->s_table_id, wxPoint(x, y), wxSize(120, 100), {});
			listbox_->SetBackgroundColour(wxColor(0, 0, 200));
			listbox_->InsertItems(it->s_order, 0);
			listbox_->Bind(wxEVT_LISTBOX_DCLICK, &DiningRoom::onListBoxClicked, this);
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
void DiningRoom::createButtons(wxWindow* panel) {
	int tableID = ID_TABLE_BASE;
	int x = 150;
	int y = 100;
	int index = 0;
	for (int i = 0; i < 15; i++) {
		wxButton* table = new wxButton(panel, tableID, "", wxPoint(x, y), wxSize(75, 75));
		index = findIndexOfTable(tableID);

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

		table->Bind(wxEVT_BUTTON, &DiningRoom::onButtonClick, this);
		tableID++;
		y += 120;
		if (y == 700) {
			x += 150;
			y = 100;
		}
		if (i == 14) table = nullptr;
	}
}

int DiningRoom::findIndexOfTable(int& id) const {
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
bool DiningRoom::hasPatrons(int& id) {
	num_patrons_ = 0;
	cur_table_index_ = findIndexOfTable(id);
	if(cur_table_index_ != -1)
		if (restaurant_data_[cur_table_index_].s_has_people == true) return false;

	dialog_ = new wxDialog(this, wxID_ANY, "Enter how many patrons are being sat", wxPoint(500, 300), wxDefaultSize);
	wxButton* button = new wxButton(dialog_, wxID_ANY, "Confirm", wxPoint(150, 100), wxSize(75, 50));
	spin_ = new wxSpinCtrl(dialog_, wxID_ANY, wxEmptyString, wxPoint(165, 75), wxDefaultSize, 16384L, 1, 4);
	TableData table_data;
	button->Bind(wxEVT_BUTTON, &DiningRoom::updatePatronNumberOnClick, this);

	//if UpdatePatronOnClick event terminates dialog, update variables (only if successful process)
	if (dialog_->ShowModal() != wxID_OK) {
		if (num_patrons_ != 0) {
			table_data.s_table_id = id;
			table_data.s_patrons_sat = num_patrons_;
			table_data.s_has_people = true;
			table_data.s_options.clear();
			table_data.s_options.resize(num_patrons_); //resize the rows
			for (auto& patronOptions : table_data.s_options) { //resize the columns
				patronOptions.resize(4); // Give every patron 4 options for their order, depending on the dish
			}
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

/*
	Function that uses another dialog to activate function process.
	Creates a dialog modal that containers a choicebox of food category, based on how many people were sat at the table.
	All choice boxes created are binded to event related to AddOrderOfPatrons.
	Once selected, event tied to function creates listBoxes related to category selected, and once the modal ends,
	updates the index of the container that matches the table's id, and changes color on pressing confirm.

	Return false if order_placed_ is true, otherwise return true once process is completed once.
*/
bool DiningRoom::hasOrders(int& id) {
	cur_table_index_ = findIndexOfTable(id);
	if (cur_table_index_ == -1) return false;
	else if (restaurant_data_[cur_table_index_].s_has_ordered == true) return false;

	dialog_ = new wxDialog(this, wxID_ANY, "Order Menu", wxPoint(650, 100), wxSize(720, 400)); //edited wxPoint
	wxButton* orderButton = new wxButton(dialog_, ID_CONFIRM_ORDER, "Confirm", wxPoint(300, 300), wxSize(75, 50));
	orderButton->Bind(wxEVT_BUTTON, &DiningRoom::updateOrdersOnClick, this);

	if (restaurant_data_[cur_table_index_].s_patrons_sat == 3) {
		dialog_->SetSize(wxSize(540, 400));
		orderButton->SetSize(wxSize(75, 50));
		orderButton->SetPosition(wxPoint(200, 300));
	}
	else if (restaurant_data_[cur_table_index_].s_patrons_sat == 2) {
		dialog_->SetSize(wxSize(365, 400));
		orderButton->SetSize(wxSize(75, 50));
		orderButton->SetPosition(wxPoint(140, 300));
	}
	else if (restaurant_data_[cur_table_index_].s_patrons_sat == 1) {
		dialog_->SetSize(wxSize(190, 600)); //edited size (idea would be to change size when conditions met (type of food))
		orderButton->SetSize(wxSize(75, 50));
		orderButton->SetPosition(wxPoint(40, 400)); //changed y from 300 to 400
	}

	int choiceId = ID_CHOICE_BASE;
	int x = 10;
	for (int i = 0; i < restaurant_data_[cur_table_index_].s_patrons_sat; i++) {
		choice_ = new wxChoice(dialog_, choiceId, wxPoint(x, 50), wxDefaultSize,
			{ "Seafood", "Meat", "Combination" }, 0L, wxDefaultValidator, "Choose category of dishes");
		choice_->Bind(wxEVT_CHOICE, &DiningRoom::createFoodOptionsOnSelection, this);
		x += 175;
		choiceId++;
	}

	// if updateOrdersOnClick ends abruptly and all orders were not selected for number of patrons sat, return false
	if (dialog_->ShowModal() != wxID_OK && restaurant_data_[cur_table_index_].s_has_ordered == false) {
		delete dialog_;
		dialog_ = nullptr;
		listbox_ = nullptr;
		choice_ = nullptr;
		return false;
	}
	return true;
}

/*
	Function that will update how many dishes have been ordered this session.
	Uses two pointer of Choice and ListBox, and if neither points to nullptr, takes the value and uses it to begin searching
	through the different struct vectors and updates the corresponding index to the dish

	I.E. if Salmon is at index 0, and a customer orders Salmon, then we update/increment s_count of seafood_ at index 0 by 1
*/
void DiningRoom::updateCountOfDishes() {
	int choiceId = ID_CHOICE_BASE;
	int listId = ID_LISTBOX_BASE;
	choice_ = (wxChoice*)this->FindWindowById(choiceId);
	listbox_ = (wxListBox*)this->FindWindowById(listId);
	while (choice_ != nullptr) {
		if (choice_->GetSelection() == 0) {
			if (listbox_ != nullptr) {
				if (seafood_.find(listbox_->GetStringSelection()) != seafood_.end()) {
					seafood_.find(listbox_->GetStringSelection())->second.s_dish_count += 1;
				}
			}
		}
		else if (choice_->GetSelection() == 1) {
			if (listbox_ != nullptr) {
				if (meat_.find(listbox_->GetStringSelection()) != meat_.end()) {
					meat_.find(listbox_->GetStringSelection())->second.s_dish_count += 1;
				}
			}
		}
		else if (choice_->GetSelection() == 2) {
			if (listbox_ != nullptr) {
				if (combination_.find(listbox_->GetStringSelection()) != combination_.end()) {
					combination_.find(listbox_->GetStringSelection())->second.s_dish_count += 1;
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
	Function that will update line 3 of database if it exist, or initialize line 3 for the first time and return a string
	that will be used in overwriting Database.txt.
	If line 3 exist of database, it will perform the appropriate math based on the length of string. For length 2 and greater,
	the entry gets updated as normal. For strings of length 1, the value stored in *it is converted to ASCII with std::stoi,
	so we subtract by '0' to retrieve the original integer value and perform the math as normal from there.
*/
std::string DiningRoom::updateDataOfDishes(const std::string& line, std::unordered_map<wxString, DishData>& seafood_count, std::unordered_map<wxString, DishData>& meat_count, std::unordered_map<wxString, DishData>& combination_count) {
	std::string data = "";
	int size = 0;
	auto seafood = seafood_count.begin();
	auto meat = meat_count.begin();
	auto combination = combination_count.begin();
	if (line != "") {
		for (auto it = line.begin(); it != line.end(); ++it) {
			int num = 0;
			if (*it == ' ') {
				if (seafood != seafood_count.end()) {
					if (line.substr(it - line.begin() - size, size).length() >= 2) {
						num = std::stoi(line.substr(it - line.begin() - size, size)) + seafood->second.s_dish_count;
						seafood->second.s_dish_count = 0;
					}
					else {
						num = std::stoi(line.substr(it - line.begin() - size, size)) - '0' + seafood->second.s_dish_count;
						seafood->second.s_dish_count = 0;
					}
					++seafood;
				}
				else if (meat != meat_count.end()) {
					if (line.substr(it - line.begin() - size, size).length() >= 2) {
						num = std::stoi(line.substr(it - line.begin() - size, size)) + meat->second.s_dish_count;
						meat->second.s_dish_count = 0;
					}
					else {
						num = std::stoi(line.substr(it - line.begin() - size, size)) - '0' + meat->second.s_dish_count;
						meat->second.s_dish_count = 0;
					}
					++meat;
				}
				else if (combination != combination_count.end()) {
					if (line.substr(it - line.begin() - size, size).length() >= 2) {
						num = std::stoi(line.substr(it - line.begin() - size, size)) + combination->second.s_dish_count;
						combination->second.s_dish_count = 0;
					}
					else {
						num = std::stoi(line.substr(it - line.begin() - size, size)) - '0' + combination->second.s_dish_count;
						combination->second.s_dish_count = 0;
					}
					++combination;
				}
				data += std::to_string(num) + " ";
				size = 0;
			}
			size++;
		}
		return data;
	}
	// initial database initialization
	else if (line == "") {
		while (seafood != seafood_count.end()) {
			data += std::to_string(seafood->second.s_dish_count) + " ";
			seafood->second.s_dish_count = 0;
			++seafood;
		}
		while (meat != seafood_count.end()) {
			data += std::to_string(meat->second.s_dish_count) + " ";
			meat->second.s_dish_count = 0;
			++meat;
		}
		while (combination != seafood_count.end()) {
			data += std::to_string(combination->second.s_dish_count) + " ";
			combination->second.s_dish_count = 0;
			++combination;
		}
		return data;
	}
	return "";
}

/*
	Function that is called in MainFrame.cpp on window close, where we take the vectors of dishes count, and update the amount
	of order that have taken place in the database (text file). Takes into account whether or not it is user's first time updating
	the database or updating existing database with new values to be added to existing entries.
*/
void DiningRoom::setDataIntoDatabase(std::unordered_map<wxString, DishData>& seafood_count, std::unordered_map<wxString, DishData>& meat_count, std::unordered_map<wxString, DishData>& combination_count) {
	std::vector<std::string> database = {}; // highly ineffcient but the method will work for now
	std::ifstream database_copy;
	database_copy.open("Database.txt");
	if (database_copy.is_open()) {
		std::string line = "";
		while (std::getline(database_copy, line)) {
			database.push_back(line);
		}
		database_copy.close();
	}
	// represents the new data to be inserted in the database (either overwriting or initialization)
	std::string data = "";
	if (database.size() == 3) {
		data = updateDataOfDishes(database[2], seafood_count, meat_count, combination_count);
		database.pop_back(); // removes the last line that will be overwritten (if exist)
	}
	else if (database.size() == 2) {
		data = updateDataOfDishes("", seafood_count, meat_count, combination_count);
	}

	std::ofstream overwrite_database("Database.txt");
	if (overwrite_database.is_open()) {
		for (auto it = database.begin(); it != database.end(); ++it) {
			overwrite_database << *it << std::endl;
		}
		overwrite_database << data;
		overwrite_database << std::endl;
		overwrite_database.close();
	}
}