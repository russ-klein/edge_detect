/**************************************************************************
 *                                                                        *
 *  Edge Detect Design Walkthrough for HLS                                *
 *                                                                        *
 *  Software Version: 1.0                                                 *
 *                                                                        *
 *  Release Date    : Tue Jan 14 15:40:43 PST 2020                        *
 *  Release Type    : Production Release                                  *
 *  Release Build   : 1.0.0                                               *
 *                                                                        *
 *  Copyright 2020, Mentor Graphics Corporation,                          *
 *                                                                        *
 *  All Rights Reserved.                                                  *
 *  
 **************************************************************************
 *  Licensed under the Apache License, Version 2.0 (the "License");       *
 *  you may not use this file except in compliance with the License.      * 
 *  You may obtain a copy of the License at                               *
 *                                                                        *
 *      http://www.apache.org/licenses/LICENSE-2.0                        *
 *                                                                        *
 *  Unless required by applicable law or agreed to in writing, software   * 
 *  distributed under the License is distributed on an "AS IS" BASIS,     * 
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or       *
 *  implied.                                                              * 
 *  See the License for the specific language governing permissions and   * 
 *  limitations under the License.                                        *
 **************************************************************************
 *                                                                        *
 *  The most recent version of this package is available at github.       *
 *                                                                        *
 *************************************************************************/
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#include "EdgeDetect_Algorithm.h"
#if !(SINGLEPORT||CIRCULAR)
#include "EdgeDetect_Hierarchy.h"
#elif SINGLEPORT
#include "EdgeDetect_Hierarchy_Singleport.h"
#elif CIRCULAR
#include "EdgeDetect_Hierarchy_Circular.h"
#endif
#include "bmpUtil/bmp_io.hpp"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <mc_scverify.h>

CCS_MAIN(int argc, char *argv[])
{
  unsigned int iW = 1296;
  unsigned int iH = 864;
  EdgeDetect_Algorithm    inst0;
  EdgeDetect_Hierarchy    inst1;

  unsigned long int width = iW;
  long int height         = iH;
  unsigned char *rarray = new unsigned char[iH*iW];
  unsigned char *garray = new unsigned char[iH*iW];
  unsigned char *barray = new unsigned char[iH*iW];

  cout << "Loading Input File" << endl;

  if (argc < 3) {
    cout << "Usage: " << argv[0] << " <inputbmp> <outputbmp_alg> <outputbmp_ba>" << endl;
    CCS_RETURN(-1);
  }

  std::string bmpIn(argv[1]);  // input bitmap file
  std::string bmpAlg(argv[2]); // output bitmap (algorithm)
  std::string bmpBA(argv[3]);  // output bitmap (bit-accurate)

  bmp_read((char*)bmpIn.c_str(), &width, &height, &rarray, &garray, &barray);
  assert(width==iW);
  assert(height==iH);

#ifdef DEBUG
  std::string cmd;
  cmd = "display ";
  cmd.append(bmpIn.c_str());
  std::cout << "Display input image file using command: " << cmd << endl;
  std::system(cmd.c_str());
#endif

  ac_channel<pixelBus>            dat_in;
  ac_channel<magAngBus>  magn_angle;

  unsigned char *dat_in_orig = new unsigned char[iH*iW];;
  double *magn_orig = new double[iH*iW];
  double *angle_orig = new double[iH*iW];
  //iH = iW = 8;
  unsigned  cnt = 0;
  for (int y = 0; y < iH; y++) {
    for (int x = 0; x < iW; x++) {
      dat_in_orig[cnt] = rarray[cnt];
      cnt++;
    }
  }

  cnt = 0;
  pixelBus pixels;
  for (int y = 0; y < iH; y++) {
    for (int x = 0; x < iW/BUS_WORDS; x++) {
      for(int i=0; i<BUS_WORDS; i++){
        pixels.data[i] = rarray[cnt];
        cnt++;
      }
      dat_in.write(pixels); // just using red component (pseudo monochrome)
    }
  }

  cout << "Running" << endl;
#ifdef CCS_SCVERIFY
  iH = iW = 128;
#endif
  inst0.run(dat_in_orig,magn_orig,angle_orig,iW,iH);
  inst1.run(dat_in,magn_angle,iW,iH);

  cnt = 0;
  float sumErr = 0;
  float sumAngErr = 0;
 
 for (int y = 0; y < iH; y++) {
    for (int x = 0; x < iW/BUS_WORDS; x++) {
      magAngBus ma = magn_angle.read();
      for(int i=0; i<BUS_WORDS; i++){
        int hw = ma.data0[i].to_int();
        int alg = (int)*(magn_orig+cnt);
        int diff = alg-(hw);
        int adiff = abs(diff);
        sumErr += adiff;
        float angO = (double)*(angle_orig+cnt);
        angType angHw;
        angHw = ma.data1[i];
        float angAdiff = abs(angO-angHw.to_double());
        sumAngErr += angAdiff;
        cnt++;
        rarray[cnt] = hw;   // repurposing 'red' array to the bit-accurate monochrome edge-detect output
        garray[cnt] = alg;  // repurposing 'green' array to the original algorithmic edge-detect output
      }
    }
  }

  printf("Magnitude: average error per pixel %f\n",sumErr/(iH*iW));
  printf("Angle: average per pixel %f\n",sumAngErr/(iH*iW));

  cout << "Writing algorithmic bitmap output to: " << bmpAlg << endl;
  bmp_24_write((char*)bmpAlg.c_str(), iW,  iH, garray, garray, garray);
 
#ifdef DEBUG
  cmd = "display ";
  cmd.append(bmpAlg.c_str());
  std::cout << "Display output image file using command: " << cmd << endl;
  std::system(cmd.c_str());
#endif

  cout << "Writing bit-accurate bitmap output to: " << bmpBA << endl;
  bmp_24_write((char*)bmpBA.c_str(), iW,  iH, rarray, rarray, rarray);

#ifdef DEBUG
  cmd = "display ";
  cmd.append(bmpBA.c_str());
  std::cout << "Display output image file using command: " << cmd << endl;
  std::system(cmd.c_str());
#endif

  delete (dat_in_orig);
  delete (magn_orig);
  delete (angle_orig);
  delete (rarray);
  delete (garray);
  delete (barray);

  cout << "Finished" << endl;

  CCS_RETURN(0);
}
