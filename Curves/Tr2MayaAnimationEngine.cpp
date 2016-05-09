#include "StdAfx.h"
#include "Tr2MayaAnimationEngine.h"

#if BLUE_WITH_PYTHON

#include <float.h>

// Mayas numerical functions for root finding
// They are left as is even though that can be
// rewritten to be a little bit more readable

// The root finding is the only bit of the 
// Maya animation engine that has not been re factored

// ----- Begin Autodesk Root Finding!
/*
//   Evaluate a polynomial in array form ( value only )
//   input:
//      P               array 
//      deg             degree
//      s               parameter
//   output:
//      ag_horner1      evaluated polynomial
//   process: 
//      ans = sum (i from 0 to deg) of P[i]*s^i
//   restrictions: 
//      deg >= 0           
*/
static float ag_horner1 (float P[], int deg, float s)
{
	float h = P[deg];
	while (--deg >= 0) h = (s * h) + P[deg];
	return (h);
}

typedef struct ag_polynomial {
	float *p;
	int deg;
} AG_POLYNOMIAL;

/*
//	Description
//   Compute parameter value at zero of a function between limits
//       with function values at limits
//   input:
//       a, b      real interval
//       fa, fb    double values of f at a, b
//       f         real valued function of t and pars
//       tol       tolerance
//       pars      pointer to a structure
//   output:
//       ag_zeroin2   a <= zero of function <= b
//   process:
//       We find the zeroes of the function f(t, pars).  t is
//       restricted to the interval [a, b].  pars is passed in as
//       a pointer to a structure which contains parameters
//       for the function f.
//   restrictions:
//       fa and fb are of opposite sign.
//       Note that since pars comes at the end of both the
//       call to ag_zeroin and to f, it is an optional parameter.
*/
static float ag_zeroin2 (float a, float b, float fa, float fb, float tol, AG_POLYNOMIAL *pars)
{
	int test;
	float c, d, e, fc, del, m, machtol, p, q, r, s;

	/* initialization */
	machtol = FLT_EPSILON;

	/* start iteration */
label1:
	c = a;  fc = fa;  d = b-a;  e = d;
label2:
	if (fabs(fc) < fabs(fb)) {
		a = b;   b = c;   c = a;   fa = fb;   fb = fc;   fc = fa;
	}

	/* convergence test */
	del = 2.0f * machtol * fabs(b) + 0.5f*tol;
	m = 0.5f * (c - b);
	test = ((fabs(m) > del) && (fb != 0.0f));
	if (test) {
		if ((fabs(e) < del) || (fabs(fa) <= fabs(fb))) {
			/* bisection */
			d = m;  e= d;
		}
		else {
			s = fb / fa;
			if (a == c) {
				/* linear interpolation */
				p = 2.0f*m*s;    q = 1.0f - s;
			}
			else {
				/* inverse quadratic interpolation */
				q = fa/fc;
				r = fb/fc;
				p = s*(2.0f*m*q*(q-r)-(b-a)*(r-1.0f));
				q = (q-1.0f)*(r-1.0f)*(s-1.0f);
			}
			/* adjust the sign */
			if (p > 0.0f) q = -q;  else p = -p;
			/* check if interpolation is acceptable */
			s = e;   e = d;
			if ((2.0f*p < 3.0f*m*q-fabs(del*q))&&(p < fabs(0.5f*s*q))) {
				d = p/q;
			}
			else {
				d = m;	e = d;
			}
		}
		/* complete step */
		a = b;	fa = fb;
		if ( fabs(d) > del )   b += d;
		else if (m > 0.0) b += del;  else b -= del;
		fb = ag_horner1 (pars->p, pars->deg, b);
		if (fb*(fc/fabs(fc)) > 0.0 ) {
			goto label1;
		}
		else {
			goto label2;
		}
	}
	return (b);
}

