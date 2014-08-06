////////////////////////////////////////////////////////////
//
//    Created:   August 2014
//    Copyright: CCP 2014
//
#pragma once
#ifndef StringUtils_H
#define StringUtils_H

// --------------------------------------------------------------------------------
// Description:
//   Inserts insertStr before the last instance of beforeSubstr in baseString.
// --------------------------------------------------------------------------------
inline bool InsertStringStub( std::string& baseString, const char* beforeSubstr, const char* insertStr )
{
	size_t index = baseString.rfind( beforeSubstr );
	if( index == std::string::npos )
	{
		return false;
	}

	baseString.insert( index, insertStr );
	return true;
}

// --------------------------------------------------------------------------------
// Description:
//   Split up a string into a vector of strings using a seperator.
//   Similar to Python's split()
// --------------------------------------------------------------------------------
inline void SplitString( std::vector<std::string>& result, const char* original, char seperator )
{
	std::string originalStr( original );
	for( size_t i = 0; i < originalStr.size(); ++i )
	{
		size_t next = originalStr.find( seperator, i );
		if( next == std::string::npos )
		{
			next = originalStr.size();
		}

		result.push_back( originalStr.substr( i, next - i ) );

		i = next;
	}
}




#endif // StringUtils_H