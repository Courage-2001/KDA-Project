#pragma once
#include <wx/wx.h>
#include <fstream>
#include <queue>
#include <map>

class Admin : public wxFrame {
public:
	Admin(const wxString& title, wxFrame* frame);
	void loginButtonClicked(wxCommandEvent& evt);
	void adminFrameOnClose(wxCloseEvent& evt);
	void displayDataFromDatabase();
	void setDataIntoDatabase(std::map<wxString, int>& seafood_count, std::map<wxString, int>& meat_count, std::map<wxString, int>& combination_count);
	std::string updateDataOfDishes(const std::string& line, std::map<wxString, int>& seafood_count, std::map<wxString, int>& meat_count, std::map<wxString, int>& combination_count);
	bool searchUserAndPass(bool& user, bool& pass);
	bool hasDatabase();

	


private:
	wxFrame* frame_;
};