/*
//	Description:
//   Compute parameter value at zero of a function between limits
//   input:
//       a, b            real interval
//       f               real valued function of t and pars
//       tol             tolerance
//       pars            pointer to a structure
//   output:
//       ag_zeroin       zero of function
//   process:
//       Call ag_zeroin2 to find the zeroes of the function f(t, pars).
//       t is restricted to the interval [a, b].
//       pars is passed in as a pointer to a structure which contains
//       parameters for the function f.
//   restrictions:
//       f(a) and f(b) are of opposite sign.
//       Note that since pars comes at the end of both the
//         call to ag_zeroin and to f, it is an optional parameter.
//       If you already have values for fa,fb use ag_zeroin2 directly
*/
static float ag_zeroin (float a, float b, float tol, AG_POLYNOMIAL *pars)
{
	float fa, fb;

	fa = ag_horner1 (pars->p, pars->deg, a);
	if (fabs(fa) < FLT_EPSILON) return(a);

	fb = ag_horner1 (pars->p, pars->deg, b);
	if (fabs(fb) < FLT_EPSILON) return(b);

	return (ag_zeroin2 (a, b, fa, fb, tol, pars));
} 

/*
// Description:
//   Find the zeros of a polynomial function on an interval
//   input:
//       Poly                 array of coefficients of polynomial
//       deg                  degree of polynomial
//       a, b                 interval of definition a < b
//       a_closed             include a in interval (TRUE or FALSE)
//       b_closed             include b in interval (TRUE or FALSE)
//   output: 
//       polyzero             number of roots 
//                            -1 indicates Poly == 0.0
//       Roots                zeroes of the polynomial on the interval
//   process:
//       Find all zeroes of the function on the open interval by 
//       recursively finding all of the zeroes of the derivative
//       to isolate the zeroes of the function.  Return all of the 
//       zeroes found adding the end points if the corresponding side
//       of the interval is closed and the value of the function 
//       is indeed 0 there.
//   restrictions:
//       The polynomial p is simply an array of deg+1 doubles.
//       p[0] is the constant term and p[deg] is the coef 
//       of t^deg.
//       The array roots should be dimensioned to deg+2. If the number
//       of roots returned is greater than deg, suspect numerical
//       instabilities caused by some nearly flat portion of Poly.
*/
static int polyZeroes(float Poly[], int deg, float a, int a_closed, float b, int b_closed, float Roots[])
{
	int i, nr;
	float e, f, pe, tol, *p, p_x[22], *d, d_x[22], *dr, dr_x[22];
	AG_POLYNOMIAL ply;

	e = pe = 0.0;  
	f = 0.0;

	for (i = 0 ; i < deg + 1; ++i) {
		f += fabs(Poly[i]);
	}
	tol = (fabs(a) + fabs(b))*(deg+1)*FLT_EPSILON;

	/* Zero polynomial to tolerance? */
	if (f <= tol)  return(-1);

	p = p_x;  d = d_x;  dr = dr_x;
	for (i = 0 ; i < deg + 1; ++i) {
		p[i] = 1.0f/f * Poly[i];
	}

	/* determine true degree */
	while ( fabs(p[deg]) < tol) deg--;

	/* Identically zero poly already caught so constant fn != 0 */
	nr = 0;
	if (deg == 0) return (nr);

	/* check for linear case */
	if (deg == 1) {
		Roots[0] = -p[0] / p[1];
		int left_ok  = (a_closed) ? (a<Roots[0]+tol) : (a<Roots[0]-tol);
		int right_ok = (b_closed) ? (b>Roots[0]-tol) : (b>Roots[0]+tol);
		nr = (left_ok && right_ok) ? 1 : 0;
		if (nr) {
			if (a_closed && Roots[0]<a) Roots[0] = a;
			else if (b_closed && Roots[0]>b) Roots[0] = b;
		}
		return (nr);
	}
	/* handle non-linear case */
	else {
		ply.p = p;  ply.deg = deg;

		/* compute derivative */
		for (i=1; i<=deg; i++) d[i-1] = i*p[i];

		int ndr, skip;

		/* find roots of derivative */
		ndr = polyZeroes ( d, deg-1, a, 0, b, 0, dr );
		if (ndr == -1) return (0);

		/* find roots between roots of the derivative */
		for (i=skip=0; i<=ndr; i++) {
			float s, ps;
			if (nr>deg) return (nr);
			if (i==0) {
				s=a; ps = ag_horner1( p, deg, s);
				if ( fabs(ps)<=tol && a_closed) Roots[nr++]=a;
			}
			else { s=e; ps=pe; }
			if (i==ndr) { e = b; skip = 0;}
			else e=dr[i];
			pe = ag_horner1( p, deg, e );
			if (skip) skip = 0;
			else {
				if ( fabs(pe) < tol ) {
					if (i!=ndr || b_closed) {
						Roots[nr++] = e;
						skip = 1;
					}
				}
				else if ((ps<0 && pe>0)||(ps>0 && pe<0)) {
					Roots[nr++] = ag_zeroin(s, e, 0.0, &ply );
					if ((nr>1) && Roots[nr-2]>=Roots[nr-1]-tol) { 
						Roots[nr-2] = (Roots[nr-2]+Roots[nr-1]) * 0.5f;
						nr--;
					}
				}
			}
		}
	}

	return (nr);
} 
// ----- End Autodesk Root Finding!

