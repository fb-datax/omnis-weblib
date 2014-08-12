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

/* OMNIS TOOLS (HEADER)
 * 
 * This is a colleciton of tools for creating omnis components created by David McKeone and Modified by Florian Binder
 *
 * October 4, 2010 David McKeone (Created)
 * July 7, 2014 Florian Binder DATAX AG (Modified)
 */


//Omnis includes
#include <omnis/extcomp.he>
#include <omnis/chrbasic.he>


// C Library includes
#include <ctime>

// STD Library includes
#include <string>
#include <map>
#include <vector>

#include <Awesomium/Platform.h>
#include <Awesomium/WebString.h>

#ifndef OMNIS_TOOLS_HE_
#define OMNIS_TOOLS_HE_

namespace OmnisTools {
 
	// EXTfldval helpers
	std::string getStringFromEXTFldVal(EXTfldval&);
	
	Awesomium::WebString getWebStringFromEXTFldVal(EXTfldval& fVal);
	Awesomium::WebString getWebStringFromStr(std::string& value);
	std::string getStringFromWebString(const Awesomium::WebString& value);
	
	int getIntFromEXTFldVal(EXTfldval& fVal);

	void getEXTFldValFromString(EXTfldval&, const std::string);
    void getEXTFldValFromChar(EXTfldval&, const char*);
	void getEXTFldValFromInt(EXTfldval& fVal, int);
	void getEXTFldValFromInt64(EXTfldval& fVal, int64);
	void getEXTFldValFromBool(EXTfldval& fVal, bool b);
	
	str255 initStr255(const char*);

	// Allg Helpers
	qchar* getQCharFromString( const std::string readString, qlong &retLength );
	void logToTrace(const char* msg);
}

#endif // OMNIS_TOOLS_HE_
