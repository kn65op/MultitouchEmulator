#include "StdAfx.h"
#include "ParametersXML.h"

#include <sstream>

ParametersXML::ParametersXML(xmlpp::Node & parameter_node)
{
  //reading xml
  std::string tmp;
  name = parameter_node.get_children("name").front()->get_name();
  camera_height = atof(parameter_node.get_children("cameraHeight").front()->get_name().c_str());
  device_height = atof(parameter_node.get_children("deviceHeight").front()->get_name().c_str());
  camera_pos_x = atof(parameter_node.get_children("cameraPositionX").front()->get_name().c_str());
  camera_pos_y= atof(parameter_node.get_children("cameraPositionY").front()->get_name().c_str());
  mistake_posibility = atof(parameter_node.get_children("mistakes").front()->get_name().c_str());
  time = atoi(parameter_node.get_children("time").front()->get_name().c_str());
}


ParametersXML::~ParametersXML(void)
{
}

void ParametersXML::saveToXML(xmlpp::Node * parent_node)
{
  //sstreamy to change values to string
  std::stringstream ch, dh, cx, cy, m, t;
  ch << camera_height;
  dh << device_height;
  cx << camera_pos_x;
  cy << camera_pos_y;
  m << mistake_posibility;
  t << time;

  xmlpp::Element *child, *params;
  params = parent_node->add_child("parameters");

  child = params->add_child("name");
  child->add_child_text(name);
  child = params->add_child("cameraHeight");
  child->add_child_text(ch.str());
  child = params->add_child("deviceHeight");
  child->add_child_text(dh.str());
  child = params->add_child("cameraPositionX");
  child->add_child_text(cx.str());
  child = params->add_child("cameraPositionY");
  child->add_child_text(cx.str());
  child = params->add_child("mistakes");
  child->add_child_text(m.str());
  child = params->add_child("time");
  child->add_child_text(t.str());
}

ParametersXML::ParametersXML(std::string name)
{
  this->name = name;
  camera_height = camera_pos_x = camera_pos_y = device_height = mistake_posibility = 0.0;
  time = 0;
}