#include "Admin.h"

Admin::Admin(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	flag_ = false;
	wxPanel* loginPanel = new wxPanel(this, wxID_ANY, wxPoint(0, 0), wxSize(800, 800));
	wxButton* loginButton = new wxButton(loginPanel, wxID_ANY, "Login", wxPoint(100, 500), wxSize(100, 50));
	loginButton->Bind(wxEVT_BUTTON, &Admin::loginButtonClicked, this);

}

void Admin::setFlag() {
	flag_ = true;
}

bool Admin::getFlag() const {
	return flag_;
}

void Admin::loginButtonClicked(wxCommandEvent& evt) {
	setFlag();
	this->Destroy();
}
