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
 *  @file SDARecoOffsetsCalc.h
 *  @brief Calculates offsets for each JPetRecoSignal from SDA
 *  Reads a TTree of JPetRecoSignals and calculates is offset value. For more info
 *  look into Monika Pawlik-Niedzwiecka master thesis. 
 */

#ifndef _JPETANALYSISMODULE_SDARECOOFFSETCALC_H_
#define _JPETANALYSISMODULE_SDARECOOFFSETCALC_H_

#include <TCanvas.h>
#include "../../JPetTask/JPetTask.h"
#include "../../JPetWriter/JPetWriter.h"
class SDARecoOffsetsCalc: public JPetTask{
public:
	SDARecoOffsetsCalc(const char* name, const char* title);
	virtual ~SDARecoOffsetsCalc();
	virtual void init(const JPetTaskInterface::Options&)override;
	virtual void exec()override;
	virtual void terminate()override;
	virtual void setWriter(JPetWriter* writer)override;
private:
	JPetWriter* fWriter;
	int fCurrentEventNumber;
	double fOffset;
	int fBadSignals;
};

#endif