static BlueStructureDefinition HermiteSegmentStructureDef[] =
{ 
	{ "time",		Be::FLOAT32_1,	0 }, 
	{ "value",		Be::FLOAT32_1,	4 }, 
	{ "coeff",		Be::FLOAT32_4,	8 }, 
	{ "isStep",		Be::BYTE_1,		24 }, 
	{ "isStepNext",	Be::BYTE_1,		25 }, 
	{0} 
};

static BlueStructureDefinition BezierSegmentStructureDef[] =
{ 
	{ "time",		Be::FLOAT32_1,	0 }, 
	{ "value",		Be::FLOAT32_1,	4 }, 
	{ "coeff",		Be::FLOAT32_4,	8 }, 
	{ "poly",		Be::FLOAT32_4,	24 },
	{ "isStep",		Be::BYTE_1,		40 }, 
	{ "isStepNext",	Be::BYTE_1,		41 }, 	
	{ "isLinear",	Be::BYTE_1,		42 },
	{0} 
};

static BlueStructureDefinition CurveStructureDef[] =
{ 
	// static data
	{ "numSegments",	Be::INT32_1,	0 }, 
	{ "segmentOffset",	Be::INT32_1,	4 }, 
	{ "endTime",		Be::FLOAT32_1,	8 }, 
	{ "endValue",		Be::FLOAT32_1,	12 }, 
	{ "inTangent",		Be::FLOAT32_1,	16 }, 
	{ "outTangent",		Be::FLOAT32_1,	20 }, 
	{ "preInfinity",	Be::BYTE_1,		24 },
	{ "postInfinity",	Be::BYTE_1,		25 },
	{ "isWeighted",		Be::BYTE_1,		26 },
	{0} 
};

// info objects
Tr2MayaAnimationCurveInfo::Tr2MayaAnimationCurveInfo( IRoot* lockobj ):
	m_index( -1 )
{
}

Tr2MayaAnimationEngineInfo::Tr2MayaAnimationEngineInfo( IRoot* lockobj ):
	PARENTLOCK( m_curves ),
	m_fps(30.0f)
{
}

Tr2MayaAnimationEngine::Tr2MayaAnimationEngine( IRoot* lockobj ):
	PARENTLOCK( m_curves ),
	PARENTLOCK( m_bezierSegments ),
	PARENTLOCK( m_hermiteSegments ),
	m_evalCache( NULL )
{
	m_curves.SetStructureDefinition( CurveStructureDef );
	m_hermiteSegments.SetStructureDefinition( HermiteSegmentStructureDef );
	m_bezierSegments.SetStructureDefinition( BezierSegmentStructureDef );
}

Tr2MayaAnimationEngine::~Tr2MayaAnimationEngine( )
{
	CCP_FREE( m_evalCache );
}	

