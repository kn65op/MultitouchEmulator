#include "StdAfx.h"
#include "Key.h"

#include <random>

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