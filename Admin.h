#pragma once
#include <wx/wx.h>
#include <fstream>

class Admin : public wxFrame {
public:
	Admin(const wxString& title, wxFrame* frame);
	void loginButtonClicked(wxCommandEvent& evt);
	void frameOnClose(wxCloseEvent& evt);
	bool searchUserAndPass(bool& user, bool& pass);



private:
	wxFrame* frame_;
};