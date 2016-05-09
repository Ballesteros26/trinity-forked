#include "StdAfx.h"
#include "Tr2ScalarBezierCurve.h"
#include <float.h>

// --------------------------------------------------------------------------------------
// Description:
//   Horner evaluation of a polynomial using the following representation:
//   t *( t *( t*a + b ) + c ) + d
// Arguments:
//   poly - Array of polynomial coefficients
//   deg - Degree of polynomial
//   s - The polynomial independent variable
// Return Value:
//   The polynomial evaluated at point s
// --------------------------------------------------------------------------------------
float evalpoly( float poly[], int deg, float s )
{
	float a = poly[deg];
	while( --deg >= 0 )
	{
		a = (s * a) + poly[deg];
	}
	return a;
}

// --------------------------------------------------------------------------------------
// Description:
//   Implementation of Brent's method for finding a root of a polynomial.
//   <extlink http://en.wikipedia.org/wiki/Brent's_method>Brent's Method</extlink>
// Arguments:
//   a - TODO
//   b - TODO
//   poly - Polynomial coefficients
//   deg - Polynomial degree
// Return Value:
//   Root of the polynomial
// --------------------------------------------------------------------------------------
float brents( float a, float b, float poly[], int deg )
{
	float fa, fb, fc, c, d;

	fa = evalpoly( poly, deg, a );
	if( fabs( fa ) < FLT_EPSILON ) 
	{
		return a;
	}

	fb = evalpoly( poly, deg, b );
	if( fabs( fb ) < FLT_EPSILON )
	{
		return b;
	}

	bool m = true;
	float n = -1.0f;
	if( fabs( fa ) > fabs( fb ) )
	{
		fc = fa;
		c = a;
		fa = fb;
		a = b;
		fb = fc;
		b = c;
	}
	c = a;
	d = 0.0f;

	while( ( n < 0.0f || n >= 1.0f ) && ( fb != 0.0f && fabs( b - a ) > FLT_EPSILON ) )
	{
		fc = evalpoly( poly, deg, c );
		float s = 0.0f;

		if( !( fa == fc || fb == fc ) )
		{
			// Inverse quadratic interpolation
			s = (a*fb*fc/(fa-fb)/(fa-fc))+(b*fa*fc/(fb-fa)/(fb-fc))+(c*fa*fb/(fc-fa)/(fc-fb));
		}
		else
		{
			// Secant rule
			s = b - ( fb * ( b - a ) ) / ( fb - fa );
		}
		float e = ( 3.0f * a + b ) / 4.0f;
		if( ( ( ( s >= b ) && ( s <= e ) ) || ( ( s >= e ) && ( s <= b ) ) ) 
			|| ( m && ( fabs( s - b ) >= fabs( b - c ) * 0.5f ) ) 
			|| ( !m && ( fabs( s - b ) >= fabs( c - d ) * 0.5f ) ) )
		{
			// Bisection method
			s = ( a + b ) * 0.5f;
			m = true;
		}
		else
		{
			m = false;
		}

		float fs = evalpoly(poly, deg, s);
		d = c; 
		c = b;
		if( fa * fs < 0.0f )
		{
			b = s;
		}
		else
		{
			a = s;
		}
		if( fabs( fa ) < fabs( fb ) )
		{
			float temp = a;
			a = b;
			b = temp;
		}
		n -= 1.0f;
		fa = evalpoly( poly, deg, a );
		fb = evalpoly( poly, deg, b );

	}
	return b;
}

