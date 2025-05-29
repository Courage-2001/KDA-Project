#pragma once
#include "MainFrame.h"

#pragma once
#include <wx/wx.h>

class Admin : public wxFrame {
public:
	Admin(const wxString& title, wxFrame* frame);
	void loginButtonClicked(wxCommandEvent& evt);


private:
	wxFrame* frame_;
};