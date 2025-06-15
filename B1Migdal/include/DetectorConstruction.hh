#ifndef DETECTORCONSTRUCTION_HH
#define DETECTORCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "globals.hh"
#include "SensitiveDetector.hh"  
#include "G4SDManager.hh" 
#include "RunAction.hh"       


namespace B1 {


class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    DetectorConstruction(RunAction* runAction);
    virtual ~DetectorConstruction();

    virtual G4VPhysicalVolume* Construct() override;
    virtual void ConstructSDandField() override;

private:
    void DefineMaterials();

private:
    RunAction* fRunAction;

    G4Material* world_mat;
    G4Material* acero;
    G4Material* xenonGas;
    G4Material* argonGas;
    G4Material* gaseousXe136;
    G4Material* detMaterial;

    G4LogicalVolume* logicShape;  // To assign sensitive detector later
};

}  // namespace B1

#endif

