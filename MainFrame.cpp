#include "MainFrame.h"
#include "MainFrame.h"
#include <wx/wx.h>

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	wxPanel* panel = new wxPanel(this);
	panel->SetBackgroundColour(wxColor(0,0,200));

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
}