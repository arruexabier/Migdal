#ifndef B1_RUNACTION_HH
#define B1_RUNACTION_HH

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include <H5Cpp.h>

namespace B1
{

class RunAction : public G4UserRunAction
{
public:
    RunAction();
    virtual ~RunAction();

    virtual void BeginOfRunAction(const G4Run*) override;
    virtual void EndOfRunAction(const G4Run*) override;

    void WriteStepData(G4int eventID, G4ThreeVector position, G4double energy);

private:
    H5::H5File* file;
    H5::DataSet* dataset;
    H5::DataSpace* dataspace;

    hsize_t currentSize;
    const hsize_t maxSize;
};

} // namespace B1

#endif // B1_RUNACTION_HH

