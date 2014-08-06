///
/// WebDownloadListener Impl
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
#include "WebDownloadListener.h"
#include "WebBrowser.h"

using namespace WebLib;

/*
	Constructor
*/
WebDownloadListener::WebDownloadListener(HWND pHWnd)
{
	mHWnd = pHWnd;	// Ref auf das Fenster via hwn
}
/*
	Destructor
*/
WebDownloadListener::~WebDownloadListener()
{}


void WebDownloadListener::OnRequestDownload(Awesomium::WebView* caller, int download_id,
										 const Awesomium::WebURL& url, const Awesomium::WebString& suggested_filename,
										 const Awesomium::WebString& mime_type)
{

	// Event myEvent1 occurred. Send event to OMNIS 
	EXTCompInfo* eci = new EXTCompInfo(); 
	eci->mParamFirst = 0; 

	// ECI mit Parametern befüllen 
	EXTfldval _downloadId;
	OmnisTools::getEXTFldValFromInt(_downloadId,download_id);
	ECOaddParam(eci,&_downloadId,0,0,0,1,0);  

	
	EXTfldval _url;
	OmnisTools::getEXTFldValFromString(_url,OmnisTools::getStringFromWebString(url.spec()));
	ECOaddParam(eci,&_url,0,0,0,2,0);  

	EXTfldval _suggested_filename;
	OmnisTools::getEXTFldValFromString(_suggested_filename,OmnisTools::getStringFromWebString(suggested_filename));
	ECOaddParam(eci,&_suggested_filename,0,0,0,3,0);  

	EXTfldval _mime_Type;
	OmnisTools::getEXTFldValFromString(_mime_Type,OmnisTools::getStringFromWebString(mime_type));
	ECOaddParam(eci,&_mime_Type,0,0,0,4,0);  

	// Send event to OMNIS 
	qbool eventOk = ECOsendCompEvent( mHWnd, eci, evOnDownloadRequest, qtrue ); 

	// Delete parameters from EXTCompInfo structure 
	ECOmemoryDeletion( eci ); 

	// Delete eci structure 
	delete eci; 

	
}

void WebDownloadListener::OnUpdateDownload(Awesomium::WebView* caller,int download_id,int64 total_bytes,
											int64 received_bytes, int64 current_speed) {

	// Event myEvent1 occurred. Send event to OMNIS 
	EXTCompInfo* eci = new EXTCompInfo(); 
	eci->mParamFirst = 0; 

	// ECI mit Parametern befüllen 
	EXTfldval _downloadId;
	OmnisTools::getEXTFldValFromInt(_downloadId,download_id);
	ECOaddParam(eci,&_downloadId,0,0,0,1,0);  
	
	EXTfldval _total;
	OmnisTools::getEXTFldValFromInt64(_total,total_bytes);
	ECOaddParam(eci,&_total,0,0,0,2,0); 

	EXTfldval _received_bytes;
	OmnisTools::getEXTFldValFromInt64(_received_bytes,received_bytes);
	ECOaddParam(eci,&_received_bytes,0,0,0,3,0);

	EXTfldval _current_speed;
	OmnisTools::getEXTFldValFromInt64(_current_speed,current_speed);
	ECOaddParam(eci,&_current_speed,0,0,0,4,0);  

	// Send event to OMNIS 
	qbool eventOk = ECOsendCompEvent( mHWnd, eci, evOnDownloadUpdate, qtrue ); 

	// Delete parameters from EXTCompInfo structure 
	ECOmemoryDeletion( eci ); 

	// Delete eci structure 
	delete eci; 

};

void WebDownloadListener::OnFinishDownload(Awesomium::WebView* caller,int download_id,
										const Awesomium::WebURL& url,const Awesomium::WebString& saved_path) 
{

	// Event myEvent1 occurred. Send event to OMNIS 
	EXTCompInfo* eci = new EXTCompInfo(); 
	eci->mParamFirst = 0; 

	// ECI mit Parametern befüllen 
	EXTfldval _downloadId;
	OmnisTools::getEXTFldValFromInt(_downloadId,download_id);
	ECOaddParam(eci,&_downloadId,0,0,0,1,0);  
	
	EXTfldval _url;
	OmnisTools::getEXTFldValFromString(_url,OmnisTools::getStringFromWebString(url.spec()));
	ECOaddParam(eci,&_url,0,0,0,2,0);  

	EXTfldval _path;
	OmnisTools::getEXTFldValFromString(_path,OmnisTools::getStringFromWebString(saved_path));
	ECOaddParam(eci,&_path,0,0,0,3,0);  

	// Send event to OMNIS 
	qbool eventOk = ECOsendCompEvent( mHWnd, eci, evOnDownloadFinish, qtrue ); 

	// Delete parameters from EXTCompInfo structure 
	ECOmemoryDeletion( eci ); 

	// Delete eci structure 
	delete eci; 

};