// Refactored and optimized version of the maya evaluation logic and setup.
float Tr2MayaAnimationEngine::evaluateBezier(const Tr2MayaAnimBezierSegment *segment, float time, float nextSegmentTime) const
{
	float t, s;

	s = (time - segment->m_time) / (nextSegmentTime - segment->m_time);

	if( segment->m_isLinear ) 
	{
		t = s;
	}
	else 
	{
		float poly[4], roots[5];
		poly[3] = segment->m_fCoeff[3];
		poly[2] = segment->m_fCoeff[2];
		poly[1] = segment->m_fCoeff[1];
		poly[0] = segment->m_fCoeff[0] - s;

		int numRoots = polyZeroes (poly, 3, 0.0, 1, 1.0, 1, roots);
		if (numRoots == 1) 
		{
			t = roots[0];
		}
		else 
		{
			t = 0.0;
		}
	}
	return (t * (t * (t * segment->m_fPolyY[3] + segment->m_fPolyY[2]) + segment->m_fPolyY[1]) + segment->m_fPolyY[0]);
}

/*
// Description:
//		Given the time between fX1 and fX2, return the function
//		value of the curve
*/
float Tr2MayaAnimationEngine::evaluateHermite(const Tr2MayaAnimHermiteSegment *segment, float time) const
{
	float t;
	t = time - segment->m_time;
	return (t * (t * (t * segment->m_fCoeff[0] + segment->m_fCoeff[1]) + segment->m_fCoeff[2]) + segment->m_fCoeff[3]);
}

/*
//	Function Name:
//		evaluateInfinities
//
//	Description:
//		A static helper function to evaluate the infinity portion of an
//	animation curve.  The infinity portion is the parts of the animation
//	curve outside the range of keys.
//
//  Input Arguments:
//		EtCurve *animCurve			The animation curve to evaluate
//		EtTime time					The time (in seconds) to evaluate
//		EtBoolean evalPre
//			true				evaluate the pre-infinity portion
//			false			evaluate the post-infinity portion
//
//  Return Value:
//		EtValue value				The evaluated value of the curve at time
*/
float Tr2MayaAnimationEngine::evaluateInfinities(const Tr2MayaAnimCurve *animCurve, void* firstSegment, size_t segmentSize, float time, bool evalPre )
{
	if (evalPre) 
	{
		switch(animCurve->m_preInfinity)
		{
		case INFINITY_OSCILLATE:
				return evaluateInfinityOscillatePre(animCurve, firstSegment, segmentSize, time);				
		case INFINITY_CYCLE:
				return evaluateInfinityCyclePre(animCurve, firstSegment, segmentSize, time);				
		case INFINITY_CYCLE_RELATIVE:
				return evaluateInfinityCycleRelativePre(animCurve, firstSegment, segmentSize, time);				
		case INFINITY_LINEAR:
				return evaluateInfinityLinearPre(animCurve, firstSegment, segmentSize, time);				
		default:
			return 0.0f;
		};
	}
	else 
	{
		switch(animCurve->m_postInfinity)
		{
		case INFINITY_OSCILLATE:
				return evaluateInfinityOscillatePost(animCurve, firstSegment, segmentSize, time);
		case INFINITY_CYCLE:
				return evaluateInfinityCyclePost(animCurve, firstSegment, segmentSize, time);
		case INFINITY_CYCLE_RELATIVE:
				return evaluateInfinityCycleRelativePost(animCurve, firstSegment, segmentSize, time);
		case INFINITY_LINEAR:
				return evaluateInfinityLinearPost(animCurve, time);
		default:
			return 0.0f;
		};
	}
}

float Tr2MayaAnimationEngine::evaluateInfinityLinearPre( const Tr2MayaAnimCurve *animCurve, void* firstSegment, size_t segmentSize, float time )
{
	float firstTime = static_cast<Tr2MayaAnimSegment*>( firstSegment )->m_time;
	float value = static_cast<Tr2MayaAnimSegment*>( firstSegment )->m_value;	

	value -= ((firstTime - time) * animCurve->m_inTangent);					

	return value;
}

