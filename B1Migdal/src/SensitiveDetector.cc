#include "SensitiveDetector.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"

namespace B1 { 

SensitiveDetector::SensitiveDetector(const G4String& name, RunAction* runAction)
    : G4VSensitiveDetector(name), fRunAction(runAction) {}

SensitiveDetector::~SensitiveDetector() {}

G4bool SensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*) {
    if (!fRunAction) return false;

    // Get event ID
    G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    // Get position and energy deposit
    G4ThreeVector position = step->GetPreStepPoint()->GetPosition() / CLHEP::cm;
    G4double energyDeposit = step->GetTotalEnergyDeposit() / CLHEP::keV;

    // Only store meaningful hits
    if (energyDeposit > 0) {
        fRunAction->WriteStepData(eventID, position, energyDeposit);
    }

    return true;
}

}  

