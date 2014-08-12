/* Copyright (c) 2010 David McKeone
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 * derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* OMNIS TOOLS (IMPLEMENTATION)
 * 
 * This is a colleciton of tools for creating omnis components created by David McKeone and Modified by Florian Binder
 *
 * October 4, 2010 David McKeone (Created)
 * July 7, 2014 Florian Binder DATAX AG (Modified)
 */

#include "OmnisTools.h"
#include <iconv/iconv.hpp>
#include <Awesomium/STLHelpers.h>

#include <sstream>
#include <iostream>
#include <iterator>
#include <map>
#include <vector>


// Set an existing EXTfldval object from a std::string
#ifdef isunicode
qchar* OmnisTools::getQCharFromString(const std::string readString, qlong &retLength) {
	qlong length = readString.size();
	
	// Cast-away constness of c_str() pointer 
	char* cString = const_cast<char*>(readString.c_str());
	
	// Feed into raw byte data
	qbyte* utf8data = reinterpret_cast<qbyte*> (cString);
	
	// Allocate new qchar* string
	qchar* omnisString = new qchar[length];
	
	// Convert to Omnis Character field
	retLength = CHRunicode::utf8ToChar(utf8data, length, omnisString);  // Convert characters into Omnis Char Field
	
	return omnisString;
}

// Set an existing EXTfldval object from a std::string
void OmnisTools::getEXTFldValFromString(EXTfldval& fVal, const std::string readString) {
	qlong length;
	qchar* omnisString = getQCharFromString(readString, length);
	
	fVal.setChar(omnisString, length); // Set value of character field, but exclude the last character since it will be the null terminator from the C String
	
	// Clean-up
	delete [] omnisString;
}
#else
// Get a dynamically allocated qchar* array from a std::string
qchar* OmnisTools::getQCharFromString(const std::string readString, qlong &retLength) {

	// Get a dynamically allocated qchar* array from a std::string
	qlong length = retLength = readString.size();
	qoschar* cString = const_cast<qoschar*>(readString.c_str());
	qoschar* utf8data = reinterpret_cast<qoschar*>(cString);
	CHRconvFromOs test(utf8data,length);
	
	qchar* qcharData = new qchar[length+1];
	OMstrcpy(qcharData, test.dataPtr());
	return qcharData;
}

void OmnisTools::getEXTFldValFromString(EXTfldval& fVal, const std::string readString) {
	
	qlong length = readString.size();
	qoschar* cString = const_cast<qoschar*>(readString.c_str());
	qoschar* utf8data = reinterpret_cast<qoschar*>(cString);
	CHRconvFromOs test(utf8data,length);
	qchar* qcharData = new qchar[length+1];
	OMstrcpy(qcharData, test.dataPtr());
	fVal.setChar(qcharData, length); // Set value of character field, but exclude the last character since it will be the null terminator from the C String
	
	// Clean-up
	delete [] qcharData;
}
#endif

// Set an existing EXTfldval object from a std::wstring
void OmnisTools::getEXTFldValFromChar(EXTfldval& fVal, const char* readChar) {
    std::string readString;
    if (readChar)
        readString = readChar;
    else
        readString = "";
    
    getEXTFldValFromString(fVal, readString);
}

// Get a std::string from an EXTfldval object
#ifdef isunicode
std::string OmnisTools::getStringFromEXTFldVal(EXTfldval& fVal) {
	std::string retString;
	
	// Get a qchar* string
	qlong maxLength = fVal.getBinLen()+1; // Use binary length as approximation of maximum size
	qlong length = 0, stringLength = 0;
	qchar* omnisString = new qchar[maxLength];
	fVal.getChar(maxLength, omnisString, length);
	
	// Translate qchar* string into UTF8 binary
	qbyte* utf8data = reinterpret_cast<qbyte*>(omnisString);
	stringLength = CHRunicode::charToUtf8(omnisString, length, utf8data);
	
	// Translate UTF8 binary into char* string
	char* cString = reinterpret_cast<char*> (utf8data);
	
	// Create standard string
	retString = std::string(cString,stringLength);
	
	// Clean-up
	delete [] omnisString;
	
	return retString;
}
#else
std::string OmnisTools::getStringFromEXTFldVal(EXTfldval& fVal) {
	std::string retString;
	
	// Get a qchar* string
	qlong maxLength = fVal.getBinLen()*2; // Use binary length as approximation of maximum size
	qlong length = 0, stringLength = 0;
	qchar* omnisString = new qchar[maxLength];
	fVal.getChar(maxLength, omnisString, length);

	qoschar* utf8data = reinterpret_cast<qoschar*>(omnisString);
	stringLength = CHRconvToOs::convToOs(omnisString,length,utf8data);
	char* cString = reinterpret_cast<char*> (utf8data);
	
	// Create standard string
	retString = std::string(cString,stringLength);
	
	// Clean-up
	delete [] omnisString;
	
	return retString;
}
#endif


