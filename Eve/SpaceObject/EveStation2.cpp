#include "StdAfx.h"

#include "EveStation2.h"

#include "Tr2Mesh.h"
#include "Eve/SpaceObject/Attachments/Sets/EveSpriteSet.h"

// --------------------------------------------------------------------------------
// Description:
//   Initialize data members
// --------------------------------------------------------------------------------
EveStation2::EveStation2( IRoot* lockobj ) : 
	EveSpaceObject2( lockobj )
{
}

// --------------------------------------------------------------------------------
// Description:
//   Destr
// --------------------------------------------------------------------------------
EveStation2::~EveStation2()
{
}

// --------------------------------------------------------------------------------
// Description:
//   Override base ::UpdateSyncronous() function, so we can update the turrets and 
//   their positions (if they are attached to animated bones!)
// --------------------------------------------------------------------------------
void EveStation2::GetBatches( ITriRenderBatchAccumulator* batches, TriBatchType batchType, const Tr2PerObjectData* perObjectData )
{
	EveSpaceObject2::GetBatches( batches, batchType, perObjectData );
}

// --------------------------------------------------------------------------------
// Description:
//   Gets called by the state machine of this object to execute some command.
// Return Value:
//   Returns true if this implementation has handled the command.
// --------------------------------------------------------------------------------
bool EveStation2::ExecuteAnimationStateCommand( const EveAnimationCommand& cmd, const std::map<std::string, float>& parameters )
{
	return EveSpaceObject2::ExecuteAnimationStateCommand( cmd, parameters );
}





