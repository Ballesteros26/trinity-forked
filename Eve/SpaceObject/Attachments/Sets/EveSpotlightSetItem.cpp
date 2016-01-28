////////////////////////////////////////////////////////////
//
//    Created:   July 2012
//    Copyright: CCP 2012
//


#include "StdAfx.h"
#include "EveSpotlightSetItem.h"

// --------------------------------------------------------------------------------
// Description:
//   Initialize data members, set everything to invalid/empty
// --------------------------------------------------------------------------------
EveSpotlightSetItem::EveSpotlightSetItem( IRoot* lockobj ) :
	m_spriteScale( 1.0f, 1.0f, 1.0f ),
	m_coneColor  ( 1.0f, 1.0f, 1.0f, 1.0f ),
	m_spriteColor( 1.0f, 1.0f, 1.0f, 1.0f ),
	m_flareColor ( 1.0f, 1.0f, 1.0f, 1.0f ),
	m_boneIndex( 0 ),
	m_boosterGainInfluence( false )
{
	D3DXMatrixIdentity( &m_transform );
}


// --------------------------------------------------------------------------------
// Description:
//   Cleanup
// --------------------------------------------------------------------------------
EveSpotlightSetItem::~EveSpotlightSetItem()
{
}

