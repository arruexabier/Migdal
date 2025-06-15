/// \file B1/include/PrimaryGeneratorAction.hh
/// \brief Definition of the B1::PrimaryGeneratorAction class

#ifndef B1PrimaryGeneratorAction_h
#define B1PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"
#include "G4PhysicsOrderedFreeVector.hh"
#include "G4RandomDirection.hh"

class G4ParticleGun;
class G4Event;
class G4ParticleDefinition;

namespace B1
{


class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction();
    ~PrimaryGeneratorAction() override;

    // method from the base class
    void GeneratePrimaries(G4Event*) override;

    // method to access particle gun
    const G4ParticleGun* GetParticleGun() const { return fParticleGun; }

  private:
    G4ParticleGun* fParticleGun = nullptr;  // pointer a to G4 gun class
    
    void defineXenon();
    
    G4double MigdalEnergy,En,m_n,m_nucleus,m_e,u,binding_energy,K_alpha,K_beta,kAlphaProb;
    G4double E_R;
    G4int Z,A;
    
    G4PhysicsOrderedFreeVector* ME_EnergyCDF;
};

}  // namespace B1

#endif
