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
	this->Bind(wxEVT_CLOSE_WINDOW, &Admin::frameOnClose, this);

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
		return true;
	}
	return false;
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
		- limiting what can be entered (like no whitespaces or illegal characters)
		- Even with correct user and pass, program does not recognize and will not process the params
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
		this->Destroy();
		frame_->Show();
		frame_ = nullptr;
		username = nullptr;
		password = nullptr;
		wxLogStatus("Username and Password matches!");
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

void Admin::frameOnClose(wxCloseEvent& evt) {
	this->Destroy(); //bad use, but it works. unsure why Close() does not work
	frame_->Show();
	frame_ = nullptr;
}
