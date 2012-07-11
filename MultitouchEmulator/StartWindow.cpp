#include "StdAfx.h"
#include "StartWindow.h"

#include <algorithm>

#include "Parameters.h"

StartWindow::StartWindow(ParametersManager * par)
{
  //initialize variables
  user = true;
  need = true;
  
  //button to check image
  add_button("Show check image", CHECK_IMAGE);

  //button ok
  ok = add_button(Gtk::Stock::OK, OK);
  parameters = par;
  setSensitives(false);

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
  main_vbox->pack_start(buttons_box);

  main_vbox->show_all_children(true);

  //labels
  name_label.set_label("Name");
  camera_height_label.set_label("Camera height (in cm)");
  device_height_label.set_label("Device height (in cm)");
  cam_pos_x_label.set_label("Camera position in x dimension (in pixels)");
  cam_pos_y_label.set_label("Camera position in y dimension (in pixels)");
  mistakes_label.set_label("Mistake probability (0-1)");
  time_label.set_label("Time of showing one bit (ms)");
  new_button.set_label("New");
  delete_button.set_label("Delete");

  //setting list of parameters
  insertParametersNames();

  //signals
  new_button.signal_clicked().connect(sigc::mem_fun(*this, &StartWindow::on_new_button_pressed));
  delete_button.signal_clicked().connect(sigc::mem_fun(*this, &StartWindow::on_delte_buton_pressed));
  choose_combo_box.signal_changed().connect(sigc::mem_fun(*this, &StartWindow::on_choose_combo_box_changed));
}


StartWindow::~StartWindow(void)
{
}

std::string StartWindow::getSelectedName()
{
  need = false;
  return choose_combo_box.get_active_text();
}

void StartWindow::insertParametersNames()
{
  user = false;
  choose_combo_box.clear();

  std::list<std::string> names = parameters->getParametersNames();

  std::for_each(names.begin(), names.end(), [this](std::string & name)
  {
    this->choose_combo_box.append_text(name);
  });
  user = true;
}

void StartWindow::on_choose_combo_box_changed()
{
  //setting button ok
  setSensitives(choose_combo_box.get_active_text() != "");

  //if it is not user action then return
  if (!user)
  {
    return;
  }

  //saving last
  save_actual_parameters();

  old_choosen = choose_combo_box.get_active_text();
  insertParametersNames();

  //reading from parameters
  try
  {
    Parameters & p = parameters->getParameters(old_choosen);
    std::stringstream ch,cx, cy, dh, m, t;
    ch << p.GetCamera_height();
    camera_height_field.set_text(ch.str());
    cx << p.GetCamera_pos_x();
    cam_pos_x_field.set_text(cx.str());
    cy << p.GetCamera_pos_y();
    cam_pos_y_field.set_text(cy.str());
    dh << p.GetDevice_height();
    device_height_field.set_text(dh.str());
    m << p.GetMistake_posibility();
    mistakes_field.set_text(m.str());
    t << p.GetTime();
    time_field.set_text(t.str());
    name_field.set_text(p.GetName());
  }
  catch (ParametersManager::NoParametersException ex)
  {
    clearFields();
  }
    
  user = false;
  choose_combo_box.set_active_text(old_choosen);
  user = true;
}

void StartWindow::on_new_button_pressed()
{
  user = false;
  save_actual_parameters();
  parameters->addNew();
  insertParametersNames();
  clearFields();
  old_choosen = "";
  user = true;
}

void StartWindow::on_delte_buton_pressed()
{
  user = false;
  std::string tmp = choose_combo_box.get_active_text();
  if (tmp != "")
  {
    parameters->remove(tmp);
    insertParametersNames();
    old_choosen = "";
  }
  clearFields();
  user = true;
}

void StartWindow::save_actual_parameters()
{
  if (old_choosen != "")
  {
    try
    {
      parameters->setParameters(old_choosen, name_field.get_text(), atof(camera_height_field.get_text().c_str()), atof(device_height_field.get_text().c_str()), atof(cam_pos_x_field.get_text().c_str()), atof(cam_pos_y_field.get_text().c_str()),
        atof(mistakes_field.get_text().c_str()), atoi(time_field.get_text().c_str()));
    }
    catch (ParametersManager::NoParametersException ex)
    {
    }
  }
}

void StartWindow::clearFields()
{
  camera_height_field.set_text("");
  cam_pos_x_field.set_text("");
  cam_pos_y_field.set_text("");
  device_height_field.set_text("");
  mistakes_field.set_text("");
  time_field.set_text("");
  name_field.set_text("");
}

bool StartWindow::isNeed()
{
  return need;
}

void StartWindow::setSensitives(bool s)
{
  ok->set_sensitive(s);
  name_field.set_sensitive(s);
  camera_height_field.set_sensitive(s);
  device_height_field.set_sensitive(s);
  cam_pos_x_field.set_sensitive(s);
  cam_pos_y_field.set_sensitive(s);
  mistakes_field.set_sensitive(s);
  time_field.set_sensitive(s);
}