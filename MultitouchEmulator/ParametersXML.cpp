#include "StdAfx.h"
#include "ParametersXML.h"

ParametersXML::ParametersXML(xmlpp::Node & parameter_node)
{
  //reading xml
  std::string tmp;
  name = parameter_node.get_children("name").front()->get_name();
  camera_height = atof(parameter_node.get_children("cameraHeight").front()->get_name().c_str());
  device_height = atof(parameter_node.get_children("deviceHeight").front()->get_name().c_str());
  camera_pos_x = atof(parameter_node.get_children("CameraPositionX").front()->get_name().c_str());
  camera_pos_y= atof(parameter_node.get_children("CameraPositionY").front()->get_name().c_str());
  mistake_posibility = atof(parameter_node.get_children("Mistakes").front()->get_name().c_str());
  time = atoi(parameter_node.get_children("Mistakes").front()->get_name().c_str());
}


ParametersXML::~ParametersXML(void)
{
}

void ParametersXML::saveToXML(xmlpp::Node * parent_node)
{

}