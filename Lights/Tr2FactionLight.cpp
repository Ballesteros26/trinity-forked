#include "StdAfx.h"
#include "Tr2FactionLight.h"


Tr2FactionLight::Tr2FactionLight( IRoot* lockobj ):
	Tr2Light( lockobj ),
    m_selectedColor( -1 ),
	m_isSpotlight( false ),
	m_parentColorSet( nullptr ),
	m_saturation( 1 )
{
	m_type = POINT_LIGHT; // POINT_LIGHT or SPOT_LIGHT
}

bool Tr2FactionLight::OnModified( Be::Var* value )
{
	if( IsMatch( value, m_isSpotlight ) )
	{
		m_isSpotlight? m_type = SPOT_LIGHT: m_type = POINT_LIGHT;
	}

	if( IsMatch( value, m_selectedColor ) )
	{
		SetLightColorFromFactionColor();
	}

	if( IsMatch( value, m_saturation ) )
	{
		SetLightColorFromFactionColor();
	}

	return Tr2Light::OnModified( value );
}

void Tr2FactionLight::SetLightColorFromFactionColor()
{
	if( nullptr == m_parentColorSet )
	{
		return;
	}

	if( m_selectedColor >= 0 && m_selectedColor < SOFDataFactionColorChooser::TYPE_MAX )
	{
		Color out = m_parentColorSet[m_selectedColor];
		// color intensity
		float i = ( out.r * 0.299f ) + ( out.g * 0.587f ) + ( out.b * 0.114f );

		out = Lerp( Color(i,i,i,i), out, max( 0.0f, m_saturation ) );

		m_lightData.color = out;
	}
}

void Tr2FactionLight::SetInheritProperties( const Color* colorSet )
{
    if ( colorSet )
    {
		m_parentColorSet = colorSet;
		SetLightColorFromFactionColor();
    }
}

Color Tr2FactionLight::GetSelectedColor() const
{
        return m_lightData.color;
}


void Tr2FactionLight::RenderDebugInfo( ITr2DebugRenderer2& renderer, const Matrix& worldMatrix, const granny_matrix_3x4* bones, size_t boneCount )
{
	if( m_isSpotlight )
	{
		auto baseColor = m_lightData.color * m_lightData.brightness;
		baseColor.a = 0.1;
		auto selectedColor = baseColor + Color( 0.0, 0.0, 0.0, 0.1 );

		Matrix boneMatrix = m_boneTransform;
		if( m_lightData.boneIndex >= 0 && m_lightData.boneIndex < boneCount ) {
			TriMatrixCopyFrom3x4( &boneMatrix, &bones[m_lightData.boneIndex] );
		}

		Matrix lightOffsetMatrix = boneMatrix * worldMatrix;
		Matrix lightMatrixWithRotation = RotationMatrix( Normalize( m_lightData.rotation ) ) * lightOffsetMatrix;
		lightMatrixWithRotation.GetTranslation() = Vector3( 0, 0, 0 );

		float scaling = XMVectorGetX( XMVectorAdd( XMVector3LengthEst( lightOffsetMatrix.GetX() ),
												   XMVectorAdd( XMVector3LengthEst( lightOffsetMatrix.GetY() ), XMVector3LengthEst( lightOffsetMatrix.GetZ() ) ) ) ) / 3.f;

		Vector3 start = TransformCoord( m_lightData.position, lightOffsetMatrix );
		Vector3 outerEnd = start + TransformCoord( Vector3( 0.0, 0.0, m_lightData.radius * scaling ), lightMatrixWithRotation );
		Vector3 innerEnd = start + TransformCoord( Vector3( 0.0, 0.0, m_lightData.innerRadius * scaling ), lightMatrixWithRotation );

		float outerConeRadius = tan( TRI_2PI * m_lightData.outerAngle / 360.f ) * m_lightData.radius * scaling;
		float innerConeRadius = tan( TRI_2PI * m_lightData.innerAngle / 360.f ) * m_lightData.innerRadius * scaling;
		renderer.DrawCone( this, outerEnd, start, outerConeRadius, 10, Tr2DebugRenderer::Solid, Tr2DebugColor( selectedColor, baseColor ) );
		renderer.DrawCone( this, innerEnd, start, innerConeRadius, 10, Tr2DebugRenderer::Solid, Tr2DebugColor( selectedColor, baseColor ) );
	}
	else
	{
		auto baseColor = m_lightData.color * m_lightData.brightness;
		baseColor.a = 0.1;
		auto selectedColor = baseColor + Color( 0.0, 0.0, 0.0, 0.2 );

		Matrix lightMatrix = m_boneTransform;
		if( m_lightData.boneIndex >= 0 && m_lightData.boneIndex < boneCount ) {
			TriMatrixCopyFrom3x4( &lightMatrix, &bones[m_lightData.boneIndex] );
		}
		lightMatrix *= worldMatrix;

		renderer.DrawSphere( this, lightMatrix, m_lightData.position, m_lightData.radius, 10, Tr2DebugRenderer::Solid, Tr2DebugColor( selectedColor, baseColor ) );
		renderer.DrawSphere( this, lightMatrix, m_lightData.position, m_lightData.innerRadius, 10, Tr2DebugRenderer::Solid, Tr2DebugColor( selectedColor, baseColor) );
	}
}

