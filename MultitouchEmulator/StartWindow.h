#pragma once

#include "ParametersManager.h"

/**
 * Class which show starting dialog to choose parameters. Parameters changed cannot be undone.
 */
class StartWindow : public Gtk::Dialog
{
public:
  /**
   * Constructor which sets parameters.
   * @param par Pointer to parameters object.
   */
  StartWindow(ParametersManager * par);
  /**
   * Default destructor.
   */
  ~StartWindow(void);

  /**
   * Enum for responses.
   */
  enum response
  {
    OK
  };

  /**
   * Function returns selected element on the list.
   * @return std::string with name of parameters.
   */
  std::string getSelectedName() const;

protected:
  void on_choose_combo_box_changed();
  void on_add_button_pressed();
  void on_remove_buton_pressed();

private:
  //parameters
  ParametersManager * parameters;
  //choose field
  Gtk::ComboBoxText choose_combo_box;
  //buttons
  Gtk::Button new_button, delete_button;
  //labels
  Gtk::Label name_label, camera_height_label, device_height_label, cam_pos_x_label, cam_pos_y_label, mistakes_label, time_label;
  //fields
  Gtk::Entry name_field, camera_height_field, device_height_field, cam_pos_x_field, cam_pos_y_field, mistakes_field, time_field;

  //view part
  //boxes
  Gtk::HBox name_box, camera_height_box, device_height_box, cam_pos_x_box, cam_pos_y_box, mistakes_box, time_box, buttons_box;

  void insertParametersNames();
};

