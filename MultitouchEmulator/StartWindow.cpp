#include "StdAfx.h"
#include "StartWindow.h"


StartWindow::StartWindow(ParametersManager * par)
{
  add_button(Gtk::Stock::OK, response::OK);
  parameters = par;
}


StartWindow::~StartWindow(void)
{
}

std::string StartWindow::getSelectedName() const
{
  return "";
}