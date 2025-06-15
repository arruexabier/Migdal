/// \file B1/src/EventAction.cc
/// \brief Implementation of the B1::EventAction class

#include "EventAction.hh"
#include "RunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include <fstream> // For file handling

namespace B1
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(RunAction* runAction)
: fRunAction(runAction)
{
	// Open the HDF5 file in write mode and write the header
    //fFile.open("seeds.hdf5");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* event)
{
    
    //long fEventSeedIndex = CLHEP::HepRandom::getTheSeed();
    //long fEventSeed1 = CLHEP::HepRandom::getTheSeeds()[0];
    //long fEventSeed2 = CLHEP::HepRandom::getTheSeeds()[1];
	//G4int eventID = event->GetEventID();
  
	//if (fFile.is_open()) {     
    //       fFile << std::right
    //        << std::setw(12) << eventID
    //        << std::setw(15) << fEventSeed1
    //        << std::setw(15) << fEventSeed2
    //        << std::setw(10)<< fEventSeedIndex
    //        << std::endl;
    //    }
        
    ////////////PARA PONER SEED DESEADA:
	//std::vector<long> fEventSeeds{124403585   ,   580854882  };
	//CLHEP::HepRandom::setTheSeeds(fEventSeeds.data(), 0);

    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event)
{
	//PRINT EVENT ID TO KNOW PROGESS
	G4int eventID = event->GetEventID();
	if (eventID % 10000 == 0) {
		G4cout << eventID << G4endl;
	}
}

} // namespace B1
