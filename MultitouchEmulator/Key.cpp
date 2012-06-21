#include "StdAfx.h"
#include "Key.h"

#include <random>
#include <iostream>
#include <sstream>

Key::Key(void)
{
  //generating alphabet
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
  
  for (int i=0; i<n; i++)
  {
    key += alphabet[dist(gen)];
  }
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
  std::vector<bool> ret(8);

  //zapisanie liczy urz�dze�
  int tmp = number_of_devices;
  int i = 7;
  do
  {
    if ( (tmp & 1) == 0 )
      ret[i--] = false;
    else
      ret[i--] = true;

    tmp >>= 1;
  } while ( tmp );

  while (i > -1)
  {
    ret[i--] = false;
  }

  /*for (int i =0; i < ret.size(); ++i)
  {
    std::cout << ret[i];
  }
  std::cout << "\n";//*/

  //skopiowanie klucza do kodu
  std::string::iterator sit, send;
  send = key.end();
  for (sit = key.begin(); sit != send; ++sit)
  {
    if (*sit == 1)
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
  //generating md5
  std::stringstream ss;
  ss << number_of_devices << key;
  
  BYTE *hash = hash_func((BYTE*)ss.str().c_str(), ss.str().size(), MD5);

	if (hash)
	{
		std::cout << "hash: 0x";

		for (int i = 0; i < 16; i ++)
			std::cout << std::hex << (int)hash[i];
	}

	delete [] hash;

  return std::vector<bool>(128);
}

unsigned char * Key::hash_func(BYTE *input, int size, HashType type)
{
	HCRYPTPROV hCryptProv;
	HCRYPTHASH hHash;
	BYTE *pData;
	DWORD dwHashLength;
	DWORD dwLength;
	DWORD dwHashType = (DWORD)type;

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
