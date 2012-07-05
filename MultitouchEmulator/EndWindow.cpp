#include "StdAfx.h"
#include "EndWindow.h"


EndWindow::EndWindow(void)
{
  //setting components
  question.set_label("What do you want to do next?");
  from_device.set_label("Start again from device recognition part");
  from_screen.set_label("Start again from screen recognition part");
  from_start.set_label("Start again from begining");
  end.set_label("Exit");

  //setting view
  get_vbox()->pack_start(question);
  get_vbox()->pack_end(hbox);

  hbox.pack_start(from_start);
  hbox.pack_start(from_screen);
  hbox.pack_start(from_device);
  hbox.pack_end(end);

  //showing
  get_vbox()->show_all_children(true);
}


EndWindow::~EndWindow(void)
{
}
