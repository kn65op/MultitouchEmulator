#pragma once


class EndWindow :
  public Gtk::Dialog
{
public:
  EndWindow(void);
  ~EndWindow(void);
private:
  //Buttons
  Gtk::Button from_device;
  Gtk::Button from_screen;
  Gtk::Button from_start;
  Gtk::Button end;
  //Label
  Gtk::Label question;

  //boxes
  Gtk::HBox hbox;
};

