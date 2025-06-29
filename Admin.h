#pragma once
#include <wx/wx.h>
#include <fstream>
#include <queue>

class Admin : public wxFrame {
public:
	Admin(const wxString& title, wxFrame* frame);
	void loginButtonClicked(wxCommandEvent& evt);
	void adminFrameOnClose(wxCloseEvent& evt);
	void displayDataFromDatabase();
	void setDataIntoDatabase(const std::vector<int>& seafood_count, const std::vector<int>& meat_count, const std::vector<int>& combination_count);
	std::string updateDataOfDishes(const std::string& line, const std::vector<int>& seafood_count, const std::vector<int>& meat_count, const std::vector<int>& combination_count);
	bool searchUserAndPass(bool& user, bool& pass);
	bool hasDatabase();

	


private:
	wxFrame* frame_;
};