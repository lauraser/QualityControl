// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

///
/// \file   PadCalibrationCheck.cxx
/// \author Laura Serksnyte
///
#include <iostream>

#include "TPC/PadCalibrationCheck.h"
#include "QualityControl/MonitorObject.h"
#include "QualityControl/Quality.h"

#include <fairlogger/Logger.h>
// ROOT
#include <TH1.h>
#include <TList.h>
#include <TPaveText.h>

using namespace std;

namespace o2::quality_control_modules::tpc
{

void PadCalibrationCheck::configure(std::string) {}

Quality PadCalibrationCheck::check(std::map<std::string, std::shared_ptr<MonitorObject>>* moMap)
{

  Quality result = Quality::Null;
  for (auto& [moName, mo] : *moMap) {
    (void)moName;
    if (mo->getName() == "c_Sides_Pedestal") {
      
    }
    if (mo->getName() == "c_Sides_Noise") {

    }
  }

  return result;
}

std::string PadCalibrationCheck::getAcceptedType() { return "TH1"; }

void PadCalibrationCheck::beautify(std::shared_ptr<MonitorObject> mo, Quality checkResult)
{
  auto* h = dynamic_cast<TH1F*>(mo->getObject());

  TPaveText* msg = new TPaveText(0.5, 0.5, 0.9, 0.75, "NDC");
  h->GetListOfFunctions()->Add(msg);
  msg->SetName(Form("%s_msg", mo->GetName()));

  if (checkResult == Quality::Good) {
    h->SetFillColor(kGreen);
    msg->Clear();
    msg->AddText("Quality::Good");
    msg->AddText("This is a fake test case.");
    msg->AddText("PLEASE IGNORE.");
    msg->SetFillColor(kGreen);
  } else if (checkResult == Quality::Bad) {
    LOG(INFO) << "Quality::Bad, setting to red";
    h->SetFillColor(kRed);
    msg->Clear();
    msg->AddText("Quality::Bad");
    msg->AddText("This is a fake test case.");
    msg->AddText("PLEASE IGNORE.");
    msg->SetFillColor(kRed);
  } else if (checkResult == Quality::Medium) {
    LOG(INFO) << "Quality::medium, setting to orange";
    h->SetFillColor(kOrange);
    msg->Clear();
    msg->AddText("Quality::Medium");
    msg->AddText("This is a fake test case.");
    msg->AddText("PLEASE IGNORE.");
    msg->SetFillColor(kOrange);
  } else if (checkResult == Quality::Null) {
    h->SetFillColor(0);
  }
  h->SetLineColor(kBlack);
}

} // namespace o2::quality_control_modules::tpc
