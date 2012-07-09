#include "StdAfx.h"
#include "StartWindow.h"

#include <algorithm>

StartWindow::StartWindow(ParametersManager * par)
{
  add_button(Gtk::Stock::OK, response::OK);
  parameters = par;

  //getting main box
  Gtk::VBox * main_vbox = get_vbox();

  //setting view
  //hboxes on labels and fields
  name_box.pack_start(name_label);
  name_box.pack_end(name_field);
  camera_height_box.pack_start(camera_height_label);
  camera_height_box.pack_end(camera_height_field);
  device_height_box.pack_start(device_height_label);
  device_height_box.pack_end(device_height_field);
  cam_pos_x_box.pack_start(cam_pos_x_label);
  cam_pos_x_box.pack_end(cam_pos_x_field);
  cam_pos_y_box.pack_start(cam_pos_y_label);
  cam_pos_y_box.pack_end(cam_pos_y_field);
  mistakes_box.pack_start(mistakes_label);
  mistakes_box.pack_end(mistakes_field);
  time_box.pack_start(time_label);
  time_box.pack_end(time_field);

  //setting buttons into box
  buttons_box.pack_start(new_button);
  buttons_box.pack_end(delete_button);

  //add all to main box
  main_vbox->pack_start(choose_combo_box);
  main_vbox->pack_start(name_box);
  main_vbox->pack_start(camera_height_box);
  main_vbox->pack_start(device_height_box);
  main_vbox->pack_start(cam_pos_x_box);
  main_vbox->pack_start(cam_pos_y_box);
  main_vbox->pack_start(mistakes_box);
  main_vbox->pack_start(time_box);

  main_vbox->show_all_children(true);

  //labels
  name_label.set_label("Name");
  camera_height_label.set_label("Camera height (in cm)");
  device_height_label.set_label("Device height (in cm)");
  cam_pos_x_label.set_label("Camera position in x dimension (in pixels)");
  cam_pos_y_label.set_label("Camera position in y dimension (in pixels)");
  mistakes_label.set_label("Mistake probability");
  time_label.set_label("Time of showing one bit");

  //setting list of parameters
  insertParametersNames();
}


StartWindow::~StartWindow(void)
{
}

std::string StartWindow::getSelectedName() const
{
  return "";
}

void StartWindow::insertParametersNames()
{
  choose_combo_box.clear();

  std::list<std::string> names = parameters->getParametersNames();

  std::for_each(names.begin(), names.end(), [this](std::string & name)
  {
    this->choose_combo_box.append_text(name);
  });
  
}