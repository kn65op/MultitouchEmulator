#pragma once


class EndWindow :
  public Gtk::Dialog
{
public:
  EndWindow(void);
  ~EndWindow(void);

  //enums
  enum response
  {
    START,
    SCREEN,
    DEVICE,
    END,
    TRANSMISSION
  };
private:
  //Label
  Gtk::Label question;

  //boxes
  Gtk::HBox hbox;
};