float Tr2MayaAnimationEngine::evaluateInfinityLinearPost( const Tr2MayaAnimCurve *animCurve, float time )
{
	float value = animCurve->m_endValue;
	value += ((time - animCurve->m_endTime) * animCurve->m_outTangent);					
	return value;
}

void Tr2MayaAnimationEngine::evaluateInfinityCycleSetup( const Tr2MayaAnimCurve *animCurve, void* firstSegment, size_t segmentSize, float time, 
	float* firstTime, float* lastTime, float *factoredTime, 
	float *remainder ) const
{
	float timeRange;
	double numCycles;
	*firstTime = static_cast<Tr2MayaAnimSegment*>( firstSegment )->m_time;
	*lastTime = animCurve->m_endTime;
	timeRange = *lastTime - *firstTime;

	if (time > *lastTime) 
	{
		*remainder = (float)fabs (modf ((time - *lastTime) / timeRange, &numCycles));
	}
	else 
	{
		*remainder = (float)fabs (modf ((time - *firstTime) / timeRange, &numCycles));
	}
	*factoredTime = timeRange * (*remainder);	
}

float Tr2MayaAnimationEngine::evaluateInfinityCyclePre( const Tr2MayaAnimCurve *animCurve, void* firstSegment, size_t segmentSize, float time )
{
	float	factoredTime, firstTime, lastTime;
	float	remainder;

	evaluateInfinityCycleSetup(animCurve, firstSegment, segmentSize, time, &firstTime, &lastTime, &factoredTime, &remainder);
	factoredTime = lastTime - factoredTime;


	return evaluateImp(animCurve, factoredTime, firstSegment, segmentSize);
}

float Tr2MayaAnimationEngine::evaluateInfinityCyclePost( const Tr2MayaAnimCurve *animCurve, void* firstSegment, size_t segmentSize, float time )
{
	float	factoredTime, firstTime, lastTime;
	float	remainder;
	evaluateInfinityCycleSetup(animCurve, firstSegment, segmentSize, time, &firstTime, &lastTime, &factoredTime, &remainder);
	factoredTime = firstTime + factoredTime;

	return evaluateImp(animCurve, factoredTime, firstSegment, segmentSize);
}

void Tr2MayaAnimationEngine::evaluateInfinityCycleRelativeSetup( const Tr2MayaAnimCurve *animCurve, void* firstSegment, size_t segmentSize, float time, 
	float* firstTime, float* lastTime, float *factoredTime, 
	float *remainder, float* valueRange, double *numCycles ) const
{
	float timeRange;
	*firstTime = static_cast<Tr2MayaAnimSegment*>( firstSegment )->m_time;
	*valueRange = animCurve->m_endValue - static_cast<Tr2MayaAnimSegment*>( firstSegment )->m_value;
	*lastTime = animCurve->m_endTime;
	timeRange = *lastTime - *firstTime;

	if (time > *lastTime) 
	{
		*remainder = (float)fabs (modf ((time - *lastTime) / timeRange, numCycles));
	}
	else 
	{
		*remainder = (float)fabs (modf ((time - *firstTime) / timeRange, numCycles));
	}
	*factoredTime = timeRange * (*remainder);	
	*numCycles = fabs (*numCycles) + 1;
}

float Tr2MayaAnimationEngine::evaluateInfinityCycleRelativePre( const Tr2MayaAnimCurve *animCurve, void* firstSegment, size_t segmentSize, float time )
{
	float	factoredTime, firstTime, lastTime, value;
	float	valueRange = 0.0f;
	float	remainder;
	double numCycles;
	evaluateInfinityCycleRelativeSetup(animCurve, firstSegment, segmentSize, time, &firstTime, &lastTime, &factoredTime, &remainder, &valueRange, &numCycles);
	factoredTime = lastTime - factoredTime;

	value = evaluateImp(animCurve, factoredTime, firstSegment, segmentSize);
	value -= (float)(numCycles * valueRange);
	return value;
}

