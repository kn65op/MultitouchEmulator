#include "StdAfx.h"
#include "ParametersManager.h"

#include <libxml++\libxml++.h>
#include <algorithm>

#include "ParametersXML.h"

ParametersManager::ParametersManager(std::string filename)
{
  //reading parameters from file
  xmlpp::DomParser parser;
  parser.set_substitute_entities();
  parser.parse_file(filename);
  if (parser)
  {
    const xmlpp::Node* root = parser.get_document()->get_root_node();
    xmlpp::Node::NodeList params = root->get_children("parameters");
    std::for_each(params.begin(), params.end(), [this](xmlpp::Node* & it)
    {
      this->parameters[it->get_children("name").front()->get_name()] = new ParametersXML(*it);
      //this->parameters[(*it)->get_children("name").front()->get_name()] = new ParametersXML(**it);
    });
  }
}


ParametersManager::~ParametersManager(void)
{
}

Parameters & ParametersManager::getParameters(std::string name)
{
  try
  {
    return *(parameters.at(name));
  }
  catch (std::out_of_range ex)
  {
    throw NoParametersException();
  }
}