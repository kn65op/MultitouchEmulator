#include "StdAfx.h"
#include "Key.h"

#include <random>
#include <iostream>
#include <sstream>
#include <WinCrypt.h>
#include <Windows.h>

Key::Key(void)
{
  //generating alphabet, from this characters key is generated
  //alphabet = "qwertyuiopasdfghjklzxcvbnm1234567890!@#$%^&*()QWERTYUIOPLKJHGFDSAZXCVBNM-=_+[];',./<>?:\"{}|\\";
  alphabet = "10";
}


Key::~Key(void)
{
}

void Key::generateMainKey(int n)
{
  key_length = n;
  key = "";

  //create random engine
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dist(0, alphabet.length() - 1);
  
  //generating key from alphabet
  for (int i=0; i<n; i++)
  {
    key += alphabet[dist(gen)];
  }
  std::cout << "Key: " << key << "\n";
}

void Key::setHashLength(int length)
{
  hash_length = length;
}

void Key::setNumberOfDevices(int  nod)
{
  number_of_devices = nod;
}

std::vector<bool> Key::getMasterDeviceCode()
{
  std::vector<bool> ret;

  //store number of devices in binary
  //ret = changeIntToBinary(number_of_devices);

  //TODO: depends of the transmision protocol

  //copy key (from std::string cointains only '1' or '0') to binary. If alphabet changes it should be also changed
  std::string::iterator sit, send;
  send = key.end();
  for (sit = key.begin(); sit != send; ++sit)
  {
    if (*sit == '1')
    {
      ret.push_back(true);
    }
    else
    {
      ret.push_back(false);
    }
  }
  return ret;
}

std::vector<bool> Key::getSecondaryDeviceCode(int n)
{
  std::vector<bool> ret;
  //generating hash 
  std::stringstream ss;
  ss << n << key;
  
  BYTE *hash = hash_func((BYTE*)ss.str().c_str(), ss.str().size(), SHA);
  //BYTE *hash = aes((BYTE*)ss.str().c_str(), ss.str().size());

  //id of device:
  ret = changeIntToBinary(n);

	if (hash)
	{
    //length of message
    int len = hash_length / 8;

    //debug: showing generated key for secondary devices
		std::cout << "hash: 0x";

    //change hash into binary code
		for (int i = 0; i < len; i ++) //it should be changed due to change of encryption.
    {
			std::cout << std::hex << (int)hash[i];
      storeHexIntoKey(hash[i], ret);
    }

    std::cout << "\n";
	}

	delete [] hash;

  return ret;
}

unsigned char * Key::aes(unsigned char * input, int size)
{
  BYTE pass[] = "pass"; //key for AES
  BYTE *data = new BYTE[128];
  int i;
  for (i=0; i <size; ++i)
  {
    data[i] = input[i];
  }
  for ( ; i<128; ++i)
  {
    data[i] = ' ';
  }

  //making provider
  HCRYPTPROV hCryptProv = NULL; 
  HCRYPTKEY hKey = NULL; 
  HCRYPTKEY hXchgKey = NULL; 
  HCRYPTHASH hHash = NULL; 
  if (CryptAcquireContext(&hCryptProv, NULL, MS_ENH_RSA_AES_PROV, PROV_RSA_AES, CRYPT_NEWKEYSET))
  {
    std::cout << "Provider\n";
  }
  else
  {
    if (CryptAcquireContext(&hCryptProv, NULL, MS_ENH_RSA_AES_PROV, PROV_RSA_AES, 0))
    {
      std::cout << "Provider\n";
    }
    else
    {
      DWORD tmp = GetLastError();
      return 0; //error
    }
  }

  //key
  if (CryptCreateHash(hCryptProv, CALG_MD5, 0, 0, &hHash))
  {
    std::cout << "Key\n";
  }
  else
  {
    DWORD tmp = GetLastError();
    return 0; //error
  }

  if (CryptHashData(hHash, pass, 4,0))
  {
    std::cout << "Key2\n";
  }
  else
  {
    DWORD tmp = GetLastError();
    return 0; //error
  }

  if (CryptDeriveKey(hCryptProv, CALG_AES_128, hHash, 0x00800000, &hKey))
  {
    std::cout << "Key3\n";
  }
  else
  {
    DWORD tmp = GetLastError();
    return 0; //error
  }

  unsigned long data_size = 128;
  //encrypt
  if (CryptEncrypt(hKey, NULL, false, 0, data, &data_size, data_size))
  {
    std::cout << "encrypted!\n";
  }
  else
  {
    DWORD tmp = GetLastError();
     return 0; //error
  }
  return data;
}

unsigned char * Key::hash_func(BYTE *input, int size, HashType type)
{
	HCRYPTPROV hCryptProv;
	HCRYPTHASH hHash;
	BYTE *pData;
	DWORD dwHashLength;
	DWORD dwLength;
	DWORD dwHashType = (DWORD)type;

  //calculating md5
	if (CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
	{
		if(CryptCreateHash(hCryptProv, dwHashType, 0, 0, &hHash))
		{
			if(CryptHashData(hHash, input, size, 0))
			{
				CryptGetHashParam(hHash, HP_HASHSIZE, (BYTE*)&dwHashLength, &dwLength, 0);
				pData = new BYTE [dwHashLength];
				CryptGetHashParam(hHash, HP_HASHVAL, pData, &dwHashLength, 0);
				CryptDestroyHash(hHash);
				CryptReleaseContext(hCryptProv, 0);

				return pData;
			}
		}
	}

	return NULL;
}

void Key::storeHexIntoKey(BYTE hex, std::vector<bool> & key)
{
  int tmp = 128;
  for (int i = 7; i != -1; --i)
  {
    if (hex & tmp)
    {
      key.push_back(true);
    }
    else
    {
      key.push_back(false);
    }
    tmp /= 2;
  }
}

int Key::getLongestLength() const
{
  return key_length + 8 > 128 ? key_length + 8 : 128;
}

std::vector<bool> Key::changeIntToBinary(int n)
{
  std::vector<bool> ret;
  int i = 7;
  do
  {
    if ( (n & 1) == 0 )
      ret[i--] = false;
    else
      ret[i--] = true;

    n >>= 1;
  } while ( n ); 
  
  while (i > -1)
  {
    ret[i--] = false;
  }

  return ret;
}