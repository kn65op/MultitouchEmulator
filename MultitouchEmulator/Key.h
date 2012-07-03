#pragma once

#include <string>
#include <vector>
#include <windows.h>

/**
 * Class do all the job with keys
 */
class Key
{
public:
  /**
   * Generates random key.
   * @param n Length of key.
   */
  void generateMainKey(int n);

  /**
   * Setting hash length.
   * @param length Demaned main key length.
   */
  void setHashLength(int length);

  /**
   * Functions setting number of devices.
   * @param nod Number of recognized devices.
   */
  void setNumberOfDevices(int nod);

  /**
   * Function returns binary code for master device.
   * @return Code for master device in std::vector<bool>.
   */
  std::vector<bool> getMasterDeviceCode();

  /**
   * Function returns binary code for secondary devices, by number.
   * @param n Number of device.
   * @return Code for secondary device in std::vector<bool>.
   */
  std::vector<bool> getSecondaryDeviceCode(int n);

  /**
   * Function returns length of the longest message
   * @return The Longest message length as int.
   */
  int getLongestLength() const;

public:
  /**
   * Constructor which creates alphabet.
   */
  Key(void);
  /**
   * Default destructor.
   */
  ~Key(void);

private:
  //Length of main key.
  int key_length;
  //Requiered length of secondary keys.
  int hash_length;
  //Number of devices
  int number_of_devices;
  //Main key
  std::string key;

  //Alphabet
  std::string alphabet;

  //Type of has (used while creating MD5)
  enum HashType
  {
    MD2 = CALG_MD2,
    MD5 = CALG_MD5,
    SHA = CALG_SHA
  };

  /**
   * Function calculating hash from input.
   * @param input Message to be hashed.
   * @param size Size of the message.
   * @param type Key::HashType hash type (actually supported is only CALC_MD5).
   * @return Calculated hash.
   */
  unsigned char * hash_func(BYTE *input, int size, HashType type);

  /**
   * Function stores one hexadecimal value (0-F) as binary vector.
   * @param hex Hexadecimal value.
   * @param key Vector where binary value is stored.
   */
  void storeHexIntoKey(BYTE hex, std::vector<bool> & key);
};

