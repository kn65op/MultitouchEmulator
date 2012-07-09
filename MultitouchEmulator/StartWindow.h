#pragma once

#include "ParametersManager.h"

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
  StartWindow(ParametersManager * par);
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
  ParametersManager * parameters;
  //choose field
};

