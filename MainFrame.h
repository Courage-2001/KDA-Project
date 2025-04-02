#pragma once
#include <wx/wx.h>

class MainFrame : public wxFrame { //mainframe inherits from wxFrame
public:
	MainFrame(const wxString& title); //wxstring is str implementation of wxwidgets, 
	//whenever wxstring is needed pass char array or stl str
	void CreateButtons(wxWindow* panel);
	void SwitchButtonClicked(wxCommandEvent& evt);
	void CreateListBox(wxWindow* panel);
};

