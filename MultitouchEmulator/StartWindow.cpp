#include "StdAfx.h"
#include "StartWindow.h"


StartWindow::StartWindow(Parameters * par)
{
  add_button(Gtk::Stock::OK, response::OK);
  parameters = par;
}


StartWindow::~StartWindow(void)
{
}
