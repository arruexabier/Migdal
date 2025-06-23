#include "RunAction.hh"
#include "G4Run.hh"
#include <H5Cpp.h>
#include <vector>
#include "EventAction.hh"
#include "G4RunManager.hh"

namespace B1
{

RunAction::RunAction()
    : file(nullptr), dataset(nullptr), dataspace(nullptr), currentSize(0), maxSize(1000000) {}

RunAction::~RunAction() {
    delete dataset;
    delete dataspace;
    delete file;
}

void RunAction::BeginOfRunAction(const G4Run*) {
    file = new H5::H5File("simulation_output.h5", H5F_ACC_TRUNC);
    
    // Define dataset structure: (EventID, Position(x,y,z), EnergyDeposit)
    hsize_t dims[2] = {0, 5};  // Start with 0 entries, max 5 columns
    hsize_t maxDims[2] = {H5S_UNLIMITED, 5};
    dataspace = new H5::DataSpace(2, dims, maxDims);

    // Create chunked dataset with extendable storage
    H5::DSetCreatPropList prop;
    hsize_t chunkDims[2] = {1000, 5};  // Store in chunks of 1000 rows
    prop.setChunk(2, chunkDims);

    dataset = new H5::DataSet(file->createDataSet("Processes", H5::PredType::NATIVE_DOUBLE, *dataspace, prop));
}

void RunAction::EndOfRunAction(const G4Run*) {
    if (file) {
        file->flush(H5F_SCOPE_GLOBAL);
        delete dataset;
        delete dataspace;
        delete file;
        dataset = nullptr;
        dataspace = nullptr;
        file = nullptr;
    }
    
    
    //AVERGAE MIGDAL PROB
    
    const EventAction* eventAction =
        static_cast<const EventAction*>(G4RunManager::GetRunManager()->GetUserEventAction());

    G4double totalProb = eventAction->GetTotalProb();
    G4int totalEvents = eventAction->GetEventCount();

    G4double avg = (totalEvents > 0) ? totalProb / totalEvents : 0.;

    G4cout << "=== Average Migdal Event Probability ===" << G4endl;
    G4cout << " Total Events: " << totalEvents << G4endl;
    G4cout << " Total Probability: " << totalProb << G4endl;
    G4cout << " Average Probability: " << avg << G4endl;
    
    
}

void RunAction::WriteStepData(G4int eventID, G4ThreeVector position, G4double energy) {
    if (!dataset || energy == 0) return;

    hsize_t newSize[2] = {currentSize + 1, 5};  // Extend rows, keep 5 columns fixed
    dataset->extend(newSize);

    // Select new row in dataset
    H5::DataSpace fileSpace = dataset->getSpace();
    hsize_t start[2] = {currentSize, 0};  // Start at new row
    hsize_t count[2] = {1, 5};  // 1 row, 5 columns
    fileSpace.selectHyperslab(H5S_SELECT_SET, count, start);

    // Create memory space for 1 row of data
    hsize_t memDims[2] = {1, 5};
    H5::DataSpace memSpace(2, memDims);

    // Prepare data to write
    double rowData[5] = {static_cast<double>(eventID), position.x(), position.y(), position.z(), energy};

    // Write to dataset
    dataset->write(rowData, H5::PredType::NATIVE_DOUBLE, memSpace, fileSpace);

    currentSize++;
}

}
