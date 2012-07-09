#pragma once

#include "parameters.h"

#include <libxml++\libxml++.h>

/**
 * Class which store Parameters in XML.
 */
class ParametersXML : public Parameters
{
public:
  /**
   * Constructor creates Parameters objects from xmlpp::Node, which is type "<parameters>".
   * @param parameter_node xmlpp::Node which is "<parameters>".
   */
  ParametersXML(xmlpp::Node & parameter_node);
  /**
   * Default constructor to create from application
   * @param name Name of Parameters as std::string
   */
  ParametersXML(std::string name);
  /**
   * Default destructor.
   */
  ~ParametersXML(void);

  /**
   * Save parameters as XML as child of parent_node.
   * @param parent_node xmlpp::Node which will be parent of created node.
   */
  void saveToXML(xmlpp::Node * parent_node);
};

