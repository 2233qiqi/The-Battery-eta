#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class EventAction : public G4UserEventAction
{
public:
    EventAction();
    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);

    void SetEnteredSiC();          
    G4bool HasEnteredSiC() const; 
    
    void AddEdep(G4double edep){ fTotalEdep += edep; }

private:
    G4bool fEnteredSiC;
    G4double fTotalEdep;
};

#endif