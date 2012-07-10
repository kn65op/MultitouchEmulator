#pragma once

#include <map>
#include <list>

#include "Parameters.h"

#include <iostream>

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
  public:
    NoParametersException()
    {
      std::cout << "Thrown\n";
    }
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
  /**
   * Function save parameters to XML.
   */
  void save(void);

  /**
   * Function returns all names of available parameters.
   * @return std::list<std::string> with all names of parameters.
   */
  std::list<std::string> getParametersNames();

  /**
   * Function add new Parameters.
   */
  void addNew();

  /**
   * Function remove Parameters by name.
   * @param name std::string with name of Parameters to remove.
   */
  void remove(std::string name);

  /**
   * Change parameters of Parameters.
   * @param name Name of Parameters to change.
   * @param new_name New name of parameters.
   * @param camera_height Camera height.
   * @param device_height Device height.
   * @param cam_pos_x Camera position in x dimension.
   * @param cam_pos_y Camera position in y dimension.
   * @param mistake Mistakes probability.
   * @param time Time between showing two bits.
   */
  void setParameters(std::string name, std::string new_name, double camera_height, double device_height, double cam_pos_x, double cam_pos_y, double mistake, int time);

private:

  //container for Parameters
  container parameters;

  //filename
  std::string filename;

};

