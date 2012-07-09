#pragma once

#include <map>

#include "Parameters.h"

/**
 * Class with manage Parameters.
 */
class ParametersManager
{
private:
  //first element of pair
  typedef std::string first;
  //second element of pair
  typedef Parameters* second;
  //container type
  typedef std::map<first, second> container;
  //pair type
  typedef std::pair<first, second*> pair;
  //iterator type
  typedef std::map<first, second>::iterator iterator;
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
  /**
   * Function save parameters to XML.
   */
  void save(void);

  //container for Parameters
  container parameters;

  //filename
  std::string filename;

};

