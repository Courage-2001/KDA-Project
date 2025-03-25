#include "MainFrame.h"
#include "MainFrame.h"
#include <wx/wx.h>

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	wxPanel* panel = new wxPanel(this);
	panel->SetBackgroundColour(wxColor(0, 0, 200));

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	int x = 100;
	int y = 100;

	for (int i = 0; i < 15; i++) {
		sizer->Add(new wxButton(panel, wxID_ANY, "", wxPoint(x, y), wxSize(50, 50)),
			1,
			wxEXPAND | wxALL, 
			10
		);
		
		y += 100;
		if (y == 600) {
			x += 100;
			y = 100;
		}
	}
	/*
	for (auto it = panel->GetChildren().begin(); it != panel->GetChildren().end(); ++it) {
		it -> SetBackgroundColour(wxColor(0, 0, 0));
	}
	*/


	/*
	wxButton* button1 = new wxButton(panel, wxID_ANY, " ", wxPoint(100, 100), wxSize(50, 50));
	button1->SetBackgroundColour(wxColor(0, 0, 0));

	wxButton* button2 = new wxButton(panel, wxID_ANY, " ", wxPoint(100, 200), wxSize(50, 50));
	button2->SetBackgroundColour(wxColor(0, 0, 0));

	wxButton* button3 = new wxButton(panel, wxID_ANY, " ", wxPoint(100, 300), wxSize(50, 50));
	button3->SetBackgroundColour(wxColor(0, 0, 0));

	wxButton* button4 = new wxButton(panel, wxID_ANY, " ", wxPoint(100, 400), wxSize(50, 50));
	button4->SetBackgroundColour(wxColor(0, 0, 0));

	wxButton* button5 = new wxButton(panel, wxID_ANY, " ", wxPoint(100, 500), wxSize(50, 50));
	button5->SetBackgroundColour(wxColor(0, 0, 0));

	wxButton* button6 = new wxButton(panel, wxID_ANY, " ", wxPoint(200, 100), wxSize(50, 50));
	button6->SetBackgroundColour(wxColor(0, 0, 0));

	sizer->Add(button1, 1, wxEXPAND | wxALL, 10);
	sizer->Add(button2, 1, wxEXPAND | wxALL, 10);
	sizer->Add(button3, 1, wxEXPAND | wxALL, 10);
	sizer->Add(button4, 1, wxEXPAND | wxALL, 10);
	sizer->Add(button5, 1, wxEXPAND | wxALL, 10);
	sizer->Add(button6, 1, wxEXPAND | wxALL, 10);
	*/
}