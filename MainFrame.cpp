#include "MainFrame.h"
#include "MainFrame.h"
#include <wx/wx.h>


//wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL); //why sizer works as global, unsure (potential ERROR LATER)
bool panelFlag = true;

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	wxPanel* firstPanel = new wxPanel(this, wxID_ANY, wxPoint(0, 0), wxSize(800,800));
	wxPanel* secondPanel = new wxPanel(this, wxID_ANY, wxPoint(800, 0), wxSize(750, 800));
	firstPanel->SetBackgroundColour(wxColor(0, 0, 200));
	secondPanel->SetBackgroundColour(wxColor(0, 0, 200));


	CreateButtons(firstPanel); //second arg is wxWindows, but panel is accepted (because subclass of wxWindows maybe?)
	CreateStatusBar();

	wxButton* switchScreenButton = new wxButton(secondPanel, wxID_ANY, "Switch Screens", wxPoint(500, 700), wxSize(100, 50));
	switchScreenButton->Bind(wxEVT_BUTTON, &MainFrame::SwitchButtonClicked, this);

}

void MainFrame::CreateListBox(wxWindow* panel) {
	int x = 150;
	int y = 100;
	for (int i = 0; i < 15; i++) {
		new wxListBox(panel, wxID_ANY, wxPoint(x, y), wxSize(75, 75));
		y += 120;
		if (y == 700) {
			x += 150;
			y = 100;
		}
	}

	for (int i = 0; i < panel->GetChildren().size(); i++) {
		panel->GetChildren()[i]->SetBackgroundColour(wxColor(0, 0, 200));
	}
}

/*
	Loop that adds inline button, where y(location of button) increments until reaches a certain point,
	moves to the next column of buttons.

	Second loop then edits each inline button color
*/
void MainFrame::CreateButtons(wxWindow* panel) {
	int x = 150;
	int y = 100;
	for (int i = 0; i < 15; i++) {
		new wxButton(panel, wxID_ANY, "", wxPoint(x, y), wxSize(75, 75));
		y += 120;
		if (y == 700) {
			x += 150;
			y = 100;
		}
	}

	for (int i = 0; i < panel->GetChildren().size(); i++) { 
		panel->GetChildren()[i]->SetBackgroundColour(wxColor(0, 0, 0));
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