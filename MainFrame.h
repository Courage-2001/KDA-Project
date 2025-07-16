#pragma once
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include "Admin.h"
#include <map>

class MainFrame : public wxFrame { //mainframe inherits from wxFrame
public:

	struct TableData {
		int s_table_id = 0;
		wxArrayString s_order = {};
		int s_patrons_sat = 0;
		bool s_has_ordered = false;
		bool s_has_people = false;
		bool s_food_served = false;
	};

	MainFrame(const wxString& title); //wxstring is str implementation of wxwidgets

	void createButtons(wxWindow* panel);
	void createListBox(wxWindow* panel);
	bool hasPatrons(int& id);
	bool hasOrders(int& id);
	int findIndexOfTable(int& id) const;
	void updateCountOfDishes();
	void onButtonClick(wxCommandEvent& evt);
	void switchButtonClicked(wxCommandEvent& evt);
	void onSettingClicked(wxCommandEvent& evt);
	void onListBoxClicked(wxCommandEvent& evt);
	void updatePatronNumberOnClick(wxCommandEvent& evt);
	void createOptionsOnClick(wxCommandEvent& evt);
	void updateOrdersOnClick(wxCommandEvent& evt);
	void updateCurrentOrderStatusOnClick(wxCommandEvent& evt);
	void mainframeOnClose(wxCloseEvent& evt);

private:
	int num_patrons_;
	wxArrayString table_order_;
	std::vector<TableData> restaurant_data_;
	wxFrame* frame_;
	wxListBox* listbox_;
	wxChoice* choice_;
	wxSpinCtrl* spin_;
	wxDialog* dialog_;
	std::map<wxString, int> seafood_;
	std::map<wxString, int> meat_;
	std::map<wxString, int> combination_;
	bool hasLogin_;
};