// --------------------------------------------------------------------------------------
// Description:
//   Finds all the roots of a polynomial.  Apparently this code is borrowed from the Maya
//   animEngine example.
// Arguments:
//   poly - Polynomial coefficients
//   deg - Polynomial degree
//   a - TODO
//   a_closed - TODO
//   b - TODO
//   b_closed - TODO
//   roots - [output] Roots of the polynomial
// Return Value:
//   Number of roots of the polynomial
// --------------------------------------------------------------------------------------
int polyZeroes( float poly[], int deg, float a, bool a_closed, float b, bool b_closed, float roots[] )
{/*
	This numerical function is borrowed from the maya animEngine example
 */
	int i;
	int nr = 0;
	float nf, e, f, pe, tol, *p, p_x[22], *d, d_x[22], *dr, dr_x[22];

	e = pe = 0.0;
	f = 0.0;
	
	// The sum of the coefficients
	for (i = 0 ; i < deg + 1; ++i) {
		f += fabs(poly[i]);
	}

	// tolerance linked to the sum of the start and endpoint and the number of coefficients
	tol = (fabs(a) + fabs(b))*(deg+1)*FLT_EPSILON;

	/* Zero polynomial to tolerance? */
	if (f <= tol)  return(-1);

	p = p_x;  // normalizing the polynomial
	d = d_x;  
	dr = dr_x;
	nf = 1.0f/f;
	for (i = 0 ; i < deg + 1; ++i) 
	{
		p[i] = nf * poly[i];
	}

	// if a coefficient is smaller than the tolerance than it will be dropped
	while ( fabs( p[deg] ) < tol )
	{
		deg--;
	}
	/* Identically zero poly already caught so constant fn != 0 */
	if (deg == 0) 
	{
		return 0;
	}

	if (deg == 1) 
	{
		int left_ok, right_ok;
		roots[0] = -p[0] / p[1];

		if ( a_closed )
		{
			left_ok  = a < ( roots[0] + tol );
		}
		else
		{
			left_ok  = a < ( roots[0] - tol );
		}

		if ( b_closed )
		{
			right_ok  = b > ( roots[0] - tol );
		}
		else
		{
			right_ok  = b > ( roots[0] + tol );
		}

		if (left_ok && right_ok )
		{
			if ( a_closed && roots[0] < a ) 
			{
				roots[0] = a;
			}
			else if ( b_closed && roots[0] > b ) 
			{
				roots[0] = b;
			}
			return 1;
		}
		else
		{
			return 0;
		}		
	}
	else 
	{

		/* compute derivative */
		for ( i = 1; i <= deg; i++ ) 
		{
			d[i-1] = i*p[i];
		}

		/* find roots of derivative */
		int ndr = polyZeroes ( d, deg-1, a, false, b, false, dr );
		if (ndr == -1) 
		{
			return 0;
		}

		bool skip;
		/* find roots between roots of the derivative */
		for (i = skip = 0 ; i <= ndr; i++) 
		{
			float s, ps;
			if ( i == 0 ) 
			{
				// When we start
				s = a; 
				// evaluate the polynomial
				ps = evalpoly( p, deg, s );
				// if the polynomial evaluates to something smaller than tol and we
				// include a on the interval then a must be a zero of the polynomial
				if ( fabs(ps)<=tol && a_closed) 
				{
					roots[nr++] = a;
				}
			}
			else 
			{ 
				// now we move forward... e is the next value the check
				s = e; 
				ps = pe; 
			}
			// If we are at the end of our derivative roots
			if ( i == ndr ) 
			{ 
				// the end of our interval..
				e = b; 
				skip = false;
			}
			else 
			{
				// now check the roots of the derivatives
				e = dr[i];
			}
			// evaluate the polynomial
			pe = evalpoly( p, deg, e );

			if ( skip )
			{
				skip = false;
			}
			else 
			{
				// if our end value evaluates to zero
				if ( fabs( pe ) < tol ) 
				{
					// Then we have found a root and want to skip ahead one interval
					if ( i != ndr || b_closed ) 
					{
						roots[nr++] = e;
						skip = true;
					}
				}
				// f(a)*f(b) < 0 means that we cross the x axis somewhere and there is a root
				// to be zeroed in on.
				else if ( ps*pe < 0.0f ) 
				{
					// Use Brents method to find our root
					roots[nr++] = brents(s, e, p, deg );
					// if we have found at least 2 roots and the new root is less than our 
					// previous one. Then we use the bisection method to place a new root between the 
					// two previous ones
					if (( nr > 1 ) && roots[nr - 2] >= roots[nr - 1] - tol )
					{
						roots[nr - 2] = (roots[nr - 2] + roots[nr - 1]) * 0.5f;
						nr--;
					}
				}
			}
			if ( nr > deg )
			{
				return nr;
			}
		}
	}

	return nr;
}

