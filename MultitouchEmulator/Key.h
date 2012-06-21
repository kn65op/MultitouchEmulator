#pragma once

#include <string>
#include <vector>
#include <windows.h>

class Key
{
public:
  /**
    * Generates random key.
    * @param n Length of key.
    */
  void generateMainKey(int n);

  /**
    * Setting hash length
    */
  void setHashLength(int length);

  /**
    * Functions setting number of devices
    */
  void setNumberOfDevices(int nod);

  /**
    * Function returns binary code for master device
    */
  std::vector<bool> getMasterDeviceCode();

  /**
    * Function returns binary code for secondary devices, by number.
    */
  std::vector<bool> getSecondaryDeviceCode(int n);

public:
  Key(void);
  ~Key(void);

private:
  int key_length;
  int hash_length;
  int number_of_devices;
  std::string key;

  std::string alphabet;

  enum HashType
  {
    MD2 = CALG_MD2, 
    MD5 = CALG_MD5, 
    SHA = CALG_SHA
  };

  unsigned char * hash_func(BYTE *input, int size, HashType type);

  void storeHexIntoKey(BYTE hex, std::vector<bool> & key);
};

