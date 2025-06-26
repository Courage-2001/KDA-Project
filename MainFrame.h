#pragma once
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include "Admin.h"

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

	//need to replace vector with just an array at some point (i think?)
	//the index of a dish in s_array corresponds to the index in s_count 
	//so if steak was index 0, then index 0 in s_count represents # of times steak has been ordered this session
	struct logistic {
		wxArrayString s_array;
		std::vector<int> s_count;
	};

	MainFrame(const wxString& title); //wxstring is str implementation of wxwidgets

	void createButtons(wxWindow* panel);
	void createListBox(wxWindow* panel);
	bool hasPatrons(int& id);
	bool hasOrders(int& id);
	int findIndex(int& id) const;
	std::vector<dataset> getContainer() const;
	std::vector<int> getSeafoodCount() const;
	std::vector<int> getMeatCount() const;
	std::vector<int> getCombinationCount() const;
	Unique SwitchID(Unique& id); //unused, must remove or rework in the future
	void updateCountOfDishes();
	void onButtonClick(wxCommandEvent& evt);
	void switchButtonClicked(wxCommandEvent& evt);
	void switchSettingClicked(wxCommandEvent& evt);
	void updatePatronNumberOnClick(wxCommandEvent& evt);
	void createOptionsOnClick(wxCommandEvent& evt);
	void updateOrdersOnClick(wxCommandEvent& evt);
	void mainframeOnClose(wxCloseEvent& evt);

private:
	int num_patrons_;
	wxArrayString event_container_;
	std::vector<dataset> container_;
	wxFrame* frame_;
	wxListBox* listbox_;
	wxChoice* choice_;
	wxSpinCtrl* spin_;
	wxDialog* dialog_;
	std::vector<logistic> seafood_;
	std::vector<logistic> meat_;
	std::vector<logistic> combination_;
	bool hasLogin_;
};