// --------------------------------------------------------------------------------------
// Description:
//   Scalar bezier key comparison function.
// Arguments:
//   context - ignored
//   a - First scalar bezier key comparison operand
//   b - Second scalar bezier key comparison operand
// Return Value:
//   true, if key a's time is less that key b's time
//   false, otherwise
// --------------------------------------------------------------------------------------
static bool CompareKeys( IRoot* context, Tr2ScalarBezierKey* a, Tr2ScalarBezierKey* b )
{
	return a->m_time < b->m_time;
}



// --------------------------------------------------------------------------------------
// Description:
//   Default constructor.
// --------------------------------------------------------------------------------------
Tr2ScalarBezierKey::Tr2ScalarBezierKey( IRoot* lockobj ) 
{
	m_value = 0.0f;
	m_time = 0.0f;
	m_leftControlPoint = Vector2( 0.0f, 0.0f );
	m_rightControlPoint = Vector2( 0.0f, 0.0f );
	m_interpolation = LINEAR;
}

// --------------------------------------------------------------------------------------
// Description:
//   Default constructor.
// --------------------------------------------------------------------------------------
Tr2ScalarBezierCurve::Tr2ScalarBezierCurve( IRoot* lockobj ) :
	Tr2Curve<Tr2ScalarBezierKey, PTr2ScalarBezierKeyVector, float>( lockobj )
{
	m_startValue = 0.0f;
	m_endValue = 0.0f;
	m_currentValue = 0.0f;
	m_controlPointA = Vector2( 0.0f, 0.0f );
	m_controlPointB = Vector2( 0.0f, 0.0f );
}

	// --------------------------------------------------------------------------------------
	// Description:
	//   Private virtual function to add a scalar bezier key at a particular time.  The
	//   left and right control points are set to (-1,0) and (1,0) respectively.
	// Arguments:
	//   time - The time at which to add the key
	//   value - The scalar value for the key
	// --------------------------------------------------------------------------------------
void Tr2ScalarBezierCurve::AddKey_( float time, const float& value )
{
	Tr2ScalarBezierKeyPtr key;
	if( !key.CreateInstance() )
	{
		return;
	}

	key->m_time = time;
	key->m_value = value;
	key->m_leftControlPoint = Vector2( 1.0f, 0.0f );
	key->m_rightControlPoint = Vector2( -1.0f, 0.0f );
	key->m_interpolation = (Interpolation)m_interpolation;
	m_keys.Insert( -1, key );
}

// --------------------------------------------------------------------------------------
// Description:
//  Sorts the color keys into ascending order based on key time.
// --------------------------------------------------------------------------------------
void Tr2ScalarBezierCurve::Sort()
{
	if ( m_keys.size() > 1 )
	{
		m_keys.Sort( (IList::CompareFn)CompareKeys, NULL );
		// We might have added a key passed the length of the curve
		if ( m_keys.back()->m_time > m_length )
		{
			Tr2ScalarBezierKey* back = m_keys.back();
			float preLength = m_length;
			float endValue = m_endValue;
			Vector2 endControlPoint = m_controlPointB;

			m_length = back->m_time;
			m_endValue = back->m_value;
			m_controlPointB = back->m_leftControlPoint;
			if ( preLength > 0.0f )
			{	
				back->m_time = preLength;
				back->m_value = endValue;
				back->m_leftControlPoint = endControlPoint;
			}
		}
	}
}

// --------------------------------------------------------------------------------------
// Description:
//   Gets the left control point for a given key index.  If the index is out of range,
//   the curve's m_controlPointA value is returned instead.
// Arguments:
//   idx - Key index
// Return Value:
//   Left control point for the given key index, or the curve's m_controlPointA value if
//   the index is out of range
// --------------------------------------------------------------------------------------
const Vector2& Tr2ScalarBezierCurve::GetKeyLeftControlPoint( unsigned int idx ) const
{
	if ( idx < (unsigned int)m_keys.size() )
	{
		return m_keys[idx]->m_leftControlPoint;
	}
	return m_controlPointA;
}

// --------------------------------------------------------------------------------------
// Description:
//   Sets the left control point for a given key index.  If the index is out of range,
//   nothing happens.
// Arguments:
//   idx - Key index
//   tangent - Left control point for the given key index
// --------------------------------------------------------------------------------------
void Tr2ScalarBezierCurve::SetKeyLeftControlPoint( unsigned int idx, const Vector2& tangent )
{
	if ( idx < (unsigned int)m_keys.size() )
	{
		m_keys[idx]->m_leftControlPoint = tangent;
	}
}

