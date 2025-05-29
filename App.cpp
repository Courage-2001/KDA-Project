#include "App.h"


wxIMPLEMENT_APP(App); //lets wxWdigets know which class represents our app, will generate main func, instance of app & call OnInit

bool App::OnInit() {
		MainFrame* mainFrame = new MainFrame("KDS App"); //initializes new frame with name of window
		return true; //tells wxWidgets that process must continue, if false app closes immedietely
}

