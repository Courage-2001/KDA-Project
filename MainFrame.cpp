#include "MainFrame.h"
#include "MainFrame.h"
#include <wx/wx.h>

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	wxPanel* firstPanel = new wxPanel(this, wxID_ANY, wxPoint(0, 0), wxSize(800,800));
	wxPanel* secondPanel = new wxPanel(this, wxID_ANY, wxPoint(800, 0), wxSize(750, 800));
	firstPanel->SetBackgroundColour(wxColor(0, 0, 200));
	secondPanel->SetBackgroundColour(wxColor(0, 0, 200));

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	CreateButtons(sizer, firstPanel);

	wxButton* switchScreenButton = new wxButton(secondPanel, wxID_ANY, "Switch Screens", wxPoint(500, 700), wxSize(100, 50));

}


/*
Loop that adds inline button, where y(location of button) increments until reaches a certain point,
moves to the next column of buttons.

Second loop then edits each inline button color
*/
void MainFrame::CreateButtons(wxBoxSizer* sizer, wxPanel* panel) {
	int x = 150;
	int y = 100;
	for (int i = 0; i < 15; i++) {
		sizer->Add(new wxButton(panel, wxID_ANY, "", wxPoint(x, y), wxSize(75, 75)),
			1,
			wxEXPAND | wxALL,
			10
		);

		y += 120;
		if (y == 700) {
			x += 150;
			y = 100;
		}
	}

	for (int i = 0; i < panel->GetChildren().size(); i++) { //why this works, and not the other, is unclear.
		panel->GetChildren()[i]->SetBackgroundColour(wxColor(0, 0, 0));
	}

	/*
	for (auto it = panel->GetChildren().begin(); it != panel->GetChildren().end(); ++it) {
		it -> SetBackgroundColour(wxColor(0, 0, 0));
	}
	*/
}