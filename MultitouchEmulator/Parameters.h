#pragma once

/**
 * Class which handle with parameters: camera height, etc.
 */
class Parameters
{
public:
  /**
   * Default constructor.
   */
  Parameters(void);
  /**
   * Default destructor.
   */
  virtual ~Parameters(void);

  /**
   * Get camera height.
   * @return Camera height as double.
   */
  double GetCamera_height() const
  {
    return camera_height;
  }

  /**
   * Set camera height
   * @param camera_height Camera height.
   */
  void SetCamera_height(double camera_height)
  {
    this->camera_height = camera_height;
  }

  /**
   * Get camera position in x dimension.
   * @return Camera position in x dimension as double.
   */
  double GetCamera_pos_x() const
  {
    return camera_pos_x;
  }

  /**
   * Set camera position in x dimension.
   * @param camera_pos_x Camera position in x dimension.
   */
  void SetCamera_pos_x(double camera_pos_x)
  {
    this->camera_pos_x = camera_pos_x;
  }

  /**
   * Get camera position in y dimension.
   * @return Camera position in y dimension as double.
   */
  double GetCamera_pos_y() const
  {
    return camera_pos_y;
  }

  /**
   * Set camera position in x dimension.
   * @param camera_pos_y Camera position in x dimension.
   */
  void SetCamera_pos_y(double camera_pos_y)
  {
    this->camera_pos_y = camera_pos_y;
  }

  /**
   * Get device height.
   * @return Device height as double.
   */
  double GetDevice_height() const
  {
    return device_height;
  }

  /**
   * Set Device height
   * @param device_height Device height.
   */
  void SetDevice_height(double device_height)
  {
    this->device_height = device_height;
  }

  /**
   * Get mistake posibility.
   * @return Mistake posibility as double.
   */
  double GetMistake_posibility() const
  {
    return mistake_posibility;
  }

  /**
   * Set mistake posibility.
   * @param mistake_posibility Mistake posibility.
   */
  void SetMistake_posibility(double mistake_posibility)
  {
    this->mistake_posibility = mistake_posibility;
  }

  /**
   * Get time between two bits.
   * @return Time between two bits as double.
   */
  int GetTime() const
  {
    return time;
  }

  /**
   * Set time between two bits
   * @param time Time.
   */
  void SetTime(int time)
  {
    this->time = time;
  }

  /**
   * Get name of parameters.
   * @return Name of paramiters as std::string.
   */
  std::string GetName() const
  {
    return name;
  }

  /**
   * Set name of parameters.
   * @param name std::string with name to set.
   */
  void SetName(std::string name)
  {
    this->name = name;
  }


protected:
  //name
  std::string name;
  //camera height in cm
  double camera_height;
  //device height in cm
  double device_height;
  //camera position in x and y dimensions calculated in pixels of screen
  double camera_pos_x;
  double camera_pos_y;
  //probability of make mistake during transmission
  double mistake_posibility;
  //time between two bits
  int time;
};