float Tr2MayaAnimationEngine::evaluateInfinityCycleRelativePost( const Tr2MayaAnimCurve *animCurve, void* firstSegment, size_t segmentSize, float time )
{
	float	factoredTime, firstTime, lastTime, value;
	float	valueRange = 0.0f;
	float	remainder;

	double numCycles;
	evaluateInfinityCycleRelativeSetup(animCurve, firstSegment, segmentSize, time, &firstTime, &lastTime, &factoredTime, &remainder, &valueRange,  &numCycles);
	factoredTime = firstTime + factoredTime;

	value = evaluateImp(animCurve, factoredTime, firstSegment, segmentSize);
	value += (float)(numCycles * valueRange);
	return value;
}

void Tr2MayaAnimationEngine::evaluateInfinityOscillateSetup( const Tr2MayaAnimCurve *animCurve, void* firstSegment, size_t segmentSize, float time, 
														float* firstTime, float* lastTime, float *factoredTime, 
														float *remainder, double *numCycles ) const
{
	float timeRange;
	/* find the number of cycles of the base animation curve */
	*firstTime = static_cast<Tr2MayaAnimSegment*>( firstSegment )->m_time;
	*lastTime = animCurve->m_endTime;
	timeRange = *lastTime - *firstTime;

	if (time > *lastTime) 
	{
		*remainder = (float)fabs (modf ((time - *lastTime) / timeRange, numCycles));
	}
	else 
	{
		*remainder = (float)fabs (modf ((time - *firstTime) / timeRange, numCycles));
	}
	*factoredTime = timeRange * (*remainder);	
	*numCycles = fabs (*numCycles) + 1;
}

float Tr2MayaAnimationEngine::evaluateInfinityOscillatePre( const Tr2MayaAnimCurve *animCurve, void* firstSegment, size_t segmentSize, float time )
{
	float	factoredTime, firstTime, lastTime;
	float	remainder;
	double numCycles, notUsed;

	evaluateInfinityOscillateSetup( animCurve, firstSegment, segmentSize, time, &firstTime, &lastTime, &factoredTime, &remainder, &numCycles );
	
	if ( modf( numCycles / 2.0, &notUsed ) != 0 ) 
	{
		factoredTime = firstTime + factoredTime;
	}
	else 
	{
		factoredTime = lastTime - factoredTime;
	}

	return evaluateImp(animCurve, factoredTime, firstSegment, segmentSize);
}

float Tr2MayaAnimationEngine::evaluateInfinityOscillatePost( const Tr2MayaAnimCurve *animCurve, void* firstSegment, size_t segmentSize, float time )
{
	float	factoredTime, firstTime, lastTime;
	float	remainder;
	double numCycles, notUsed;

	evaluateInfinityOscillateSetup( animCurve, firstSegment, segmentSize, time, &firstTime, &lastTime, &factoredTime, &remainder, &numCycles );

	if ( modf( numCycles / 2.0f, &notUsed ) != 0 ) 
	{
		factoredTime = lastTime - factoredTime;
	}
	else 
	{
		factoredTime = firstTime + factoredTime;
	}

	return evaluateImp(animCurve, factoredTime, firstSegment, segmentSize);
}

bool Tr2MayaAnimationEngine::find(const Tr2MayaAnimCurve *animCurve, float time, void* firstSegment, size_t segmentSize, int *index) const
{
	/* use a binary search to find the key */
	*index = 0;
	int len = animCurve->m_numSegments + 1;

	if (len > 0) 
	{
		int low = 0;
		int high = len - 1;
		do 
		{
			float stime;
			int mid = (low + high) >> 1;	
			if( mid < ( len - 1 ) )
			{
				void* segment = (char*)firstSegment + (segmentSize*mid);
				stime = static_cast<Tr2MayaAnimSegment*>( segment )->m_time;
			}
			else
			{
				stime = animCurve->m_endTime;
			}
			
			if (time < stime) 
			{
				high = mid - 1;			/* Search lower half */
			} 
			else if (time > stime) 
			{
				low  = mid + 1;			/* Search upper half */
			}
			else 
			{
				*index = mid;	/* Found item! */
				return true;
			}
		} while (low <= high);
		*index = low;
	}
	return false;
}

