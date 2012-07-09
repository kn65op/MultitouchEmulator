#pragma once

#include <map>

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

  /**
   * Function returs parameters with given name. If not found NoParametersException is thrown.
   * @param name std::string wih name of parameters.
   * @return Parameters with given name.
   */
  Parameters & getParameters(std::string name);

private:
  //container for Parameters
  std::map<std::string, Parameters*> parameters;
};

