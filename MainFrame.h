#pragma once
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <unordered_map>
#include "Admin.h"

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

	struct DishData {
		int s_dish_count = 0;
		wxString s_dish_type = "";
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
	void createSubOptionsOnSelection(wxCommandEvent& evt);
	void updateOrdersOnClick(wxCommandEvent& evt);
	void updateCurrentOrderStatusOnClick(wxCommandEvent& evt);
	void mainframeOnClose(wxCloseEvent& evt);
	std::string updateDataOfDishes(const std::string& line, std::unordered_map<wxString, DishData>& seafood_count, std::unordered_map<wxString, DishData>& meat_count, std::unordered_map<wxString, DishData>& combination_count);
	void setDataIntoDatabase(std::unordered_map<wxString, DishData>& seafood_count, std::unordered_map<wxString, DishData>& meat_count, std::unordered_map<wxString, DishData>& combination_count);

private:
	int num_patrons_;
	wxArrayString table_order_;
	std::vector<TableData> restaurant_data_;
	wxFrame* frame_;
	wxListBox* listbox_;
	wxChoice* choice_;
	wxSpinCtrl* spin_;
	wxDialog* dialog_;
	std::unordered_map<wxString, DishData> seafood_;
	std::unordered_map<wxString, DishData> meat_;
	std::unordered_map<wxString, DishData> combination_;
	bool hasLogin_;
};

