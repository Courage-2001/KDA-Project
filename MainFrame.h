#pragma once
#include <wx/wx.h>
#include <string>
#include <wx/spinctrl.h>

class MainFrame : public wxFrame { //mainframe inherits from wxFrame
public:
	enum Unique { Table1 = 2, Table2, Table3, Table4, Table5, Table6, Table7, 
		Table8, Table9, Table10, Table11, Table12, Table13, Table14, Table15
	};

	struct dataset {
		int id_ = 0;
		std::vector<std::string> dataset_;
		int numOfPatrons_ = 0;
	};

	MainFrame(const wxString& title); //wxstring is str implementation of wxwidgets
	void CreateButtons(wxWindow* panel);
	void CreateListBox(wxWindow* panel);
	bool AddNumberOfPatrons(int& id);
	void WriteAndRead(wxListBox* panel);
	bool FindIdOfButton(int& id);
	Unique SwitchID(Unique& id);
	void OnButtonClick(wxCommandEvent& evt);
	void SwitchButtonClicked(wxCommandEvent& evt);
	void UpdatePatronNumberOnClick(wxCommandEvent& evt);

private:
	std::vector<dataset> container;
};

