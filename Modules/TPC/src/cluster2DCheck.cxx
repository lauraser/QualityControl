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
/// \file   cluster2DCheck.cxx
/// \author Laura Serksnyte
///


#include "TPC/cluster2DCheck.h"
#include "QualityControl/MonitorObject.h"
#include "QualityControl/Quality.h"

#include <fairlogger/Logger.h>
// ROOT
#include <TCanvas.h>
#include <TH2.h>
#include <TPad.h>
#include <TList.h>
#include <TPaveText.h>

#include <iostream>

using namespace std;

namespace o2::quality_control_modules::tpc
{

void cluster2DCheck::configure(std::string) {}
Quality cluster2DCheck::check(std::map<std::string, std::shared_ptr<MonitorObject>>* moMap)
{
  auto mo = moMap->begin()->second;
  Quality result = Quality::Good;

  if (mo->getName() == "c_ROCs_N_Clusters_2D") {
    auto* canv = dynamic_cast<TCanvas*>(mo->getObject());

    for(int tpads = 1; tpads<=72; tpads++){
      auto padName  = fmt::format("c_ROCs_N_Clusters_2D_{:d}", tpads);
      auto histName  = fmt::format("h_N_Clusters_ROC_{:02d}", tpads-1);
      TPad *pad = (TPad*)canv->GetListOfPrimitives()->FindObject(padName.data());
      //pad->GetListOfPrimitives()->ls();
      TH2F *h = (TH2F*)pad->GetListOfPrimitives()->FindObject(histName.data());
      std::string titleh = h->GetTitle();
      bool iroc = false;
      bool oroc = false;
      std::cout<<"title "<<titleh<<std::endl;
      if (titleh.find("IROC") != std::string::npos) {
          iroc = true;
      }
      if (titleh.find("OROC") != std::string::npos) {
          oroc = true;
      }
      if(!(iroc||oroc)){
        return Quality::Null;
      }
      int NX = h->GetNbinsX();
      int NY = h->GetNbinsY();
      int totalBins = NX*NY;
      int totalON = 0;
      int totalOK = 0;

      if(iroc){
        totalON =  5504;
        totalOK =  totalBins - totalON;
      }
      if(oroc){
        totalON =  9856;
        totalOK =  totalBins - totalON;
      }
      int sum = 0;
      for(int i = 1; i<=NX; i++){
        for(int j = 1; j<=NY; j++){
          int val = h->GetBinContent(i,j);
          //std::cout<<i<<" "<<j<<" "<<val<<std::endl;
          if(val == 0){
            sum =sum+1;
          }
        }
      }
      
      int sumFinal = sum-totalOK;

      std::cout<<"THE SUM "<<sum<<" how many empty are  ok "<<totalOK<<" sum final "<<sumFinal<<std::endl;


      if(sumFinal>0.1*totalON){
        std::cout<<"more than 0.1*totalOn"<<std::endl;
        result = Quality::Bad;
        badSectorsName.push_back(titleh);
        badSectorsQuality.push_back( Quality::Bad);
      }
      else if(sumFinal>0){
        std::cout<<"more than 0"<<std::endl;
        if( result==Quality::Good){
          result = Quality::Medium;
        }
        badSectorsName.push_back(titleh);
        badSectorsQuality.push_back(Quality::Medium);
      }
    }


  }




  return result;
}

std::string cluster2DCheck::getAcceptedType() { return "TCanvas"; }

void cluster2DCheck::beautify(std::shared_ptr<MonitorObject> mo, Quality )
{

  auto* canv = dynamic_cast<TCanvas*>(mo->getObject());



  for(int tpads = 1; tpads<=72; tpads++){
    auto padName  = fmt::format("c_ROCs_N_Clusters_2D_{:d}", tpads);
    auto histName  = fmt::format("h_N_Clusters_ROC_{:02d}", tpads-1);
    TPad *pad = (TPad*)canv->GetListOfPrimitives()->FindObject(padName.data());
    pad->cd();
    TH2F *h = (TH2F*)pad->GetListOfPrimitives()->FindObject(histName.data());
    std::string titleh = h->GetTitle();
    std::vector<std::string>::iterator it = std::find(badSectorsName.begin(), badSectorsName.end(), titleh);
    if ( it != badSectorsName.end()) {
      TPaveText* msg = new TPaveText(0.5, 0.5, 0.9, 0.75, "NDC");
      int index = std::distance(badSectorsName.begin(), it);
      Quality qualitySpecial = badSectorsQuality[index];
      //h->GetListOfFunctions()->Add(msg);
      msg->SetName(Form("%s_msg", mo->GetName()));

      if (qualitySpecial == Quality::Good) {
        msg->Clear();
        msg->AddText("Quality::Good");
        msg->AddText("This is a fake test case.");
        msg->AddText("PLEASE IGNORE.");
        msg->SetFillColor(kGreen);
      } else if (qualitySpecial == Quality::Bad) {
        LOG(INFO) << "Quality::Bad, setting to red";
        msg->Clear();
        msg->AddText("Quality::Bad");
        msg->AddText("This is a fake test case.");
        msg->AddText("PLEASE IGNORE.");
        msg->SetFillColor(kRed);
      } else if (qualitySpecial == Quality::Medium) {
        LOG(INFO) << "Quality::medium, setting to orange";
        msg->Clear();
        msg->AddText("Quality::Medium");
        msg->AddText("This is a fake test case.");
        msg->AddText("PLEASE IGNORE.");
        msg->SetFillColor(kOrange);
      } else if (qualitySpecial == Quality::Null) {
        h->SetFillColor(0);
      }
      h->SetLineColor(kBlack);

      msg->Draw("same");

      std::cout<<"FINISH TRENDING"<<std::endl;
    }

    //}
  }

}
  

} // namespace o2::quality_control_modules::tpc
