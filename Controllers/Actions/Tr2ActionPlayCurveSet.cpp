////////////////////////////////////////////////////////////
//
//    Created:   March 2018
//    Copyright: CCP 2018
//

#include "StdAfx.h"
#include "Tr2ActionPlayCurveSet.h"
#include "Controllers/Tr2Controller.h"

#include "Eve/SpaceObject/EveSpaceObject2.h"
#include "Eve/SpaceObject/Children/IEveSpaceObjectChild.h"
#include "Eve/EveEffectRoot2.h"


void Tr2ActionPlayCurveSet::Start( Tr2Controller& controller )
{
	auto owner = controller.GetOwner();
	if( auto spaceObject = dynamic_cast<EveSpaceObject2*>( owner ) )
	{
		spaceObject->PlayCurveSet( m_curveSetName );
	}
	else if( auto effectRoot = dynamic_cast<EveSpaceObject2*>( owner ) )
	{
		effectRoot->PlayCurveSet( m_curveSetName );
	}
	else if( auto child = dynamic_cast<IEveSpaceObjectChild*>( owner ) )
	{
		child->PlayCurveSet( m_curveSetName );
	}
}

void Tr2ActionPlayCurveSet::Stop( Tr2Controller& controller )
{
	auto owner = controller.GetOwner();
	if( auto spaceObject = dynamic_cast<EveSpaceObject2*>( owner ) )
	{
		spaceObject->StopCurveSet( m_curveSetName );
	}
	else if( auto effectRoot = dynamic_cast<EveSpaceObject2*>( owner ) )
	{
		effectRoot->StopCurveSet( m_curveSetName );
	}
	else if( auto child = dynamic_cast<IEveSpaceObjectChild*>( owner ) )
	{
		child->StopCurveSet( m_curveSetName );
	}
}
