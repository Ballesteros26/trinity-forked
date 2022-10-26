////////////////////////////////////////////////////////////
//
//    Created:   October 2018
//    Copyright: CCP 2018
//

#include "StdAfx.h"
#include "Tr2ActionPlaySound.h"
#include "Controllers/Tr2Controller.h"
#include "ITr2SoundEmitterOwner.h"
#include "Audio/ITr2AudEmitter.h"
#include "Eve/EveMultiEffect.h"
#include "Eve/EveMultiEffectParameter.h"
#include "Eve/SpaceObject/Children/IEveEffectChildrenOwner.h"


Tr2ActionPlaySound::Tr2ActionPlaySound( IRoot* ) :
	m_bypassPrefix( false ),
	m_target( "" )
{
}

void Tr2ActionPlaySound::Start( Tr2Controller& controller )
{
	IEveEffectChildrenOwnerPtr owner = BlueCastPtr( controller.GetOwner() );

	if( owner && !m_target.empty() )
	{
		owner = BlueCastPtr( owner->GetEffectChildByName( m_target.c_str() ) );
	}

	if( !owner )
	{
		if( !m_target.empty() )
		{
			if( EveMultiEffectPtr multiEffect = BlueCastPtr( controller.GetOwner() ) )
			{
                if( EveMultiEffectParameterPtr mep = multiEffect->GetParameterByName( m_target ) )
                {
					owner = BlueCastPtr( mep->GetParameterObject() );
                }
			} 
		}
		if( !owner )
		{
			return;
		}
	}

	if( auto emitterOwner = dynamic_cast<ITr2SoundEmitterOwner*>( owner.p ) )
	{
		if( auto emitter = emitterOwner->FindSoundEmitter( m_emitterName.c_str() ) )
		{
			emitter->SendEvent( static_cast<const wchar_t*>( CA2W( m_soundEvent.c_str() ) ), m_bypassPrefix );
		}
	}
	else
	{
		CCP_LOGERR( "Tr2ActionPlaySound failed to find an owner! The target must be dervied from ITr2SoundEmitterOwner." );
	}
}

void Tr2ActionPlaySound::StartWithController( PyObject* obj )
{
	Tr2Controller* controller = BluePythonCast<Tr2Controller*>( obj );
	if( !controller )
	{
		PyErr_SetString( PyExc_TypeError, "StartWithController expects a Tr2Controller as a parameter." );
		return;
	}
	Start( *controller );
}
