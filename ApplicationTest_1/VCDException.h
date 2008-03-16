// 
// File:   VCDException.h
// Author: nick
//
// Created on 10 March 2008, 12:44
//

#ifndef _VCDEXCEPTION_H
#define	_VCDEXCEPTION_H

#include <exception>

using std::exception;

class VCDException : public exception
{
    
public:
    
    VCDException();
    
};

#endif	/* _VCDEXCEPTION_H */

