/// \file B1/src/DetectorConstruction.cc
/// \brief Implementation of the B1::DetectorConstruction class

#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Isotope.hh"

namespace B1
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
DetectorConstruction::DetectorConstruction(RunAction* runAction)
    : fRunAction(runAction) {
	DefineMaterials();
}

DetectorConstruction::~DetectorConstruction()
{}

void DetectorConstruction::DefineMaterials()
{
	G4NistManager *nist = G4NistManager::Instance();
	
	//------------------------XENON---------------
	// Pressure and temperature conditions
	G4double pressure = 1.0 * bar;         // 10 bar
	G4double temperature = 293.15 * kelvin; // Room temperature (20°C)

	// Molar mass of Xenon
	G4double molarMass = 131.3 * g/mole;
	// Calculate density using ideal gas law: rho = (P * M) / (R * T)
	G4double density = (pressure * molarMass) / (8.314 * joule/mole/kelvin * temperature); // g/cm³

	// Define Gaseous Xenon material
	xenonGas = new G4Material("GaseousXenon", 
											  density, 
											  1,                   // Number of components (single element)
											  kStateGas,           // State: Gas
											  temperature, 
											  pressure);

	// Add the element Xenon to the material
	G4Element* Xe = new G4Element("Xenon", "Xe", 54., molarMass);
	xenonGas->AddElement(Xe, 1.0); // 100% Xenon
	
	
	
  //------------------WORLD--------------
  world_mat = nist->FindOrBuildMaterial("G4_Galactic");
	
		
		
  //------------------STEEL------------
  acero = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
	
	



  //-----------------ARGON----------
	// Molar mass of Argon
	G4double molarMassAr = 39.95 * g/mole;

	// Calculate density using ideal gas law: rho = (P * M) / (R * T)
	G4double densityAr = (pressure * molarMassAr) / (8.314 * joule/mole/kelvin * temperature); // g/cm³

	// Define Gaseous Argon material
	argonGas = new G4Material("GaseousArgon", 
											  densityAr, 
											  1,                   // Number of components (single element)
											  kStateGas,           // State: Gas
											  temperature, 
											  pressure);

	// Add the element Argon to the material
	G4Element* Ar = new G4Element("Argon", "Ar", 18., molarMassAr);
	argonGas->AddElement(Ar, 1.0); // 100% Argon


	
	
  //--------------------XENON136-------

	// Molar mass of Xenon-136 (in g/mol)
	G4double molarMass136 = 135.907 * g/mole;

	// Calculate density using ideal gas law: rho = (P * M) / (R * T)
	G4double density136 = (pressure * molarMass136) / (8.314 * joule/mole/kelvin * temperature); // g/cm³

	// Define Xenon-136 isotope
	G4Isotope* Xe136 = new G4Isotope("Xe136", 54, 136, molarMass136);

	// Define an element containing only Xenon-136
	G4Element* elXe136 = new G4Element("Xenon136", "Xe", 1);
	elXe136->AddIsotope(Xe136, 1.0); // 100% Xe-136

	// Define the material for gaseous Xenon-136
	gaseousXe136 = new G4Material("GaseousXe136", 
											  density136, 
											  1,                   // Number of components
											  kStateGas,           // State: Gas
											  temperature, 
											  pressure);

	// Add the Xenon-136 element to the material
	gaseousXe136->AddElement(elXe136, 1.0); // 100% Xenon-136
	
	
	
	detMaterial=argonGas;
	
	
	
	
}




G4VPhysicalVolume* DetectorConstruction::Construct()
{

  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;
  
  G4double world_sizeXY = 1*m;
  G4double world_sizeZ  = 1*m;
  auto solidWorld = new G4Box("World",                           // its name
    0.5 * world_sizeXY, 0.5 * world_sizeXY, 0.5 * world_sizeZ);  // its size
  auto logicWorld = new G4LogicalVolume(solidWorld,  // its solid
    world_mat,                                       // its material
    "World");                                        // its name

  auto physWorld = new G4PVPlacement(nullptr,  // no rotation
    G4ThreeVector(),                           // at (0,0,0)
    logicWorld,                                // its logical volume
    "World",                                   // its name
    nullptr,                                   // its mother  volume
    false,                                     // no boolean operation
    0,                                         // copy number
    checkOverlaps);                            // overlaps checking
    
    
    
  //
  // Shape 1: GAS CILINDER
  //
  G4ThreeVector pos = G4ThreeVector(0,0,0);

  // CYLLINDER
  auto solidShape = new G4Tubs("Shape1",  // its name
    0.,11*cm,15*cm,0*degree,360*degree);  // its size

  logicShape = new G4LogicalVolume(solidShape,  // its solid
    detMaterial,                                        // its material
    "Shape1");                                         // its name

  new G4PVPlacement(nullptr,  // no rotation
    pos,                     // at position
    logicShape,              // its logical volume
    "Shape1",                 // its name
    logicWorld,                 // its mother  volume
    false,                    // no boolean operation
    0,                        // copy number
    checkOverlaps);           // overlaps checking







  if(1==1){
  //CUBIERTA DE ACERO
  
  //TUBO
    auto SolidtuboAcero = new G4Tubs("TuboAcero",  // its name
    11*cm,11*cm+8*mm,15*cm,0*degree,360*degree);  // its size

   auto logicTuboAcero = new G4LogicalVolume(SolidtuboAcero,  // its solid
    acero,                                        // its material
    "TuboAcero");                                         // its name

  new G4PVPlacement(nullptr,  // no rotation
    pos,                     // at position
    logicTuboAcero,              // its logical volume
    "TuboAcero",                 // its name
    logicWorld,                 // its mother  volume
    false,                    // no boolean operation
    0,                        // copy number
    checkOverlaps);           // overlaps checking
    
    
    
  //Tapas
    auto solidTapa = new G4Tubs("TapaAcero1",  // its name
    0*cm,11.8*cm,4*mm,0*degree,360*degree);  // its size

   auto logicTapa = new G4LogicalVolume(solidTapa,  // its solid
    acero,                                        // its material
    "TapaAcero1");                                         // its name

  new G4PVPlacement(nullptr,  // no rotation
    G4ThreeVector(0,0,-15.4*cm),     // at position
    logicTapa,              // its logical volume
    "TapaAcero1",                 // its name
    logicWorld,                 // its mother  volume
    false,                    // no boolean operation
    0,                        // copy number
    checkOverlaps);           // overlaps checking

  new G4PVPlacement(nullptr,  // no rotation
    G4ThreeVector(0,0,15.4*cm),     // at position
    logicTapa,              // its logical volume
    "TapaAcero2",                 // its name
    logicWorld,                 // its mother  volume
    false,                    // no boolean operation
    1,                        // copy number
    checkOverlaps);           // overlaps checking
}

  //
  //always return the physical World
  //
  return physWorld;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructSDandField() {
    SensitiveDetector* sensDet = new SensitiveDetector("MySensitiveDetector", fRunAction);
    G4SDManager::GetSDMpointer()->AddNewDetector(sensDet);
    
    //SENSITIVE DETECTOR IS GAS CILINDER
    logicShape->SetSensitiveDetector(sensDet);
}


}
