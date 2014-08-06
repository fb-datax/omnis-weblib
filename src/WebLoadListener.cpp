///
/// WebLoadListener Impl
/// 
/// 25.07.14/fb 
///
///

/// define Consts 

/// Include required Header Files 
#include <iostream>
#include <string>
#include <omnis/extcomp.he>
#include <omnis/extfval.he>
#include <omnis/hwnd.he>
#include <omnis/gdi.he>
#include "OmnisTools.h"
#include "WebLoadListener.h"
#include "WebBrowser.h"

using namespace WebLib;

/*
	Constructor
*/
WebLoadListener::WebLoadListener(HWND pHWnd)
{
	mHWnd = pHWnd;	// Ref auf das Fenster via hwn
}
/*
	Destructor
*/
WebLoadListener::~WebLoadListener()
{}

void WebLoadListener::OnDocumentReady(Awesomium::WebView* caller,const Awesomium::WebURL& url) {

	// Event myEvent1 occurred. Send event to OMNIS 
	EXTCompInfo* eci = new EXTCompInfo(); 
	eci->mParamFirst = 0; 
	
	// ECI mit Parametern befüllen 
	EXTfldval spec;
	OmnisTools::getEXTFldValFromString(spec,OmnisTools::getStringFromWebString(url.spec()));
	ECOaddParam(eci,&spec,0,0,0,1,0);  

	EXTfldval scheme;
	OmnisTools::getEXTFldValFromString(scheme,OmnisTools::getStringFromWebString(url.scheme()));
	ECOaddParam(eci,&scheme,0,0,0,2,0);  

	EXTfldval host;
	OmnisTools::getEXTFldValFromString(host,OmnisTools::getStringFromWebString(url.host()));
	ECOaddParam(eci,&host,0,0,0,3,0);  

	EXTfldval port;
	OmnisTools::getEXTFldValFromString(port,OmnisTools::getStringFromWebString(url.port()));
	ECOaddParam(eci,&port,0,0,0,4,0);  

	EXTfldval path;
	OmnisTools::getEXTFldValFromString(path,OmnisTools::getStringFromWebString(url.path()));
	ECOaddParam(eci,&path,0,0,0,5,0);  

	EXTfldval query;
	OmnisTools::getEXTFldValFromString(query,OmnisTools::getStringFromWebString(url.query()));
	ECOaddParam(eci,&query,0,0,0,6,0);  

	EXTfldval anchor;
	OmnisTools::getEXTFldValFromString(anchor,OmnisTools::getStringFromWebString(url.anchor()));
	ECOaddParam(eci,&anchor,0,0,0,7,0); 

	EXTfldval filename;
	OmnisTools::getEXTFldValFromString(filename,OmnisTools::getStringFromWebString(url.filename()));
	ECOaddParam(eci,&filename,0,0,0,8,0); 
	

	// Send event to OMNIS 
	qbool eventOk = ECOsendCompEvent( mHWnd, eci, evOnDocumentReady, qtrue ); 

	// Delete parameters from EXTCompInfo structure 
	ECOmemoryDeletion( eci ); 

	// Delete eci structure 
	delete eci; 
}

void WebLoadListener::OnFailLoadingFrame(Awesomium::WebView* caller,int64 frame_id,bool is_main_frame,const Awesomium::WebURL& url,int error_code,
										 const Awesomium::WebString& error_desc){

	// Event myEvent1 occurred. Send event to OMNIS 
	EXTCompInfo* eci = new EXTCompInfo(); 
	eci->mParamFirst = 0; 
	
	// ECI mit Parametern befüllen 
	EXTfldval _errorCode;
	OmnisTools::getEXTFldValFromInt(_errorCode,error_code);
	ECOaddParam(eci,&_errorCode,0,0,0,1,0);  

	EXTfldval _errorDesc;
	OmnisTools::getEXTFldValFromString(_errorDesc,OmnisTools::getStringFromWebString(error_desc));
	ECOaddParam(eci,&_errorDesc,0,0,0,2,0);  

	EXTfldval _url;
	OmnisTools::getEXTFldValFromString(_url,OmnisTools::getStringFromWebString(url.spec()));
	ECOaddParam(eci,&_url,0,0,0,3,0);  

	// Send event to OMNIS 
	qbool eventOk = ECOsendCompEvent( mHWnd, eci, evOnFrameLoadingFailed, qtrue ); 

	// Delete parameters from EXTCompInfo structure 
	ECOmemoryDeletion( eci ); 

	// Delete eci structure 
	delete eci; 
};
