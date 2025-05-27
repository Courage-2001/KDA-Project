#pragma once
#include "MainFrame.h"

#pragma once
#include <wx/wx.h>

class Admin : public wxFrame {
public:
	Admin(const wxString& title);
	void setFlag();
	bool getFlag() const;
	void loginButtonClicked(wxCommandEvent& evt);


private:
	bool flag_;
};