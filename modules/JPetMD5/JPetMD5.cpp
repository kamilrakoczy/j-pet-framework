/**
 *  @copyright Copyright 2016 The J-PET Framework Authors. All rights reserved.
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may find a copy of the License in the LICENCE file.
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  @file JPetMD5.cpp
 */

#include "JPetMD5.h"
#include <algorithm>
#include <fstream>
#include <memory>

#include <iostream> //DELETE


const std::string JPetMD5::getMD5FromFile(const std::string filePath, const unsigned int chunkSize)
{
  unsigned char result[MD5_DIGEST_LENGTH];
  char resultTable[2 * MD5_DIGEST_LENGTH + 1];
  resultTable[2 * MD5_DIGEST_LENGTH] = '\0'; 
  std::ifstream inFile(filePath);
  MD5_CTX mdContext;
  int bytes = 0;
  std::unique_ptr<unsigned char> data(new unsigned char[chunkSize]);

  MD5_Init(&mdContext);
  while (inFile)
  {
    inFile.read((char*)data.get(), chunkSize);
    bytes = inFile.gcount();
    MD5_Update(&mdContext, data.get(), bytes);
  }

  MD5_Final(result, &mdContext);
  inFile.close();
  for (int j = 0; j < MD5_DIGEST_LENGTH; j++)
    sprintf(&resultTable[2 * j], "%02X", result[j]);

  std::string returnString(resultTable,
                           resultTable + MD5_DIGEST_LENGTH * 2);

  std::transform(returnString.begin(), returnString.end(), returnString.begin(), ::tolower);
  return returnString;
}
