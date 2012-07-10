#include "StdAfx.h"
#include "ParametersManager.h"

#include <libxml++\libxml++.h>
#include <algorithm>

#include "ParametersXML.h"

ParametersManager::ParametersManager(std::string filename)
{
  this->filename = filename;

  //reading parameters from file
  xmlpp::DomParser parser;
  parser.set_substitute_entities();
  try
  {
    parser.parse_file(filename);
  }
  catch (std::exception ex)
  {
    return;
  }
  if (parser)
  {
    const xmlpp::Node* root = parser.get_document()->get_root_node();
    xmlpp::Node::NodeList params = root->get_children("parameters");
    std::for_each(params.begin(), params.end(), [this](xmlpp::Node* & it)
    {
      xmlpp::TextNode *tn;
      tn = dynamic_cast<xmlpp::TextNode*>(it->get_children("name").front()->get_children().front());
      if (tn)
      {
        this->parameters[tn->get_content()] = new ParametersXML(*it);
      }
    });
  }
}


ParametersManager::~ParametersManager(void)
{
}

Parameters & ParametersManager::getParameters(std::string name)
{
  //saving parameters
  save();

  //accesing wanted parameters
  try
  {
    return *(parameters.at(name));
  }
  catch (std::out_of_range ex)
  {
    throw NoParametersException();
  }
}

void ParametersManager::save(void)
{
  //make root 
  xmlpp::Document doc;
  doc.create_root_node("all");
  xmlpp::Node * root = doc.get_root_node();

  //add parameters
  iterator it, end;
  end = parameters.end();
  for (it = parameters.begin(); it != end; ++it)
  {
    dynamic_cast<ParametersXML*>(it->second)->saveToXML(root);
  }

  doc.write_to_file(filename);
}

std::list<std::string> ParametersManager::getParametersNames() 
{
  std::list<std::string> ret;

  iterator it, end;
  end = parameters.end();
  for (it = parameters.begin(); it != end; ++it)
  {
    ret.push_back(it->second->GetName());
  }

  return ret;
}

void ParametersManager::addNew()
{
  std::string new_name = "new";
  while (parameters.find(new_name) != parameters.end())
  {
    new_name += "1";
  }
  parameters[new_name] = new ParametersXML(new_name);
}

void ParametersManager::remove(std::string name)
{
  Parameters *p = parameters[name];
  parameters.erase(name);
  delete p;
}