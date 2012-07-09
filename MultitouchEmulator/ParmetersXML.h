#pragma once

#include "parameters.h"

#include <libxml++\libxml++.h>

/**
 * Class which store Parameters in XML.
 */
class ParmetersXML : public Parameters
{
public:
  /**
   * Constructor creates Parameters objects from xmlpp::Node, which is type "<parameters>".
   * @param parameter_node xmlpp::Node which is "<parameters>".
   */
  ParmetersXML(xmlpp::Node parameter_node);
  /**
   * Default destructor.
   */
  ~ParmetersXML(void);

  /**
   * Save parameters as XML as child of parent_node.
   * @param parent_node xmlpp::Node which will be parent of created node.
   */
  void saveToXML(xmlpp::Node parent_node);
};

