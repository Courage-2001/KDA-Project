#pragma once
#include <wx/wx.h>
#include <string>
#include <array>
#include <wx/spinctrl.h>

class MainFrame : public wxFrame { //mainframe inherits from wxFrame
public:
	enum Unique { Table1 = 2, Table2, Table3, Table4, Table5, Table6, Table7, 
		Table8, Table9, Table10, Table11, Table12, Table13, Table14, Table15
	};

	struct dataset {
		int s_id = 0;
		wxArrayString s_order = {};
		int s_patrons_sat = 0;
		bool s_has_ordered = false;
		bool s_has_people = false;
	};

	MainFrame(const wxString& title); //wxstring is str implementation of wxwidgets
	void createButtons(wxWindow* panel);
	void createListBox(wxWindow* panel);
	bool hasPatrons(int& id);
	bool hasOrders(int& id);
	int findIndex(int& id) const;
	std::vector<dataset> getContainer() const;
	wxArrayString getSeafood() const;
	wxArrayString getMeat() const;
	wxArrayString getCombination() const;
	Unique SwitchID(Unique& id);
	void onButtonClick(wxCommandEvent& evt);
	void switchButtonClicked(wxCommandEvent& evt);
	void updatePatronNumberOnClick(wxCommandEvent& evt);
	void createOptionsOnClick(wxCommandEvent& evt);
	void updateOrdersOnClick(wxCommandEvent& evt);

private:
	int num_patrons_ = 0;
	wxArrayString event_container_;
	std::vector<dataset> container_;
	wxArrayString seafood_ = { "Lobster", "Crab", "Seabass", "Tuna", "Scallops" };
	wxArrayString meat_ = {"Steak", "Veal", "Chicken", "Lamb", "Porkchops"};
	wxArrayString combination_ = {"Steak and Lobster", "Surf and Turf", "Chicken and Steak", "Shrimp over Linguini", "Steak with Shrimp"};
};