float Tr2MayaAnimationEngine::getLength( int curveIndex )
{
	if( m_curves.size() == 0 || curveIndex >= (int)m_curves.size() )
	{
		return 0.0f;
	}

	Tr2MayaAnimSegment *firstSegment = NULL;
	Tr2MayaAnimCurve* curve = &m_curves[curveIndex];
	int segmentOffset = curve->m_segmentOffset;
	if( curve->m_isWeighted )
	{
		firstSegment = (Tr2MayaAnimSegment*)&m_bezierSegments[segmentOffset];
	}
	else
	{
		firstSegment = (Tr2MayaAnimSegment*)&m_hermiteSegments[segmentOffset];
	}
	
	return curve->m_endTime - firstSegment->m_time;
}

float Tr2MayaAnimationEngine::evaluate( int curveIndex, float time )
{
	if( m_curves.size() == 0 || curveIndex >= (int)m_curves.size() )
	{
		return 0.0f;
	}

	m_currentCurveIndex = curveIndex;

	if( m_evalCache == NULL )
	{				
		m_evalCache = ( int* )CCP_MALLOC( "engineAnimEvaluate(): Curve Cache Data",  (m_curves.size())*sizeof(int) );			
		if( m_evalCache == NULL )
		{
			CCP_LOGERR( "engineAnimEvaluate() -> Couldn't allocate memory for one of the python parameters" );			
			return 0.0f;
		}
		memset(m_evalCache, -1, (m_curves.size())*sizeof(int));
	}


	Tr2MayaAnimCurve *animCurve = &m_curves[curveIndex];
	void *firstSegment = NULL;
	size_t segmentSize = 0;

	if(animCurve->m_isWeighted)
	{
		firstSegment = (void*)&m_bezierSegments[animCurve->m_segmentOffset];
		segmentSize = sizeof(Tr2MayaAnimBezierSegment);
	}
	else
	{
		firstSegment = (void*)&m_hermiteSegments[animCurve->m_segmentOffset];
		segmentSize = sizeof(Tr2MayaAnimHermiteSegment);
	}

	/* check if the time falls into the pre-infinity */
	if( time < static_cast<Tr2MayaAnimSegment*>( firstSegment )->m_time ) 
	{
		if (animCurve->m_preInfinity == INFINITY_CONSTANT) 
		{
			return static_cast<Tr2MayaAnimSegment*>( firstSegment )->m_value;
		}
		return evaluateInfinities(animCurve, firstSegment, segmentSize, time, true);
	}

	/* check if the time falls into the post-infinity */
	if (time > animCurve->m_endTime) 
	{
		if (animCurve->m_postInfinity == INFINITY_CONSTANT) 
		{
			return (animCurve->m_endValue);
		}
		return evaluateInfinities(animCurve, firstSegment, segmentSize, time, false);
	}

	return evaluateImp(animCurve, time, firstSegment, segmentSize);
}

