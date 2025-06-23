#include "PrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4IonTable.hh"
#include "G4PhysicalConstants.hh"

using namespace B1;

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
    fParticleGun = new G4ParticleGun(1); // One particle per call
    fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., 0.)); //all migdal in center
    En=565*keV;
    m_e = 0.511 * MeV; 
    
    usexenon=true;
    if (usexenon){
    	defineXenon();
    }else{
    	defineArgon();
    }
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fParticleGun;
}


void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    // COMPUTE ENERGIES OF ELECTRON AND NUCLEAR RECOIL
    while(true){ //método HELENA
    	MigdalEnergy = ME_EnergyCDF->Value(G4UniformRand()); //ENERGY OF MIGDAL ELECTRON
    	G4double E_Rmax=4*u*u/(m_n*m_nucleus) *En;
    	G4double delta_E=MigdalEnergy+binding_energy;
	G4double root_term=(1-(delta_E*m_n/u/En));
	if (root_term < 0) continue;
	G4double root=sqrt(root_term);
	G4double cosz=2*(G4UniformRand()-0.5);    //random between -1 and 1
	E_R = E_Rmax/4 * ((1-root)*(1-root)+2*(1-cosz)*root);
	G4double E_R_actual = E_Rmax/4 * ((1-root)*(1-root)+4);
	if(G4UniformRand()<(E_R/E_R_actual)){
		G4double qe=sqrt(2*511*511*E_R/m_nucleus);  //keV
		prob=migdal_branch*qe*qe/0.511/0.511;
		break;
		}
    }
    
    
    //1.MIGDAL ELECTRON
    G4ThreeVector electronDirection = G4RandomDirection(); // isotropic
    G4ParticleDefinition* electronDef = G4ParticleTable::GetParticleTable()->FindParticle("e-");
    fParticleGun->SetParticleDefinition(electronDef);
    fParticleGun->SetParticleEnergy(MigdalEnergy);
    fParticleGun->SetParticleMomentumDirection(electronDirection);
    fParticleGun->GeneratePrimaryVertex(anEvent);
    
    //2.Generate Recoiling Xenon Nucleus
    G4ThreeVector recoilDirection = G4RandomDirection();
    G4ParticleDefinition* ion = G4IonTable::GetIonTable()->GetIon(Z, A, 0.0); // ground state
    fParticleGun->SetParticleDefinition(ion);
    fParticleGun->SetParticleEnergy(E_R);
    fParticleGun->SetParticleMomentumDirection(recoilDirection);
    fParticleGun->GeneratePrimaryVertex(anEvent);
    
     //3.Emit X-ray in opposite direction
     G4ParticleDefinition* xray = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
     fParticleGun->SetParticleDefinition(xray);
     fParticleGun->SetParticleMomentumDirection(G4RandomDirection());
     G4double X_ray_energy = (G4UniformRand() < kAlphaProb) ? K_alpha : K_beta; //x-ray energy
     fParticleGun->SetParticleEnergy(X_ray_energy);
     fParticleGun->GeneratePrimaryVertex(anEvent);
     
     //4.De-excitation electron
     G4ParticleDefinition* de_electron = G4ParticleTable::GetParticleTable()->FindParticle("e-");
     fParticleGun->SetParticleDefinition(de_electron);
     fParticleGun->SetParticleMomentumDirection(G4RandomDirection());
     fParticleGun->SetParticleEnergy(binding_energy-X_ray_energy);
     fParticleGun->GeneratePrimaryVertex(anEvent);
    
    
    
}




























