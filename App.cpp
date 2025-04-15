#include "App.h"
#include "MainFrame.h"

wxIMPLEMENT_APP(App); //lets wxWdigets know which class represents our app, will generate main func, instance of app & call OnInit

bool App::OnInit() {
	MainFrame* mainFrame = new MainFrame("KDS App"); //name of window
	mainFrame->SetClientSize(800, 600); //width, height
	mainFrame->Center(); //centers window on screen
	mainFrame->Show(); //by default title is hidden must call Show() method
	return true; //tells wxWidgets that process must continue, if false app closes immedietely
}

