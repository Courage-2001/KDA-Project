#include "Admin.h"

/*
	Intiliazes frame_ to point to MainFrame's frame
	Creates all the necessary components for the GUI, and binds events to certain actions
*/
Admin::Admin(const wxString& title, wxFrame* frame) : wxFrame(nullptr, wxID_ANY, title) {
	SetClientSize(800, 600);
	Center();
	frame_ = frame;

	wxPanel* loginPanel = new wxPanel(this, wxID_ANY, wxPoint(0, 0), wxSize(800, 800));
	wxStaticText* staticUser = new wxStaticText(loginPanel, wxID_ANY, "Enter Username: ", wxPoint(200, 100));
	wxTextCtrl* usernameInput = new wxTextCtrl(loginPanel, 60, "", wxPoint(300, 100), wxSize(200, 25));
	wxStaticText* staticPass = new wxStaticText(loginPanel, wxID_ANY, "Enter Password: ", wxPoint(200, 200));
	wxTextCtrl* passwordInput = new wxTextCtrl(loginPanel, 61, "", wxPoint(300, 200), wxSize(200, 25));
	wxButton* loginButton = new wxButton(loginPanel, wxID_ANY, "Login", wxPoint(500, 300), wxSize(100, 50));
	loginButton->Bind(wxEVT_BUTTON, &Admin::loginButtonClicked, this);
	this->Bind(wxEVT_CLOSE_WINDOW, &Admin::adminFrameOnClose, this);

}

/*
	Function that will search the database(text file) to match the input of what the user wrote for user and password.
	Return true if function completes searching database, false if unable to open database file at all (meaning 
*/
bool Admin::searchUserAndPass(bool& user, bool& pass) {
	wxTextCtrl* userPtr = (wxTextCtrl*)this->FindWindowById(60); //points to the TextCtrl containing username input
	wxTextCtrl* passPtr = (wxTextCtrl*)this->FindWindowById(61); //points to the TextCtrl containing password input

	std::ifstream database;
	database.open("Database.txt");
	if (database.is_open()) {
		std::string word = "";
		while (std::getline(database, word, ' ')) {
			if (word != "Username:" || word != "Password:") {
				if (word == userPtr->GetLineText(0)) user = true;
				else if (word == passPtr->GetLineText(0)) pass = true;
				else if (word == "Lobster") break; // ends search of file
			}
		}
		database.close();
		return true;
	}
	return false;
}

bool Admin::hasDatabase() {
	std::ifstream test_database;
	test_database.open("Database.txt");
	if (test_database.is_open()) {
		test_database.close();
		return true;
	}
	return false;
}

/*
	Function that will destroy all children in the Admin frame (except panel), then takes data from the database and 
	inputs it into the vector data, where it will retrieve the values later on. 
	The relevant controls will be created and will show how many times a dishes has been ordered this session to the 
	user onto the panel.
*/
void Admin::displayDataFromDatabase() {
	this->GetChildren()[0]->DestroyChildren();
	std::vector<std::pair<std::string, std::string>> data = { {"Crab", ""}, {"Lobster", ""}, {"Scallops", ""}, {"Seabass", ""}, {"Tuna", ""},
																   {"Chicken", ""}, {"Lamb", ""}, {"Porkchops", ""}, {"Steak", ""}, {"Veal", ""},
																   {"Ch & Stk", ""}, {"Sh over Ling", ""}, {"Stk & Lx", ""}, {"Stk with Sh", ""}, {"Surf & Turf", ""} };
	std::ifstream database("Database.txt");
	std::string num = "";
	int line = 1;
	int size = 0;
	while (std::getline(database, num)) {
		if (line == 3) {
			int index = 0;
			for (auto it = num.begin(); it != num.end(); ++it) {
				if (*it == ' ') {
					data[index].second = std::move((num.substr(it - num.begin() - size, size))); //pushback all valid inputs in line 3 of database (not including ' ')
					index++;
					size = 0;
				}
				size++;
			}
		}
		line++;
	}
	
	// creates the necessary controls to display data (15 data params to show)
	wxPanel* panel = (wxPanel*)this->GetChildren()[0]; //takes the panel from Admin frame to use in creation of controls
	int x = 50;
	int y = 50;
	for (int i = 0; i < 15; i++) {
		std::string dish_data = std::move(data[i].first) + " ordered: " + std::move(data[i].second);
		new wxStaticText(panel, wxID_ANY, dish_data, wxPoint(x, y), wxSize(180, 35));

		y += 50;
		if (y == 400) {
			x += 200;
			y = 50;
		}
	}
}