void PrimaryGeneratorAction::defineXenon(){
  m_nucleus = 131.0 * 931.5 * MeV;          // Xenon nucleus mass
  m_n=939.565* MeV;
  u=m_n*m_nucleus/(m_n+m_nucleus);
  binding_energy = 34.6 * keV;        // K-shell binding energy
  K_alpha = 29.46 * keV;
  K_beta  = 33.64 * keV;
  kAlphaProb=0.8;
  fluor_yield=0.89;
  migdal_branch=4.6e-6;
  Z=54;
  A=131;
  
  	//XENON MIGDAL IONIZATION
  	ME_EnergyCDF = new G4PhysicsOrderedFreeVector();
	ME_EnergyCDF->InsertValues(0.00000395, 0.001000*keV);
	ME_EnergyCDF->InsertValues(0.00000798, 0.001046*keV);
	ME_EnergyCDF->InsertValues(0.00001220, 0.001093*keV);
	ME_EnergyCDF->InsertValues(0.00001662, 0.001143*keV);
	ME_EnergyCDF->InsertValues(0.00002124, 0.001195*keV);
	ME_EnergyCDF->InsertValues(0.00002607, 0.001250*keV);
	ME_EnergyCDF->InsertValues(0.00003113, 0.001307*keV);
	ME_EnergyCDF->InsertValues(0.00003641, 0.001367*keV);
	ME_EnergyCDF->InsertValues(0.00004194, 0.001429*keV);
	ME_EnergyCDF->InsertValues(0.00004772, 0.001494*keV);
	ME_EnergyCDF->InsertValues(0.00005377, 0.001562*keV);
	ME_EnergyCDF->InsertValues(0.00006010, 0.001634*keV);
	ME_EnergyCDF->InsertValues(0.00006672, 0.001708*keV);
	ME_EnergyCDF->InsertValues(0.00007365, 0.001786*keV);
	ME_EnergyCDF->InsertValues(0.00008089, 0.001868*keV);
	ME_EnergyCDF->InsertValues(0.00008847, 0.001953*keV);
	ME_EnergyCDF->InsertValues(0.00009640, 0.002042*keV);
	ME_EnergyCDF->InsertValues(0.00010469, 0.002135*keV);
	ME_EnergyCDF->InsertValues(0.00011337, 0.002233*keV);
	ME_EnergyCDF->InsertValues(0.00012244, 0.002335*keV);
	ME_EnergyCDF->InsertValues(0.00013194, 0.002441*keV);
	ME_EnergyCDF->InsertValues(0.00014187, 0.002553*keV);
	ME_EnergyCDF->InsertValues(0.00015227, 0.002669*keV);
	ME_EnergyCDF->InsertValues(0.00016314, 0.002791*keV);
	ME_EnergyCDF->InsertValues(0.00017451, 0.002918*keV);
	ME_EnergyCDF->InsertValues(0.00018641, 0.003051*keV);
	ME_EnergyCDF->InsertValues(0.00019887, 0.003191*keV);
	ME_EnergyCDF->InsertValues(0.00021189, 0.003336*keV);
	ME_EnergyCDF->InsertValues(0.00022552, 0.003489*keV);
	ME_EnergyCDF->InsertValues(0.00023978, 0.003648*keV);
	ME_EnergyCDF->InsertValues(0.00025470, 0.003814*keV);
	ME_EnergyCDF->InsertValues(0.00027031, 0.003988*keV);
	ME_EnergyCDF->InsertValues(0.00028664, 0.004170*keV);
	ME_EnergyCDF->InsertValues(0.00030373, 0.004361*keV);
	ME_EnergyCDF->InsertValues(0.00032160, 0.004560*keV);
	ME_EnergyCDF->InsertValues(0.00034030, 0.004768*keV);
	ME_EnergyCDF->InsertValues(0.00035987, 0.004985*keV);
	ME_EnergyCDF->InsertValues(0.00038034, 0.005213*keV);
	ME_EnergyCDF->InsertValues(0.00040176, 0.005451*keV);
	ME_EnergyCDF->InsertValues(0.00042417, 0.005699*keV);
	ME_EnergyCDF->InsertValues(0.00044761, 0.005960*keV);
	ME_EnergyCDF->InsertValues(0.00047213, 0.006232*keV);
	ME_EnergyCDF->InsertValues(0.00049779, 0.006516*keV);
	ME_EnergyCDF->InsertValues(0.00052462, 0.006813*keV);
	ME_EnergyCDF->InsertValues(0.00055269, 0.007124*keV);
	ME_EnergyCDF->InsertValues(0.00058206, 0.007449*keV);
	ME_EnergyCDF->InsertValues(0.00061277, 0.007789*keV);
	ME_EnergyCDF->InsertValues(0.00064489, 0.008145*keV);
	ME_EnergyCDF->InsertValues(0.00067848, 0.008516*keV);
	ME_EnergyCDF->InsertValues(0.00071361, 0.008905*keV);
	ME_EnergyCDF->InsertValues(0.00075034, 0.009311*keV);
	ME_EnergyCDF->InsertValues(0.00078875, 0.009736*keV);
	ME_EnergyCDF->InsertValues(0.00082890, 0.010181*keV);
	ME_EnergyCDF->InsertValues(0.00087088, 0.010645*keV);
	ME_EnergyCDF->InsertValues(0.00091477, 0.011131*keV);
	ME_EnergyCDF->InsertValues(0.00096064, 0.011639*keV);
	ME_EnergyCDF->InsertValues(0.00100858, 0.012170*keV);
	ME_EnergyCDF->InsertValues(0.00105868, 0.012726*keV);
	ME_EnergyCDF->InsertValues(0.00111103, 0.013306*keV);
	ME_EnergyCDF->InsertValues(0.00116573, 0.013914*keV);
	ME_EnergyCDF->InsertValues(0.00122288, 0.014549*keV);
	ME_EnergyCDF->InsertValues(0.00128259, 0.015213*keV);
	ME_EnergyCDF->InsertValues(0.00134496, 0.015907*keV);
	ME_EnergyCDF->InsertValues(0.00141012, 0.016633*keV);
	ME_EnergyCDF->InsertValues(0.00147818, 0.017392*keV);
	ME_EnergyCDF->InsertValues(0.00154928, 0.018186*keV);
	ME_EnergyCDF->InsertValues(0.00162356, 0.019015*keV);
	ME_EnergyCDF->InsertValues(0.00170117, 0.019883*keV);
	ME_EnergyCDF->InsertValues(0.00178225, 0.020791*keV);
	ME_EnergyCDF->InsertValues(0.00186696, 0.021739*keV);
	ME_EnergyCDF->InsertValues(0.00195547, 0.022731*keV);
	ME_EnergyCDF->InsertValues(0.00204796, 0.023769*keV);
	ME_EnergyCDF->InsertValues(0.00214462, 0.024854*keV);
	ME_EnergyCDF->InsertValues(0.00224562, 0.025988*keV);
	ME_EnergyCDF->InsertValues(0.00235118, 0.027174*keV);
	ME_EnergyCDF->InsertValues(0.00246150, 0.028414*keV);
	ME_EnergyCDF->InsertValues(0.00257681, 0.029710*keV);
	ME_EnergyCDF->InsertValues(0.00269732, 0.031066*keV);
	ME_EnergyCDF->InsertValues(0.00282328, 0.032484*keV);
	ME_EnergyCDF->InsertValues(0.00295493, 0.033967*keV);
	ME_EnergyCDF->InsertValues(0.00309251, 0.035517*keV);
	ME_EnergyCDF->InsertValues(0.00323628, 0.037137*keV);
	ME_EnergyCDF->InsertValues(0.00338650, 0.038832*keV);
	ME_EnergyCDF->InsertValues(0.00354344, 0.040604*keV);
	ME_EnergyCDF->InsertValues(0.00370739, 0.042457*keV);
	ME_EnergyCDF->InsertValues(0.00387865, 0.044395*keV);
	ME_EnergyCDF->InsertValues(0.00405752, 0.046421*keV);
	ME_EnergyCDF->InsertValues(0.00424434, 0.048539*keV);
	ME_EnergyCDF->InsertValues(0.00443946, 0.050754*keV);
	ME_EnergyCDF->InsertValues(0.00464324, 0.053071*keV);
	ME_EnergyCDF->InsertValues(0.00485609, 0.055493*keV);
	ME_EnergyCDF->InsertValues(0.00507843, 0.058025*keV);
	ME_EnergyCDF->InsertValues(0.00531069, 0.060673*keV);
	ME_EnergyCDF->InsertValues(0.00555333, 0.063442*keV);
	ME_EnergyCDF->InsertValues(0.00580684, 0.066337*keV);
	ME_EnergyCDF->InsertValues(0.00607172, 0.069364*keV);
	ME_EnergyCDF->InsertValues(0.00634848, 0.072530*keV);
	ME_EnergyCDF->InsertValues(0.00663767, 0.075840*keV);
	ME_EnergyCDF->InsertValues(0.00693983, 0.079301*keV);
	ME_EnergyCDF->InsertValues(0.00725554, 0.082920*keV);
	ME_EnergyCDF->InsertValues(0.00758537, 0.086704*keV);
	ME_EnergyCDF->InsertValues(0.00792995, 0.090661*keV);
	ME_EnergyCDF->InsertValues(0.00828991, 0.094798*keV);
	ME_EnergyCDF->InsertValues(0.00866592, 0.099124*keV);
	ME_EnergyCDF->InsertValues(0.00905868, 0.103648*keV);
	ME_EnergyCDF->InsertValues(0.00946895, 0.108378*keV);
	ME_EnergyCDF->InsertValues(0.00989751, 0.113324*keV);
	ME_EnergyCDF->InsertValues(0.01034519, 0.118496*keV);
	ME_EnergyCDF->InsertValues(0.01081286, 0.123903*keV);
	ME_EnergyCDF->InsertValues(0.01130143, 0.129558*keV);
	ME_EnergyCDF->InsertValues(0.01181183, 0.135470*keV);
	ME_EnergyCDF->InsertValues(0.01234506, 0.141652*keV);
	ME_EnergyCDF->InsertValues(0.01290211, 0.148117*keV);
	ME_EnergyCDF->InsertValues(0.01348405, 0.154876*keV);
	ME_EnergyCDF->InsertValues(0.01409198, 0.161944*keV);
	ME_EnergyCDF->InsertValues(0.01472702, 0.169334*keV);
	ME_EnergyCDF->InsertValues(0.01539038, 0.177062*keV);
	ME_EnergyCDF->InsertValues(0.01608331, 0.185142*keV);
	ME_EnergyCDF->InsertValues(0.01680708, 0.193591*keV);
	ME_EnergyCDF->InsertValues(0.01756307, 0.202426*keV);
	ME_EnergyCDF->InsertValues(0.01835267, 0.211664*keV);
	ME_EnergyCDF->InsertValues(0.01917736, 0.221323*keV);
	ME_EnergyCDF->InsertValues(0.02003867, 0.231424*keV);
	ME_EnergyCDF->InsertValues(0.02093821, 0.241985*keV);
	ME_EnergyCDF->InsertValues(0.02187765, 0.253028*keV);
	ME_EnergyCDF->InsertValues(0.02285871, 0.264575*keV);
	ME_EnergyCDF->InsertValues(0.02388317, 0.276649*keV);
	ME_EnergyCDF->InsertValues(0.02495290, 0.289274*keV);
	ME_EnergyCDF->InsertValues(0.02606977, 0.302476*keV);
	ME_EnergyCDF->InsertValues(0.02723579, 0.316279*keV);
	ME_EnergyCDF->InsertValues(0.02845303, 0.330713*keV);
	ME_EnergyCDF->InsertValues(0.02972368, 0.345805*keV);
	ME_EnergyCDF->InsertValues(0.03105004, 0.361586*keV);
	ME_EnergyCDF->InsertValues(0.03243446, 0.378087*keV);
	ME_EnergyCDF->InsertValues(0.03387938, 0.395342*keV);
	ME_EnergyCDF->InsertValues(0.03538724, 0.413383*keV);
	ME_EnergyCDF->InsertValues(0.03696056, 0.432248*keV);
	ME_EnergyCDF->InsertValues(0.03860198, 0.451974*keV);
	ME_EnergyCDF->InsertValues(0.04031424, 0.472600*keV);
	ME_EnergyCDF->InsertValues(0.04210026, 0.494168*keV);
	ME_EnergyCDF->InsertValues(0.04396306, 0.516720*keV);
	ME_EnergyCDF->InsertValues(0.04590572, 0.540300*keV);
	ME_EnergyCDF->InsertValues(0.04793137, 0.564957*keV);
	ME_EnergyCDF->InsertValues(0.05004324, 0.590740*keV);
	ME_EnergyCDF->InsertValues(0.05224467, 0.617698*keV);
	ME_EnergyCDF->InsertValues(0.05453911, 0.645887*keV);
	ME_EnergyCDF->InsertValues(0.05693011, 0.675363*keV);
	ME_EnergyCDF->InsertValues(0.05942137, 0.706184*keV);
	ME_EnergyCDF->InsertValues(0.06201669, 0.738411*keV);
	ME_EnergyCDF->InsertValues(0.06472003, 0.772109*keV);
	ME_EnergyCDF->InsertValues(0.06753548, 0.807344*keV);
	ME_EnergyCDF->InsertValues(0.07046726, 0.844188*keV);
	ME_EnergyCDF->InsertValues(0.07351965, 0.882713*keV);
	ME_EnergyCDF->InsertValues(0.07669697, 0.922996*keV);
	ME_EnergyCDF->InsertValues(0.08000370, 0.965118*keV);
	ME_EnergyCDF->InsertValues(0.08344454, 1.009162*keV);
	ME_EnergyCDF->InsertValues(0.08702432, 1.055216*keV);
	ME_EnergyCDF->InsertValues(0.09074783, 1.103371*keV);
	ME_EnergyCDF->InsertValues(0.09461990, 1.153724*keV);
	ME_EnergyCDF->InsertValues(0.09864559, 1.206375*keV);
	ME_EnergyCDF->InsertValues(0.10283022, 1.261429*keV);
	ME_EnergyCDF->InsertValues(0.10717904, 1.318995*keV);
	ME_EnergyCDF->InsertValues(0.11169720, 1.379188*keV);
	ME_EnergyCDF->InsertValues(0.11639006, 1.442128*keV);
	ME_EnergyCDF->InsertValues(0.12126328, 1.507941*keV);
	ME_EnergyCDF->InsertValues(0.12632235, 1.576757*keV);
	ME_EnergyCDF->InsertValues(0.13157269, 1.648713*keV);
	ME_EnergyCDF->InsertValues(0.13701984, 1.723953*keV);
	ME_EnergyCDF->InsertValues(0.14266944, 1.802627*keV);
	ME_EnergyCDF->InsertValues(0.14852709, 1.884891*keV);
	ME_EnergyCDF->InsertValues(0.15459840, 1.970909*keV);
	ME_EnergyCDF->InsertValues(0.16088891, 2.060853*keV);
	ME_EnergyCDF->InsertValues(0.16740406, 2.154902*keV);
	ME_EnergyCDF->InsertValues(0.17414925, 2.253242*keV);
	ME_EnergyCDF->InsertValues(0.18112985, 2.356070*keV);
	ME_EnergyCDF->InsertValues(0.18835056, 2.463591*keV);
	ME_EnergyCDF->InsertValues(0.19581600, 2.576019*keV);
	ME_EnergyCDF->InsertValues(0.20353145, 2.693577*keV);
	ME_EnergyCDF->InsertValues(0.21150140, 2.816500*keV);
	ME_EnergyCDF->InsertValues(0.21972986, 2.945033*keV);
	ME_EnergyCDF->InsertValues(0.22822052, 3.079432*keV);
	ME_EnergyCDF->InsertValues(0.23697621, 3.219964*keV);
	ME_EnergyCDF->InsertValues(0.24600022, 3.366909*keV);
	ME_EnergyCDF->InsertValues(0.25529538, 3.520560*keV);
	ME_EnergyCDF->InsertValues(0.26486351, 3.681223*keV);
	ME_EnergyCDF->InsertValues(0.27470613, 3.849218*keV);
	ME_EnergyCDF->InsertValues(0.28482415, 4.024880*keV);
	ME_EnergyCDF->InsertValues(0.29521743, 4.208558*keV);
	ME_EnergyCDF->InsertValues(0.30588578, 4.400619*keV);
	ME_EnergyCDF->InsertValues(0.31682820, 4.601444*keV);
	ME_EnergyCDF->InsertValues(0.32804284, 4.811434*keV);
	ME_EnergyCDF->InsertValues(0.33952709, 5.031007*keV);
	ME_EnergyCDF->InsertValues(0.35127758, 5.260600*keV);
	ME_EnergyCDF->InsertValues(0.36328998, 5.500671*keV);
	ME_EnergyCDF->InsertValues(0.37555898, 5.751698*keV);
	ME_EnergyCDF->InsertValues(0.38807846, 6.014181*keV);
	ME_EnergyCDF->InsertValues(0.40084119, 6.288642*keV);
	ME_EnergyCDF->InsertValues(0.41383907, 6.575628*keV);
	ME_EnergyCDF->InsertValues(0.42706283, 6.875712*keV);
	ME_EnergyCDF->InsertValues(0.44050272, 7.189489*keV);
	ME_EnergyCDF->InsertValues(0.45414674, 7.517587*keV);
	ME_EnergyCDF->InsertValues(0.46798237, 7.860657*keV);
	ME_EnergyCDF->InsertValues(0.48199591, 8.219383*keV);
	ME_EnergyCDF->InsertValues(0.49617260, 8.594480*keV);
	ME_EnergyCDF->InsertValues(0.51049671, 8.986695*keV);
	ME_EnergyCDF->InsertValues(0.52495179, 9.396809*keV);
	ME_EnergyCDF->InsertValues(0.53951973, 9.825639*keV);
	ME_EnergyCDF->InsertValues(0.55418127, 10.274038*keV);
	ME_EnergyCDF->InsertValues(0.56891676, 10.742901*keV);
	ME_EnergyCDF->InsertValues(0.58370544, 11.233160*keV);
	ME_EnergyCDF->InsertValues(0.59852621, 11.745793*keV);
	ME_EnergyCDF->InsertValues(0.61335663, 12.281820*keV);
	ME_EnergyCDF->InsertValues(0.62817454, 12.842310*keV);
	ME_EnergyCDF->InsertValues(0.64295618, 13.428377*keV);
	ME_EnergyCDF->InsertValues(0.65767825, 14.041190*keV);
	ME_EnergyCDF->InsertValues(0.67231672, 14.681969*keV);
	ME_EnergyCDF->InsertValues(0.68684778, 15.351991*keV);
	ME_EnergyCDF->InsertValues(0.70124705, 16.052589*keV);
	ME_EnergyCDF->InsertValues(0.71549087, 16.785160*keV);
	ME_EnergyCDF->InsertValues(0.72955564, 17.551162*keV);
	ME_EnergyCDF->InsertValues(0.74341853, 18.352121*keV);
	ME_EnergyCDF->InsertValues(0.75705667, 19.189633*keV);
	ME_EnergyCDF->InsertValues(0.77044859, 20.065365*keV);
	ME_EnergyCDF->InsertValues(0.78357299, 20.981061*keV);
	ME_EnergyCDF->InsertValues(0.79641033, 21.938546*keV);
	ME_EnergyCDF->InsertValues(0.80894143, 22.939727*keV);
	ME_EnergyCDF->InsertValues(0.82114903, 23.986596*keV);
	ME_EnergyCDF->InsertValues(0.83301712, 25.081241*keV);
	ME_EnergyCDF->InsertValues(0.84453140, 26.225840*keV);
	ME_EnergyCDF->InsertValues(0.85567880, 27.422674*keV);
	ME_EnergyCDF->InsertValues(0.86644805, 28.674126*keV);
	ME_EnergyCDF->InsertValues(0.87682952, 29.982690*keV);
	ME_EnergyCDF->InsertValues(0.88681507, 31.350970*keV);
	ME_EnergyCDF->InsertValues(0.89639916, 32.781693*keV);
	ME_EnergyCDF->InsertValues(0.90557756, 34.277707*keV);
	ME_EnergyCDF->InsertValues(0.91434812, 35.841994*keV);
	ME_EnergyCDF->InsertValues(0.92270994, 37.477667*keV);
	ME_EnergyCDF->InsertValues(0.93066433, 39.187986*keV);
	ME_EnergyCDF->InsertValues(0.93821384, 40.976356*keV);
	ME_EnergyCDF->InsertValues(0.94536274, 42.846340*keV);
	ME_EnergyCDF->InsertValues(0.95211677, 44.801661*keV);
	ME_EnergyCDF->InsertValues(0.95848332, 46.846215*keV);
	ME_EnergyCDF->InsertValues(0.96447107, 48.984074*keV);
	ME_EnergyCDF->InsertValues(0.97008949, 51.219495*keV);
	ME_EnergyCDF->InsertValues(0.97534942, 53.556932*keV);
	ME_EnergyCDF->InsertValues(0.98026233, 56.001038*keV);
	ME_EnergyCDF->InsertValues(0.98484040, 58.556683*keV);
	ME_EnergyCDF->InsertValues(0.98909698, 61.228957*keV);
	ME_EnergyCDF->InsertValues(0.99304559, 64.023182*keV);
	ME_EnergyCDF->InsertValues(0.99670033, 66.944923*keV);
	ME_EnergyCDF->InsertValues(1.00000000, 70.000000*keV);
  
}


