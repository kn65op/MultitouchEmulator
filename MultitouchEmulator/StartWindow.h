#pragma once

#include "Parameters.h"

class StartWindow : public Gtk::Dialog
{
public:
  StartWindow(Parameters * par);
  ~StartWindow(void);

  enum response
  {
    OK
  };
private:
  //parameters
  Parameters * parameters;
  //choose field
};

