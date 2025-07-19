#pragma once
#include <wx/wx.h>
#include <fstream>

class Admin : public wxFrame {
public:

	Admin(const wxString& title, wxFrame* frame);
	void loginButtonClicked(wxCommandEvent& evt);
	void adminFrameOnClose(wxCloseEvent& evt);
	void displayDataFromDatabase();
	bool searchUserAndPass(bool& user, bool& pass);
	bool hasDatabase();

private:
	wxFrame* frame_;
};