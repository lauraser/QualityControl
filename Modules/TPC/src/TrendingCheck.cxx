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
/// \file   TrendingCheck.cxx
/// \author Laura Serksnyte
///
#include <iostream>

#include "TPC/TrendingCheck.h"
#include "QualityControl/MonitorObject.h"
#include "QualityControl/Quality.h"

#include <fairlogger/Logger.h>
// ROOT
#include <TCanvas.h>
#include <TGraph.h>
#include <TList.h>
#include <TPaveText.h>

using namespace std;

namespace o2::quality_control_modules::tpc
{

void TrendingCheck::configure(std::string) {}

Quality TrendingCheck::check(std::map<std::string, std::shared_ptr<MonitorObject>>* moMap)
{
  auto mo = moMap->begin()->second;
  Quality result = Quality::Null;
  std::cout<<"START IF STATEMENT"<<std::endl;

  if (mo->getName() == "TPCncl_StatMean_Trend_AfterCuts") {
    auto* canv = dynamic_cast<TCanvas*>(mo->getObject());
    //canv->ls();
    TGraph *h = (TGraph*)canv->GetListOfPrimitives()->FindObject("Graph");
    float NBins = h->GetN();
    std::cout<<NBins<<std::endl;
    double mean = h->GetMean(2);
    double RMS = h->GetRMS(2);
    std::cout<<"mean: "<<mean<<" RMS:"<<RMS<<std::endl;
    for( int i = 0;i<NBins;i++){
      double x=0., y=0.;
      h->GetPoint(i,x,y);
      std::cout<<i<<" x:"<<x<<"  y:"<<y<<std::endl;
    }
    double xlast=0., ylast=0.;
    h->GetPoint(NBins-1,xlast,ylast);
    if((ylast-mean)<RMS*3.){
      result = Quality::Medium;
    }
    else{
      result = Quality::Bad;
    }
  }
  std::cout<<"FINISH IF STATEMENT"<<std::endl;



  return result;
}

std::string TrendingCheck::getAcceptedType() { return "TCanvas"; }

void TrendingCheck::beautify(std::shared_ptr<MonitorObject> mo, Quality checkResult)
{
  //std::cout<<"START TRENDING"<<std::endl;
  auto* c1 = dynamic_cast<TCanvas*>(mo->getObject());
  TGraph *h = (TGraph*)c1->GetListOfPrimitives()->FindObject("Graph");
  //std::cout<<"AFTER DYNAMIC CAST"<<std::endl;
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

  std::cout<<"FINISH TRENDING"<<std::endl;
}
  

} // namespace o2::quality_control_modules::tpc
