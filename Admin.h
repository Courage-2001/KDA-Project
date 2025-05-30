#pragma once
#include <wx/wx.h>
#include <fstream>

class Admin : public wxFrame {
public:
	Admin(const wxString& title, wxFrame* frame);
	void loginButtonClicked(wxCommandEvent& evt);
	void frameOnClose(wxCloseEvent& evt);


private:
	wxFrame* frame_;
	wxTextCtrl* text_;
};