// Get a std::string from an EXTfldval object
Awesomium::WebString OmnisTools::getWebStringFromEXTFldVal(EXTfldval& fVal) {
	std::string retString;
	std::string input = getStringFromEXTFldVal(fVal);
    Awesomium::WebString webString = getWebStringFromStr(input);	
	return webString;
}

// Get a std::string from an EXTfldval object
#ifdef isunicode
Awesomium::WebString OmnisTools::getWebStringFromStr(std::string& value) {
	Awesomium::WebString webString = Awesomium::ToWebString(value);	
	return webString;
}
#else
Awesomium::WebString OmnisTools::getWebStringFromStr(std::string& value) {
	Awesomium::WebString webString;
	if (value.length()<=0){
		webString = Awesomium::ToWebString("");
	} else {
		iconvpp::converter conv("UTF-8","ISO-8859-1",true,value.length()*2); 
		std::string output;
		conv.convert(value, output);
		webString = Awesomium::ToWebString(output);
	}
	
	return webString;
}
#endif


#ifdef isunicode
std::string OmnisTools::getStringFromWebString(const Awesomium::WebString& value) {
	std::string output = Awesomium::ToString(value);
	return output;
}
#else
std::string OmnisTools::getStringFromWebString(const Awesomium::WebString& value) {
	std::string output;
	std::string input = Awesomium::ToString(value);
	if (input.length()<=0){
		output = input;
	} else {
		iconvpp::converter conv("ISO-8859-1","UTF-8",true,input.length()); 
		conv.convert(input, output);
	}
    return output;
}
#endif


// Return a C++ int from an EXTfldval
int OmnisTools::getIntFromEXTFldVal(EXTfldval& fVal) {
	qlong omnInt = fVal.getLong();
	
	if (omnInt < INT_MIN || omnInt > INT_MAX) {
        omnInt = 0; // zero out any numbers that exceed
	}
	return static_cast<int>( omnInt );
}


void OmnisTools::getEXTFldValFromBool(EXTfldval& fVal, bool b) {
	qshort omBool;
	if (b==true) 
        omBool = 2;
	else if (b==false)
        omBool = 1;
    
	fVal.setBool(omBool);
}


// Get an EXTfldval for a C++ int
void OmnisTools::getEXTFldValFromInt(EXTfldval& fVal, int i) {
	fVal.setLong(static_cast<qlong>(i));
}

// Get an EXTfldval for a C++ int
void OmnisTools::getEXTFldValFromInt64(EXTfldval& fVal, int64 i) {
	if (i < INT_MIN || i > INT_MAX) {
        i = 0; // zero out any numbers that exceed
	}
	fVal.setLong(static_cast<qlong>(i));
}


// Get a str255 object for a character constant (No string if it doesn't correspond to the conditions)
#ifdef isunicode
str255 OmnisTools::initStr255(const char* in) {
    str255 theString;
    qshort length = strlen(in);
    if (length > 0 && length <= 255) {
        theString.setUtf8((qbyte*) in, strlen(in));
    }
    return theString;
} 
#else
str255 OmnisTools::initStr255(const char* in) {    
    return str255(in);
} 
#endif

void OmnisTools::logToTrace(const char* msg)
{
	#if defined(IS_DEBUG)
		str255 logString = initStr255(msg);
		ECOaddTraceLine(&logString);
	#endif
}
