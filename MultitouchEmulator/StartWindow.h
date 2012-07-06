#pragma once

#include "Parameters.h"

/**
 * Class which show starting dialog to choose parameters
 */
class StartWindow : public Gtk::Dialog
{
public:
  /**
   * Constructor which sets parameters.
   * @param par Pointer to parameters object.
   */
  StartWindow(Parameters * par);
  /**
   * Default destructor.
   */
  ~StartWindow(void);

  /**
   * Enum for responses.
   */
  enum response
  {
    OK
  };
private:
  //parameters
  Parameters * parameters;
  //choose field
};

