#include "MainFrame.h"
#include <wx/wx.h>
#include <wx/editlbox.h>

bool panelFlag = true;

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	wxPanel* firstPanel = new wxPanel(this, wxID_ANY, wxPoint(0, 0), wxSize(800,800));
	wxPanel* secondPanel = new wxPanel(this, wxID_ANY, wxPoint(800, 0), wxSize(750, 800));
	firstPanel->SetBackgroundColour(wxColor(0, 0, 200));
	secondPanel->SetBackgroundColour(wxColor(0, 0, 200));


	CreateButtons(firstPanel); //arg is wxWindows, but panel is accepted (because subclass of wxWindows maybe?)
	CreateStatusBar();

	wxButton* switchScreenButton = new wxButton(secondPanel, wxID_ANY, "Switch Screens", wxPoint(500, 700), wxSize(100, 50));
	//may remove listbox for some sort of choice box instead
	wxEditableListBox* listBox = new wxEditableListBox(secondPanel, wxID_ANY, "Enter entries here", wxPoint(0, 100), wxSize(500, 600));
	switchScreenButton->Bind(wxEVT_BUTTON, &MainFrame::SwitchButtonClicked, this);

}

//may change later into two seperate functions
void MainFrame::WriteAndRead(wxListBox* box) {
}

void MainFrame::CreateListBox(wxWindow* panel) {
	Unique id = Table1;
	int x = 150;
	int y = 20;
	for (int i = 0; i < 15; i++) {
		new wxListBox(panel, id, wxPoint(x, y), wxSize(100, 150), {"hello", "bye"});
		panel->GetChildren()[i]->SetBackgroundColour(wxColor(0, 0, 200));
		SwitchID(id);
		y += 200;
		if (y == 1020) {
			x += 150;
			y = 20;
		}
	}
}

/*
	Loop that adds inline button, where y(location of button) increments until reaches a certain point,
	moves to the next column of buttons.

	Second loop then edits each inline button color
*/
void MainFrame::CreateButtons(wxWindow* panel) {
	Unique id = Table1;
	int x = 150;
	int y = 100;
	for (int i = 0; i < 15; i++) {
		new wxButton(panel, wxID_ANY, "", wxPoint(x, y), wxSize(75, 75));
		panel->GetChildren()[i]->SetBackgroundColour(wxColor(0, 0, 0));
		SwitchID(id);
		y += 120;
		if (y == 700) {
			x += 150;
			y = 100;
		}
	}
}

/*
	Uses global flag to determine which function to use on button click.
	Destroys all current children of panel, creates new children on each instance
	Sets flag to opposite value on each click
*/
void MainFrame::SwitchButtonClicked(wxCommandEvent& evt) {

	if (panelFlag == false) {
		this->GetChildren()[0]->DestroyChildren();
		CreateButtons(this->GetChildren()[0]);
		panelFlag = true;
		wxLogStatus("Buttons created");
	}
	else if (panelFlag == true) {
		this->GetChildren()[0]->DestroyChildren();
		CreateListBox(this->GetChildren()[0]);
		panelFlag = false;
		wxLogStatus("Listboxes created");
	}
}

MainFrame::Unique MainFrame::SwitchID(Unique& id) {
	switch (id) 
	{
	case Table1: 
		id = Table2;
	case Table2:
		id = Table3;
	case Table3:
		id = Table4;
	case Table4:
		id = Table5;
	case Table5:
		id = Table6;
	case Table6:
		id = Table7;
	case Table7:
		id = Table8;
	case Table8:
		id = Table9;
	case Table9:
		id = Table10;
	case Table10:
		id = Table11;
	case Table11:
		id = Table12;
	case Table12:
		id = Table13;
	case Table13:
		id = Table14;
	case Table14:
		id = Table15;
	}
	return id;
}