float Tr2MayaAnimationEngine::evaluateImp( const Tr2MayaAnimCurve *animCurve, float time, void* firstSegment, size_t segmentSize )
{
	bool withinInterval = false;
	Tr2MayaAnimSegment* nextSegment = NULL;
	Tr2MayaAnimSegment* lastSegment = NULL;
	int index;

	/* check to see if the time falls within the last segment we evaluated */
	if (m_evalCache[m_currentCurveIndex] >= 0) 
	{
		lastSegment = reinterpret_cast<Tr2MayaAnimSegment*>( static_cast<char*>( firstSegment ) + ( segmentSize * m_evalCache[m_currentCurveIndex] ) );
		if ((m_evalCache[m_currentCurveIndex] < (animCurve->m_numSegments - 1)) && (time > lastSegment->m_time)) 
		{			
			nextSegment = reinterpret_cast<Tr2MayaAnimSegment*>( static_cast<char*>( firstSegment ) + ( segmentSize * ( m_evalCache[m_currentCurveIndex] + 1 ) ) );
			if (time == nextSegment->m_time) 
			{
				m_evalCache[m_currentCurveIndex]++;
				return nextSegment->m_value;
			}
			else if (time < nextSegment->m_time ) 
			{
				index = m_evalCache[m_currentCurveIndex] + 1;
				withinInterval = true;
			}
			else
			{
				nextSegment = NULL;
			}
		}
		else if ((m_evalCache[m_currentCurveIndex] > 0) && (time < lastSegment->m_time)) 
		{			
			Tr2MayaAnimSegment* prevSegment = reinterpret_cast<Tr2MayaAnimSegment*>( static_cast<char*>( firstSegment ) + ( segmentSize * ( m_evalCache[m_currentCurveIndex] - 1 ) ) );
			if (time > prevSegment->m_time) 
			{
				index = m_evalCache[m_currentCurveIndex];
				withinInterval = true;
			}
			else if (time == prevSegment->m_time) 
			{
				m_evalCache[m_currentCurveIndex]--;
				return prevSegment->m_value;
			}
		}
	}

	/* it does not, so find the new segment */
	if( !withinInterval ) 
	{
		if( find( animCurve, time, firstSegment, segmentSize, &index ) || ( index == 0 ) ) 
		{
			/*
			//	Exact match or before range of this action,
			//	return exact keyframe value.
			*/
			if( index == animCurve->m_numSegments )
			{
				index--;
				m_evalCache[m_currentCurveIndex] = index;
				return animCurve->m_endValue;
			}
			else
			{				
				m_evalCache[m_currentCurveIndex] = index;
				return reinterpret_cast<Tr2MayaAnimSegment*>( static_cast<char*>( firstSegment ) + segmentSize * index )->m_value;	
			}		
		}
		else if( index == (animCurve->m_numSegments+1) )  
		{
			/* Beyond range of this action return end keyframe value */
			m_evalCache[m_currentCurveIndex] = 0;
			return animCurve->m_endValue;
		}
	}

	if( m_evalCache[m_currentCurveIndex] != (index - 1) ) 
	{
		m_evalCache[m_currentCurveIndex] = index - 1;
		lastSegment = reinterpret_cast<Tr2MayaAnimSegment*>( static_cast<char*>( firstSegment ) + segmentSize * m_evalCache[m_currentCurveIndex] );
		if(nextSegment == NULL)
		{
			nextSegment = reinterpret_cast<Tr2MayaAnimSegment*>( static_cast<char*>( firstSegment ) + segmentSize * index );
		}
	}

	if( animCurve->m_isWeighted )
	{
		Tr2MayaAnimBezierSegment* bSegment = static_cast<Tr2MayaAnimBezierSegment*>( lastSegment );
		/* finally we can evaluate the segment */
		if(bSegment->m_isStep) 
		{
			return bSegment->m_value;
		}
		else if(bSegment->m_isStepNext) 
		{
			if(nextSegment == NULL)
			{
				return animCurve->m_endValue;
			}
			return nextSegment->m_value;
		}
		else
		{
			float nextKeyTime = animCurve->m_endTime;
			if( ( m_evalCache[m_currentCurveIndex] + 1 ) < animCurve->m_numSegments )
			{
				nextKeyTime = nextSegment->m_time;
			}
			return evaluateBezier(bSegment, time, nextKeyTime);
		}
	}
	else
	{
		Tr2MayaAnimHermiteSegment* hSegment = static_cast<Tr2MayaAnimHermiteSegment*>( lastSegment );
		/* finally we can evaluate the segment */
		if(hSegment->m_isStep) 
		{
			return hSegment->m_value;
		}
		else if(hSegment->m_isStepNext) 
		{
			if(nextSegment == NULL)
			{
				return animCurve->m_endValue;
			}
			return nextSegment->m_value;
		}
		else 
		{
			return evaluateHermite(hSegment, time);
		}
	}
}

#endif
