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
/// \file   Tracks.h
/// \author Stefan Heckel, sheckel@cern.ch
///

#ifndef QC_MODULE_TPC_Tracks_H
#define QC_MODULE_TPC_Tracks_H

// O2 includes
#include "TPCQC/Tracks.h"

// QC includes
#include "QualityControl/TaskInterface.h"

using namespace o2::quality_control::core;

namespace o2::quality_control_modules::tpc
{

/// \brief Quality Control DPL Task for QC Module TPC for track related observables
/// \author Stefan Heckel

class Tracks /*final*/ : public TaskInterface // todo add back the "final" when doxygen is fixed
{
 public:
  /// \brief Constructor
  Tracks() = default;
  /// Destructor
  ~Tracks() override = default;

  // Definition of the methods for the template method pattern
  void initialize(o2::framework::InitContext& ctx) override;
  void startOfActivity(Activity& activity) override;
  void startOfCycle() override;
  void monitorData(o2::framework::ProcessingContext& ctx) override;
  void endOfCycle() override;
  void endOfActivity(Activity& activity) override;
  void reset() override;

 private:
  o2::tpc::qc::Tracks mQCTracks{}; ///< TPC QC class from o2
};

} // namespace o2::quality_control_modules::tpc

#endif // QC_MODULE_TPC_Tracks_H
