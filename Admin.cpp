#include "Admin.h"

/*
	- Intiliazes frame_ to point to MainFrame's frame
	- Creates all the necessary components for the GUI, and binds events to certain actions
*/
Admin::Admin(const wxString& title, wxFrame* frame) : wxFrame(nullptr, wxID_ANY, title) {
	SetClientSize(800, 600);
	Center();
	Show();
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

bool Admin::searchUserAndPass(bool& user, bool& pass) {
	wxTextCtrl* userPtr = (wxTextCtrl*)this->FindWindowById(60); //points to the TextCtrl containing username input
	wxTextCtrl* passPtr = (wxTextCtrl*)this->FindWindowById(61); //points to the TextCtrl containing password input

	std::ifstream database;
	database.open("Database.txt");
	if (database.is_open()) {

		std::string word = "";
		char delimeter = ' '; //whitespace denotes end of word
		while (std::getline(database, word, delimeter)) {
			if (word != "Username:" || word != "Password:") {
				if (word == userPtr->GetLineText(0)) user = true;
				else if (word == passPtr->GetLineText(0)) pass = true;
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
	if (test_database.is_open()) return true;
	return false;
}

/*
	Function that will destroy all children in the Admin frame, then takes data from the database and inputs it into
	num_data, where it will retrieve the values later on. 
	The relevant controls will be created and will show how many times a dishes has been ordered this session to the user onto the
	panel.
	Error(remove later): current database has 1, 1, 1, 1, 1 for first 5 digits but only shows 4 1's. On top of that, 
	"Stk with Sh" is missing a number associated with the dish. 
*/
void Admin::displayDataFromDatabase() {
	this->GetChildren()[0]->DestroyChildren(); //panel destroys all children
	std::vector<std::string> num_data = {};
	std::vector<std::string> name_data = {"Lobster", "Crab", "Seabass", "Tuna", "Scallops",
											"Steak", "Veal", "Chicken", "Lamb", "Porkchops",
											"Stk & Lx", "Surf & Turf", "Ch & Stk", "Sh over Ling", "Stk with Sh" };
	std::ifstream database("Database.txt");
	std::string num = "";
	char delimeter = ' ';
	while (std::getline(database, num, delimeter)) {
		if (num.length() == 1) num_data.push_back(num);
	}
	
	//creates the necessary controls to display data (15 data params to show)
	wxPanel* panel = (wxPanel*)this->GetChildren()[0]; //takes the panel from Admin frame to use in creation of controls
	int x = 150;
	int y = 50;
	for (int i = 0; i < 15; i++) {
		std::string dish_name = name_data[i] + " ordered: ";
		new wxStaticText(panel, wxID_ANY, dish_name, wxPoint(x, y), wxSize(120, 50));
		new wxTextCtrl(panel, wxID_ANY, num_data[i], wxPoint(x + 200, y), wxSize(60, 50)); //change back to StaticText later

		y += 100;
		if (y == 750) {
			x += 350;
			y = 50;
		}
	}
}

void Admin::setDataIntoDatabase(const std::vector<int>& seafood_count, const std::vector<int>& meat_count, const std::vector<int>& combination_count) {
	std::vector<std::string> database = {}; //highly ineffcient but the method will work for now
	std::ifstream database_copy;
	database_copy.open("Database.txt");
	if (database_copy.is_open()) {
		std::string line = "";
		while (std::getline(database_copy, line)) {
			database.push_back(line);
		}
		database_copy.close();
	}
	//represents the new data to be inserted in the database (either overwriting or new)
	std::vector<std::string> data = updateDataOfDishes(database, seafood_count, meat_count, combination_count);
	if(database.size() == 3) database.pop_back(); //removes the last line that will be overwritten (if exist)

	std::ofstream overwrite_database("Database.txt");
	if (overwrite_database.is_open()) {
		for (auto it = database.begin(); it != database.end(); ++it) {
			overwrite_database << *it << std::endl;
		}
		for (auto it = data.begin(); it != data.end(); ++it) {
			overwrite_database << *it << " ";
		}
		overwrite_database << std::endl;
		overwrite_database.close();
	}
	this->Destroy(); //since we invoked the constructor in MainFrame, we destroy (at least i think for mem safety)
}

/*
	Function that will take all count of dishes ordered in MainFrame, and returns a vector with either
	overwritten or new data to set the database with.
	Absolutely horrendous parameters, but will do for now.
*/
std::vector<std::string> Admin::updateDataOfDishes(const std::vector<std::string>& database, const std::vector<int>& seafood_count, const std::vector<int>& meat_count, const std::vector<int>& combination_count) {
	std::vector<std::string> num_of_dishes = {};
	int index = 0;
	//need to change names of it2, it3, it4 to something more clearer
	auto it2 = seafood_count.begin();
	auto it3 = meat_count.begin();
	auto it4 = combination_count.begin();
	if (database.size() == 3) {
		//gets the string from database[2] (line 3), parses it and only appends strings of length 1 to num_of_dishes not including ' '
		for (auto it = database[2].begin(); it != database[2].end(); ++it) {
			if (*it != ' ') {
				num_of_dishes.push_back(std::to_string(*it));
			}
		}

		for (auto it = num_of_dishes.begin(); it != num_of_dishes.end(); ++it) {
			int num = 0;
			if (index < 5) {
				num = std::stoi(*it) - '0' + *it2; //current index of data (we do - '0' to remove char value and return the int) + current index of seafood (str to int)
				*it = std::to_string(num); //iterator of data now points to new value added from num (returns back to string)
				++it2;
			}
			else if (index > 4 && index < 10) {
				num = std::stoi(*it) - '0' + *it3;
				*it = std::to_string(num);
				++it3;
			}
			else if (index > 9 && index < 15) {
				num = std::stoi(*it) - '0' + *it4;
				*it = std::to_string(num);
				++it4;
			}
			index++;
		}
		return num_of_dishes;
	}	
	else if (database.size() == 2) {
		while (index < 15) {
			if (index < 5) {
				num_of_dishes.push_back(std::to_string(*it2));
				++it2;
			}
			else if (index > 4 && index < 10) {
				num_of_dishes.push_back(std::to_string(*it3));
				++it3;
			}
			else if (index > 9 && index < 15) {
				num_of_dishes.push_back(std::to_string(*it4));
				++it4;
			}
			index++;
		}
		return num_of_dishes;
	}
	return {}; //empty only if no condition matches
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
