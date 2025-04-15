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
		int id_ = 0;
		wxArrayString order_ = {};
		int numOfPatrons_ = 0;
		bool order_placed_ = false;
	};

	MainFrame(const wxString& title); //wxstring is str implementation of wxwidgets
	void CreateButtons(wxWindow* panel);
	void CreateListBox(wxWindow* panel);
	bool AddNumberOfPatrons(int& id);
	bool AddOrderOfPatrons(int& id);
	bool FindIdOfButton(int& id);
	int FindIndexOfId(int& id) const;
	std::vector<dataset> GetContainer() const;
	wxArrayString GetSeafood() const;
	wxArrayString GetMeat() const;
	wxArrayString GetCombination() const;
	Unique SwitchID(Unique& id);
	void OnButtonClick(wxCommandEvent& evt);
	void SwitchButtonClicked(wxCommandEvent& evt);
	void UpdatePatronNumberOnClick(wxCommandEvent& evt);
	void CreateOptionsOnClick(wxCommandEvent& evt);
	void UpdateOrdersOnClick(wxCommandEvent& evt);

private:
	bool flag = false;
	int num_of_patrons_ = 0;
	wxArrayString event_container_;
	std::vector<dataset> container_;
	wxArrayString seafood_ = { "Lobster", "Crab", "Seabass", "Tuna", "Scallops" };
	wxArrayString meat_ = {"Steak", "Veal", "Chicken", "Lamb", "Porkchops"};
	wxArrayString combination_ = {"Steak and Lobster", "Surf and Turf", "Chicken and Steak", "Shrimp over Linguini", "Steak with Shrimp"};
};