void PrimaryGeneratorAction::defineArgon(){
  m_nucleus = 39.9 * 931.5 * MeV;          // Argon nucleus mass
  m_n=939.565* MeV;
  u=m_n*m_nucleus/(m_n+m_nucleus);
  binding_energy = 3.2 * keV;        
  K_alpha = 2.96 * keV;              
  K_beta  = 3.19 * keV;              
  kAlphaProb = 0.85;                 
  fluor_yield = 0.12;  
  migdal_branch=7.2e-5;              
  Z = 18;                            
  A = 40; 


	ME_EnergyCDF = new G4PhysicsOrderedFreeVector();
	ME_EnergyCDF->InsertValues(0.00003841, 0.001000*keV);
	ME_EnergyCDF->InsertValues(0.00007772, 0.001046*keV);
	ME_EnergyCDF->InsertValues(0.00011884, 0.001093*keV);
	ME_EnergyCDF->InsertValues(0.00016187, 0.001143*keV);
	ME_EnergyCDF->InsertValues(0.00020687, 0.001195*keV);
	ME_EnergyCDF->InsertValues(0.00025396, 0.001250*keV);
	ME_EnergyCDF->InsertValues(0.00030322, 0.001307*keV);
	ME_EnergyCDF->InsertValues(0.00035476, 0.001367*keV);
	ME_EnergyCDF->InsertValues(0.00040869, 0.001429*keV);
	ME_EnergyCDF->InsertValues(0.00046510, 0.001494*keV);
	ME_EnergyCDF->InsertValues(0.00052413, 0.001562*keV);
	ME_EnergyCDF->InsertValues(0.00058589, 0.001634*keV);
	ME_EnergyCDF->InsertValues(0.00065052, 0.001708*keV);
	ME_EnergyCDF->InsertValues(0.00071813, 0.001786*keV);
	ME_EnergyCDF->InsertValues(0.00078888, 0.001868*keV);
	ME_EnergyCDF->InsertValues(0.00086290, 0.001953*keV);
	ME_EnergyCDF->InsertValues(0.00094036, 0.002042*keV);
	ME_EnergyCDF->InsertValues(0.00102141, 0.002135*keV);
	ME_EnergyCDF->InsertValues(0.00110621, 0.002233*keV);
	ME_EnergyCDF->InsertValues(0.00119496, 0.002335*keV);
	ME_EnergyCDF->InsertValues(0.00128781, 0.002441*keV);
	ME_EnergyCDF->InsertValues(0.00138498, 0.002553*keV);
	ME_EnergyCDF->InsertValues(0.00148665, 0.002669*keV);
	ME_EnergyCDF->InsertValues(0.00159305, 0.002791*keV);
	ME_EnergyCDF->InsertValues(0.00170438, 0.002918*keV);
	ME_EnergyCDF->InsertValues(0.00182087, 0.003051*keV);
	ME_EnergyCDF->InsertValues(0.00194277, 0.003191*keV);
	ME_EnergyCDF->InsertValues(0.00207033, 0.003336*keV);
	ME_EnergyCDF->InsertValues(0.00220380, 0.003489*keV);
	ME_EnergyCDF->InsertValues(0.00234347, 0.003648*keV);
	ME_EnergyCDF->InsertValues(0.00248961, 0.003814*keV);
	ME_EnergyCDF->InsertValues(0.00264252, 0.003988*keV);
	ME_EnergyCDF->InsertValues(0.00280251, 0.004170*keV);
	ME_EnergyCDF->InsertValues(0.00296991, 0.004361*keV);
	ME_EnergyCDF->InsertValues(0.00314506, 0.004560*keV);
	ME_EnergyCDF->InsertValues(0.00332831, 0.004768*keV);
	ME_EnergyCDF->InsertValues(0.00352004, 0.004985*keV);
	ME_EnergyCDF->InsertValues(0.00372062, 0.005213*keV);
	ME_EnergyCDF->InsertValues(0.00393047, 0.005451*keV);
	ME_EnergyCDF->InsertValues(0.00415000, 0.005699*keV);
	ME_EnergyCDF->InsertValues(0.00437965, 0.005960*keV);
	ME_EnergyCDF->InsertValues(0.00461988, 0.006232*keV);
	ME_EnergyCDF->InsertValues(0.00487118, 0.006516*keV);
	ME_EnergyCDF->InsertValues(0.00513403, 0.006813*keV);
	ME_EnergyCDF->InsertValues(0.00540896, 0.007124*keV);
	ME_EnergyCDF->InsertValues(0.00569650, 0.007449*keV);
	ME_EnergyCDF->InsertValues(0.00599723, 0.007789*keV);
	ME_EnergyCDF->InsertValues(0.00631172, 0.008145*keV);
	ME_EnergyCDF->InsertValues(0.00664059, 0.008516*keV);
	ME_EnergyCDF->InsertValues(0.00698447, 0.008905*keV);
	ME_EnergyCDF->InsertValues(0.00734403, 0.009311*keV);
	ME_EnergyCDF->InsertValues(0.00771997, 0.009736*keV);
	ME_EnergyCDF->InsertValues(0.00811301, 0.010181*keV);
	ME_EnergyCDF->InsertValues(0.00852390, 0.010645*keV);
	ME_EnergyCDF->InsertValues(0.00895345, 0.011131*keV);
	ME_EnergyCDF->InsertValues(0.00940245, 0.011639*keV);
	ME_EnergyCDF->InsertValues(0.00987177, 0.012170*keV);
	ME_EnergyCDF->InsertValues(0.01036229, 0.012726*keV);
	ME_EnergyCDF->InsertValues(0.01087493, 0.013306*keV);
	ME_EnergyCDF->InsertValues(0.01141065, 0.013914*keV);
	ME_EnergyCDF->InsertValues(0.01197042, 0.014549*keV);
	ME_EnergyCDF->InsertValues(0.01255529, 0.015213*keV);
	ME_EnergyCDF->InsertValues(0.01316632, 0.015907*keV);
	ME_EnergyCDF->InsertValues(0.01380462, 0.016633*keV);
	ME_EnergyCDF->InsertValues(0.01447133, 0.017392*keV);
	ME_EnergyCDF->InsertValues(0.01516766, 0.018186*keV);
	ME_EnergyCDF->InsertValues(0.01589485, 0.019015*keV);
	ME_EnergyCDF->InsertValues(0.01665419, 0.019883*keV);
	ME_EnergyCDF->InsertValues(0.01744705, 0.020791*keV);
	ME_EnergyCDF->InsertValues(0.01827485, 0.021739*keV);
	ME_EnergyCDF->InsertValues(0.01913910, 0.022731*keV);
	ME_EnergyCDF->InsertValues(0.02004138, 0.023769*keV);
	ME_EnergyCDF->InsertValues(0.02098337, 0.024854*keV);
	ME_EnergyCDF->InsertValues(0.02196682, 0.025988*keV);
	ME_EnergyCDF->InsertValues(0.02299355, 0.027174*keV);
	ME_EnergyCDF->InsertValues(0.02406543, 0.028414*keV);
	ME_EnergyCDF->InsertValues(0.02518439, 0.029710*keV);
	ME_EnergyCDF->InsertValues(0.02635238, 0.031066*keV);
	ME_EnergyCDF->InsertValues(0.02757140, 0.032484*keV);
	ME_EnergyCDF->InsertValues(0.02884343, 0.033967*keV);
	ME_EnergyCDF->InsertValues(0.03017049, 0.035517*keV);
	ME_EnergyCDF->InsertValues(0.03155470, 0.037137*keV);
	ME_EnergyCDF->InsertValues(0.03299824, 0.038832*keV);
	ME_EnergyCDF->InsertValues(0.03450343, 0.040604*keV);
	ME_EnergyCDF->InsertValues(0.03607271, 0.042457*keV);
	ME_EnergyCDF->InsertValues(0.03770873, 0.044395*keV);
	ME_EnergyCDF->InsertValues(0.03941430, 0.046421*keV);
	ME_EnergyCDF->InsertValues(0.04119238, 0.048539*keV);
	ME_EnergyCDF->InsertValues(0.04304602, 0.050754*keV);
	ME_EnergyCDF->InsertValues(0.04497833, 0.053071*keV);
	ME_EnergyCDF->InsertValues(0.04699247, 0.055493*keV);
	ME_EnergyCDF->InsertValues(0.04909160, 0.058025*keV);
	ME_EnergyCDF->InsertValues(0.05127895, 0.060673*keV);
	ME_EnergyCDF->InsertValues(0.05355786, 0.063442*keV);
	ME_EnergyCDF->InsertValues(0.05593181, 0.066337*keV);
	ME_EnergyCDF->InsertValues(0.05840439, 0.069364*keV);
	ME_EnergyCDF->InsertValues(0.06097937, 0.072530*keV);
	ME_EnergyCDF->InsertValues(0.06366061, 0.075840*keV);
	ME_EnergyCDF->InsertValues(0.06645203, 0.079301*keV);
	ME_EnergyCDF->InsertValues(0.06935762, 0.082920*keV);
	ME_EnergyCDF->InsertValues(0.07238136, 0.086704*keV);
	ME_EnergyCDF->InsertValues(0.07552727, 0.090661*keV);
	ME_EnergyCDF->InsertValues(0.07879953, 0.094798*keV);
	ME_EnergyCDF->InsertValues(0.08220263, 0.099124*keV);
	ME_EnergyCDF->InsertValues(0.08574139, 0.103648*keV);
	ME_EnergyCDF->InsertValues(0.08942094, 0.108378*keV);
	ME_EnergyCDF->InsertValues(0.09324628, 0.113324*keV);
	ME_EnergyCDF->InsertValues(0.09722213, 0.118496*keV);
	ME_EnergyCDF->InsertValues(0.10135264, 0.123903*keV);
	ME_EnergyCDF->InsertValues(0.10564178, 0.129558*keV);
	ME_EnergyCDF->InsertValues(0.11009377, 0.135470*keV);
	ME_EnergyCDF->InsertValues(0.11471329, 0.141652*keV);
	ME_EnergyCDF->InsertValues(0.11950563, 0.148117*keV);
	ME_EnergyCDF->InsertValues(0.12447611, 0.154876*keV);
	ME_EnergyCDF->InsertValues(0.12962982, 0.161944*keV);
	ME_EnergyCDF->InsertValues(0.13497159, 0.169334*keV);
	ME_EnergyCDF->InsertValues(0.14050608, 0.177062*keV);
	ME_EnergyCDF->InsertValues(0.14623792, 0.185142*keV);
	ME_EnergyCDF->InsertValues(0.15217172, 0.193591*keV);
	ME_EnergyCDF->InsertValues(0.15831202, 0.202426*keV);
	ME_EnergyCDF->InsertValues(0.16466328, 0.211664*keV);
	ME_EnergyCDF->InsertValues(0.17122985, 0.221323*keV);
	ME_EnergyCDF->InsertValues(0.17801596, 0.231424*keV);
	ME_EnergyCDF->InsertValues(0.18502572, 0.241985*keV);
	ME_EnergyCDF->InsertValues(0.19226304, 0.253028*keV);
	ME_EnergyCDF->InsertValues(0.19973168, 0.264575*keV);
	ME_EnergyCDF->InsertValues(0.20743520, 0.276649*keV);
	ME_EnergyCDF->InsertValues(0.21537692, 0.289274*keV);
	ME_EnergyCDF->InsertValues(0.22355994, 0.302476*keV);
	ME_EnergyCDF->InsertValues(0.23198698, 0.316279*keV);
	ME_EnergyCDF->InsertValues(0.24066028, 0.330713*keV);
	ME_EnergyCDF->InsertValues(0.24958172, 0.345805*keV);
	ME_EnergyCDF->InsertValues(0.25875308, 0.361586*keV);
	ME_EnergyCDF->InsertValues(0.26817598, 0.378087*keV);
	ME_EnergyCDF->InsertValues(0.27785134, 0.395342*keV);
	ME_EnergyCDF->InsertValues(0.28777935, 0.413383*keV);
	ME_EnergyCDF->InsertValues(0.29795968, 0.432248*keV);
	ME_EnergyCDF->InsertValues(0.30839155, 0.451974*keV);
	ME_EnergyCDF->InsertValues(0.31907358, 0.472600*keV);
	ME_EnergyCDF->InsertValues(0.33000379, 0.494168*keV);
	ME_EnergyCDF->InsertValues(0.34117963, 0.516720*keV);
	ME_EnergyCDF->InsertValues(0.35259778, 0.540300*keV);
	ME_EnergyCDF->InsertValues(0.36425393, 0.564957*keV);
	ME_EnergyCDF->InsertValues(0.37614291, 0.590740*keV);
	ME_EnergyCDF->InsertValues(0.38825868, 0.617698*keV);
	ME_EnergyCDF->InsertValues(0.40059432, 0.645887*keV);
	ME_EnergyCDF->InsertValues(0.41314192, 0.675363*keV);
	ME_EnergyCDF->InsertValues(0.42589247, 0.706184*keV);
	ME_EnergyCDF->InsertValues(0.43883567, 0.738411*keV);
	ME_EnergyCDF->InsertValues(0.45196021, 0.772109*keV);
	ME_EnergyCDF->InsertValues(0.46525399, 0.807344*keV);
	ME_EnergyCDF->InsertValues(0.47870361, 0.844188*keV);
	ME_EnergyCDF->InsertValues(0.49229416, 0.882713*keV);
	ME_EnergyCDF->InsertValues(0.50600968, 0.922996*keV);
	ME_EnergyCDF->InsertValues(0.51983386, 0.965118*keV);
	ME_EnergyCDF->InsertValues(0.53374905, 1.009162*keV);
	ME_EnergyCDF->InsertValues(0.54773619, 1.055216*keV);
	ME_EnergyCDF->InsertValues(0.56177564, 1.103371*keV);
	ME_EnergyCDF->InsertValues(0.57584753, 1.153724*keV);
	ME_EnergyCDF->InsertValues(0.58993091, 1.206375*keV);
	ME_EnergyCDF->InsertValues(0.60400437, 1.261429*keV);
	ME_EnergyCDF->InsertValues(0.61804588, 1.318995*keV);
	ME_EnergyCDF->InsertValues(0.63203336, 1.379188*keV);
	ME_EnergyCDF->InsertValues(0.64594469, 1.442128*keV);
	ME_EnergyCDF->InsertValues(0.65975708, 1.507941*keV);
	ME_EnergyCDF->InsertValues(0.67344979, 1.576757*keV);
	ME_EnergyCDF->InsertValues(0.68700005, 1.648713*keV);
	ME_EnergyCDF->InsertValues(0.70038591, 1.723953*keV);
	ME_EnergyCDF->InsertValues(0.71358665, 1.802627*keV);
	ME_EnergyCDF->InsertValues(0.72658167, 1.884891*keV);
	ME_EnergyCDF->InsertValues(0.73935183, 1.970909*keV);
	ME_EnergyCDF->InsertValues(0.75187744, 2.060853*keV);
	ME_EnergyCDF->InsertValues(0.76414036, 2.154902*keV);
	ME_EnergyCDF->InsertValues(0.77612344, 2.253242*keV);
	ME_EnergyCDF->InsertValues(0.78781066, 2.356070*keV);
	ME_EnergyCDF->InsertValues(0.79918820, 2.463591*keV);
	ME_EnergyCDF->InsertValues(0.81024193, 2.576019*keV);
	ME_EnergyCDF->InsertValues(0.82095972, 2.693577*keV);
	ME_EnergyCDF->InsertValues(0.83133105, 2.816500*keV);
	ME_EnergyCDF->InsertValues(0.84134714, 2.945033*keV);
	ME_EnergyCDF->InsertValues(0.85100103, 3.079432*keV);
	ME_EnergyCDF->InsertValues(0.86028639, 3.219964*keV);
	ME_EnergyCDF->InsertValues(0.86919905, 3.366909*keV);
	ME_EnergyCDF->InsertValues(0.87773647, 3.520560*keV);
	ME_EnergyCDF->InsertValues(0.88589764, 3.681223*keV);
	ME_EnergyCDF->InsertValues(0.89368315, 3.849218*keV);
	ME_EnergyCDF->InsertValues(0.90109550, 4.024880*keV);
	ME_EnergyCDF->InsertValues(0.90813767, 4.208558*keV);
	ME_EnergyCDF->InsertValues(0.91481438, 4.400619*keV);
	ME_EnergyCDF->InsertValues(0.92113158, 4.601444*keV);
	ME_EnergyCDF->InsertValues(0.92709638, 4.811434*keV);
	ME_EnergyCDF->InsertValues(0.93271726, 5.031007*keV);
	ME_EnergyCDF->InsertValues(0.93800312, 5.260600*keV);
	ME_EnergyCDF->InsertValues(0.94296384, 5.500671*keV);
	ME_EnergyCDF->InsertValues(0.94761009, 5.751698*keV);
	ME_EnergyCDF->InsertValues(0.95195309, 6.014181*keV);
	ME_EnergyCDF->InsertValues(0.95600460, 6.288642*keV);
	ME_EnergyCDF->InsertValues(0.95977681, 6.575628*keV);
	ME_EnergyCDF->InsertValues(0.96328216, 6.875712*keV);
	ME_EnergyCDF->InsertValues(0.96653323, 7.189489*keV);
	ME_EnergyCDF->InsertValues(0.96954269, 7.517587*keV);
	ME_EnergyCDF->InsertValues(0.97232323, 7.860657*keV);
	ME_EnergyCDF->InsertValues(0.97488747, 8.219383*keV);
	ME_EnergyCDF->InsertValues(0.97724784, 8.594480*keV);
	ME_EnergyCDF->InsertValues(0.97941664, 8.986695*keV);
	ME_EnergyCDF->InsertValues(0.98140582, 9.396809*keV);
	ME_EnergyCDF->InsertValues(0.98322704, 9.825639*keV);
	ME_EnergyCDF->InsertValues(0.98489152, 10.274038*keV);
	ME_EnergyCDF->InsertValues(0.98641008, 10.742901*keV);
	ME_EnergyCDF->InsertValues(0.98779324, 11.233160*keV);
	ME_EnergyCDF->InsertValues(0.98905096, 11.745793*keV);
	ME_EnergyCDF->InsertValues(0.99019272, 12.281820*keV);
	ME_EnergyCDF->InsertValues(0.99122755, 12.842310*keV);
	ME_EnergyCDF->InsertValues(0.99216396, 13.428377*keV);
	ME_EnergyCDF->InsertValues(0.99300994, 14.041190*keV);
	ME_EnergyCDF->InsertValues(0.99377307, 14.681969*keV);
	ME_EnergyCDF->InsertValues(0.99446044, 15.351991*keV);
	ME_EnergyCDF->InsertValues(0.99507866, 16.052589*keV);
	ME_EnergyCDF->InsertValues(0.99563387, 16.785160*keV);
	ME_EnergyCDF->InsertValues(0.99613179, 17.551162*keV);
	ME_EnergyCDF->InsertValues(0.99657768, 18.352121*keV);
	ME_EnergyCDF->InsertValues(0.99697642, 19.189633*keV);
	ME_EnergyCDF->InsertValues(0.99733254, 20.065365*keV);
	ME_EnergyCDF->InsertValues(0.99765017, 20.981061*keV);
	ME_EnergyCDF->InsertValues(0.99793311, 21.938546*keV);
	ME_EnergyCDF->InsertValues(0.99818482, 22.939727*keV);
	ME_EnergyCDF->InsertValues(0.99840848, 23.986596*keV);
	ME_EnergyCDF->InsertValues(0.99860694, 25.081241*keV);
	ME_EnergyCDF->InsertValues(0.99878285, 26.225840*keV);
	ME_EnergyCDF->InsertValues(0.99893860, 27.422674*keV);
	ME_EnergyCDF->InsertValues(0.99907631, 28.674126*keV);
	ME_EnergyCDF->InsertValues(0.99919796, 29.982690*keV);
	ME_EnergyCDF->InsertValues(0.99930529, 31.350970*keV);
	ME_EnergyCDF->InsertValues(0.99939988, 32.781693*keV);
	ME_EnergyCDF->InsertValues(0.99948315, 34.277707*keV);
	ME_EnergyCDF->InsertValues(0.99955637, 35.841994*keV);
	ME_EnergyCDF->InsertValues(0.99962070, 37.477667*keV);
	ME_EnergyCDF->InsertValues(0.99967715, 39.187986*keV);
	ME_EnergyCDF->InsertValues(0.99972664, 40.976356*keV);
	ME_EnergyCDF->InsertValues(0.99976997, 42.846340*keV);
	ME_EnergyCDF->InsertValues(0.99980788, 44.801661*keV);
	ME_EnergyCDF->InsertValues(0.99984102, 46.846215*keV);
	ME_EnergyCDF->InsertValues(0.99986994, 48.984074*keV);
	ME_EnergyCDF->InsertValues(0.99989516, 51.219495*keV);
	ME_EnergyCDF->InsertValues(0.99991714, 53.556932*keV);
	ME_EnergyCDF->InsertValues(0.99993628, 56.001038*keV);
	ME_EnergyCDF->InsertValues(0.99995291, 58.556683*keV);
	ME_EnergyCDF->InsertValues(0.99996737, 61.228957*keV);
	ME_EnergyCDF->InsertValues(0.99997991, 64.023182*keV);
	ME_EnergyCDF->InsertValues(0.99999079, 66.944923*keV);
	ME_EnergyCDF->InsertValues(1.00000000, 70.000000*keV);


}

