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

  /**
   * Function returns selected element on the list.
   * @return std::string with name of parameters.
   */
  std::string getSelectedName() const;
private:
  //parameters
  ParametersManager * parameters;
  //choose field
};