// --------------------------------------------------------------------------------------
// Description:
//   Gets the right control point for a given key index.  If the index is out of range,
//   the curve's m_controlPointB value is returned instead.
// Arguments:
//   idx - Key index
// Return Value:
//   Right control point for the given key index, or the curve's m_controlPointB value if
//   the index is out of range
// --------------------------------------------------------------------------------------
const Vector2& Tr2ScalarBezierCurve::GetKeyRightControlPoint( unsigned int idx ) const
{
	if ( idx < (unsigned int)m_keys.size() )
	{
		return m_keys[idx]->m_rightControlPoint;
	}
	return m_controlPointB;
}

// --------------------------------------------------------------------------------------
// Description:
//   Sets the right control point for a given key index.  If the index is out of range,
//   nothing happens.
// Arguments:
//   idx - Key index
//   tangent - right control point for the given key index
// --------------------------------------------------------------------------------------
void Tr2ScalarBezierCurve::SetKeyRightControlPoint( unsigned int idx, const Vector2& val )
{
	if ( idx < (unsigned int)m_keys.size() )
	{
		m_keys[idx]->m_rightControlPoint = val;
	}
}

// --------------------------------------------------------------------------------------
// Description:
//   Interpolates between two Bezier keys using the curve's internal time as the 
//   interpolation control factor.
// Arguments:
//   out - [output] Interpolated scalar Bezier value
//   startKey - First key from which to interpolate
//   endKey - Second key from which to interpolate
// Return Value:
//   Value of the Bezier curve evaluated at the current time
// --------------------------------------------------------------------------------------
float* Tr2ScalarBezierCurve::Interpolate( 
	float* out, 
	Tr2ScalarBezierKey* startKey, 
	Tr2ScalarBezierKey* endKey )
{
	*out = m_startValue;
	float time = m_localTime;
	float deltaTime = m_length;
	float startValue = m_startValue;
	float endValue = m_endValue;
	if( startKey )
	{
		time -=  startKey->m_time;
	}

	Vector2 p0( 0.0f, m_startValue );
	Vector2 p3( m_length, m_endValue );
	Vector2 p1 = m_controlPointA;
	Vector2 p2 = m_controlPointB;
	
	if( startKey && endKey )
	{		
		startValue = startKey->m_value;
		p0 = Vector2( startKey->m_time, startValue );
		p1 = startKey->m_rightControlPoint;
		endValue = endKey->m_value;
		p3 = Vector2( endKey->m_time, endValue );
		p2 = endKey->m_leftControlPoint;
		deltaTime = endKey->m_time - startKey->m_time;
	}
	else if( startKey == NULL && endKey != NULL )
	{
		endValue = endKey->m_value;
		p3 = Vector2( endKey->m_time, endValue );
		p2 = endKey->m_leftControlPoint;
		deltaTime = endKey->m_time;
	}
	else if( startKey != NULL && endKey == NULL )
	{
		startValue = startKey->m_value;
		p0 = Vector2( startKey->m_time, startValue );
		p1 = startKey->m_rightControlPoint;
		deltaTime = m_length - startKey->m_time;
	}
	float s = time / deltaTime;

	float t;	
	float coeff[4];
	float polys[4];
	float roots[5];
	int nroots = 0;

	// Bezier to power
	float a = ( p2.x + p3.x ) / deltaTime;
	float b = ( p1.x + p0.x ) / deltaTime;
	float c = a - b;
	float d = c - b;
	coeff[3] = 1.0f - a - c - d;
	coeff[2] = d + d + d;
	coeff[1] = b + b + b;
	coeff[0] = -s;

	a = p1.y;
	b = ( p2.y + p3.y ) - ( p1.y + p0.y );
	c = -p2.y;
	d = b - a;
	polys[3] = c - b - d;
	polys[2] = d + d + d;
	polys[1] = a + a + a;
	polys[0] = p0.y;

	nroots = polyZeroes( coeff, 3, 0.0, true, 1.0, true, roots );

	if( nroots > 0 )
	{		
		t = roots[0];
	}
	else 
	{
		t = 0.0;
	}
	
	*out = ( t * ( t * ( t * polys[3] + polys[2] ) + polys[1] ) + polys[0] );
	
	return out;
}