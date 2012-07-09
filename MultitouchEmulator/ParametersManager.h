#pragma once

#include "Parameters.h"

/**
 * Class with manage Parameters.
 */
class ParametersManager
{
public:

  /**
   * Class Exception for selecting parameters, when name is not found.
   */
  class NoParametersException
  {

  };

  /**
   * Constructor which takes filename to read parameters from.
   * @param filename std::string with filename to read parameters from.
   */
  ParametersManager(std::string filename);
  /**
   * Default destructor.
   */
  ~ParametersManager(void);

  Parameters & getParameters(std::string name);
};