/*
	Function that will update line 3 of database if it exist, or initialize line 3 for the first time and return a string
	that will be used in overwriting Database.txt.
	If line 3 exist of database, it will perform the appropriate math based on the length of string. For length 2 and greater, 
	the entry gets updated as normal. For strings of length 1, the value stored in *it is converted to ASCII with std::stoi, 
	so we subtract by '0' to retrieve the original integer value and perform the math as normal from there. 
*/
std::string Admin::updateDataOfDishes(const std::string& line, std::map<wxString, int>& seafood_count, std::map<wxString, int>& meat_count, std::map<wxString, int>& combination_count) {
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
						num = std::stoi(line.substr(it - line.begin() - size, size)) + seafood->second;
						seafood->second = 0;
					}
					else {
						num = std::stoi(line.substr(it - line.begin() - size, size)) - '0' + seafood->second;
						seafood->second = 0;
					}
					++seafood;
				}
				else if (meat != meat_count.end()) {
					if (line.substr(it - line.begin() - size, size).length() >= 2) {
						num = std::stoi(line.substr(it - line.begin() - size, size)) + meat->second;
						meat->second = 0;
					}
					else {
						num = std::stoi(line.substr(it - line.begin() - size, size)) - '0' + meat->second;
						meat->second = 0;
					}
					++meat;
				}
				else if (combination != combination_count.end()) {
					if (line.substr(it - line.begin() - size, size).length() >= 2) {
						num = std::stoi(line.substr(it - line.begin() - size, size)) + combination->second;
						combination->second = 0;
					}
					else {
						num = std::stoi(line.substr(it - line.begin() - size, size)) - '0' + combination->second;
						combination->second = 0;
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
			data += std::to_string(seafood->second) + " ";
			seafood->second = 0;
			++seafood;
		}
		while (meat != seafood_count.end()) {
			data += std::to_string(meat->second) + " ";
			meat->second = 0;
			++meat;
		}
		while (combination != seafood_count.end()) {
			data += std::to_string(combination->second) + " ";
			combination->second = 0;
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
void Admin::setDataIntoDatabase(std::map<wxString, int>& seafood_count, std::map<wxString, int>& meat_count, std::map<wxString, int>& combination_count) {
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

/*
	Event that will attempt to look if database exist, or create a new database with new username and password.
	Uses two TextCtrl ptr's to point to the controls created in the construtor denoting the user and pass.
	If textfile opens, we search using a delimeter for the user and password, where if the user and pass matches what is in
	the input, (FOR THE MOMENT) returns back to the orignal frame (MainFrame).
	If textfile opens, but user and pass do not match, the modal continues on until either user inputs correct params or closes
	the modal.
	If textfile is unable to be opened (meaning it doesn't exist), we create a new database textfile and return the
	user back to the original frame (MainFrame)

	MISSING: 
		- limiting what can be entered (like illegal characters)
*/
void Admin::loginButtonClicked(wxCommandEvent& evt) {
	wxDialog* dialog = new wxDialog(this, wxID_ANY, "", wxPoint(500, 300), wxDefaultSize);
	wxStaticText* staticText = new wxStaticText(dialog, wxID_ANY,
		"No records exist. \nInitializing Database with new user.", wxPoint(50, 50), wxDefaultSize);

	wxDialog* errorDialog = new wxDialog(this, wxID_ANY, "", wxPoint(500, 300), wxDefaultSize);
	wxStaticText* errorText = new wxStaticText(errorDialog, wxID_ANY,
		"", wxPoint(50, 50), wxDefaultSize);

	wxTextCtrl* username = (wxTextCtrl*)this->FindWindowById(60);
	wxTextCtrl* password = (wxTextCtrl*)this->FindWindowById(61);
	bool is_user = false;
	bool is_pass = false;
	if(searchUserAndPass(is_user, is_pass) == false) {
		if (username->GetLineText(0) == "" || password->GetLineText(0) == "") {
			errorText->SetLabel("There is a blank entry. \nPlease give a VALID entry for both fields to initialize database.");
			errorDialog->ShowModal();
		}
		else if (dialog->ShowModal() != wxID_OK) {
			std::ofstream newDatabase("Database.txt");
			newDatabase << "Username: " << username->GetLineText(0)
			<< " Password: " << password->GetLineText(0) << " " << std::endl;
			newDatabase << "Lobster Crab Seabass Tuna Scallops "
				<< "Steak Veal Chicken Lamb Porkchops "
				<< "SteakandLobster SurfandTurf ChickenandSteak ShrimpoverLinguini SteakwithShrimp " << std::endl;
			newDatabase.close();

			this->Destroy();
			frame_->Show();
			frame_ = nullptr;
			username = nullptr;
			password = nullptr;
			wxLogStatus("Database successfully created!");
		}
	}
	else if (is_user == true && is_pass == true) {
		this->GetChildren()[0]->DestroyChildren(); //panel remains, but all children of panel is gone
		displayDataFromDatabase();
		username = nullptr;
		password = nullptr;
		wxLogStatus("displayDataFromDatabase() worked! user and pass matched");
	}
	else if (is_user == true && is_pass == false) {
		errorText->SetLabel("password is incorrect. Try again!");
		errorDialog->ShowModal();
	}
	else if (is_user == false && is_pass == true) {
		errorText->SetLabel("username is incorrect. Try again!");
		errorDialog->ShowModal();
	}
	else if (is_user == false && is_pass == false) {
		errorText->SetLabel("both username and password is incorrect. Try again!");
		errorDialog->ShowModal();
	}
}

void Admin::adminFrameOnClose(wxCloseEvent& evt) {
	this->Destroy(); //bad use, but it works. unsure why Close() does not work
	frame_->Show();
	frame_ = nullptr;
}
