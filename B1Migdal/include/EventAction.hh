/// \file B1/include/EventAction.hh
/// \brief Definition of the B1::EventAction class

#ifndef B1EventAction_h
#define B1EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include <fstream>

/// Event action class
///

namespace B1
{

class RunAction;

class EventAction : public G4UserEventAction
{
  public:
    EventAction(RunAction* runAction);
    ~EventAction() override = default;

    void BeginOfEventAction(const G4Event* event) override;
    void EndOfEventAction(const G4Event* event) override;
    
    G4double GetTotalProb() const { return totalProb; }
    G4int GetEventCount() const { return eventCount; }
    void AddProb(G4double p) { totalProb += p; eventCount++; }



  private:
    RunAction* fRunAction = nullptr;
    
    std::ofstream fFile;
    
    G4double totalProb = 0.;
    G4int eventCount = 0;
};

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
