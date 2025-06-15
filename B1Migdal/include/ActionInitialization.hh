#ifndef B1ActionInitialization_h
#define B1ActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
#include "RunAction.hh"
#include "EventAction.hh"

namespace B1
{

class ActionInitialization : public G4VUserActionInitialization
{
  public:
    // Constructor now accepts a pointer to an existing RunAction instance
    explicit ActionInitialization(RunAction* runAction);
    ~ActionInitialization() override = default;

    void BuildForMaster() const override;
    void Build() const override;

  private:
    RunAction* fRunAction;  // Pointer to the shared RunAction instance
};

} // namespace B1

#endif

