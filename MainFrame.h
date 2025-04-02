#pragma once
#include <wx/wx.h>
#include <string>

class MainFrame : public wxFrame { //mainframe inherits from wxFrame
public:
	enum Unique { Table1 = 2, Table2, Table3, Table4, Table5, Table6, Table7, 
		Table8, Table9, Table10, Table11, Table12, Table13, Table14, Table15
	};

	struct dataset {
		Unique id;
		std::vector<std::string> dataset;
	};

	MainFrame(const wxString& title); //wxstring is str implementation of wxwidgets
	void CreateButtons(wxWindow* panel);
	void SwitchButtonClicked(wxCommandEvent& evt);
	void CreateListBox(wxWindow* panel);
	void WriteAndRead(wxListBox* panel);
	Unique SwitchID(Unique& id);

private:
	std::vector<dataset> container;
};

