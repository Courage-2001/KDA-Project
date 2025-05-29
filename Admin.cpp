#include "Admin.h"

Admin::Admin(const wxString& title, wxFrame* frame) : wxFrame(nullptr, wxID_ANY, title) {
	SetClientSize(800, 600);
	Center();
	Show();
	frame_ = frame;

	wxPanel* loginPanel = new wxPanel(this, wxID_ANY, wxPoint(0, 0), wxSize(800, 800));
	wxButton* loginButton = new wxButton(loginPanel, wxID_ANY, "Login", wxPoint(100, 500), wxSize(100, 50));
	loginButton->Bind(wxEVT_BUTTON, &Admin::loginButtonClicked, this);

}

void Admin::loginButtonClicked(wxCommandEvent& evt) {
	Close();
	frame_->Show();
	frame_ = nullptr;
}
