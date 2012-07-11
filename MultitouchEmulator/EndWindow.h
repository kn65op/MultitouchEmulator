#pragma once

/**
 * Class which is dialog asking what to do after transmission finished.
 */
class EndWindow :
  public Gtk::Dialog
{
public:
  /**
   * Default constructor.
   */
  EndWindow(void);
  /**
   * Default destructor
   */
  ~EndWindow(void);

  /**
   * Enum for responses of dialog.
   */
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

