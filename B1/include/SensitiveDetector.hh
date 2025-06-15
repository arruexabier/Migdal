#ifndef SENSITIVE_DETECTOR_HH
#define SENSITIVE_DETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "G4ThreeVector.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "RunAction.hh"

namespace B1 {
  class SensitiveDetector : public G4VSensitiveDetector {
  public:
      SensitiveDetector(const G4String& name, RunAction* runAction);
      virtual ~SensitiveDetector();

      virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;

  private:
      RunAction* fRunAction;
  };
}

#endif

