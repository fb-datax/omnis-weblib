///
/// WebViewListener Impl
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
#include "WebViewListener.h"
#include "WebBrowser.h"

using namespace WebLib;

/*
	Constructor
*/
WebViewListener::WebViewListener(HWND pHWnd)
{
	mHWnd = pHWnd;	// Ref auf das Fenster via hwnd
}

/*
	Destructor
*/
WebViewListener::~WebViewListener()
{	
}

void WebViewListener::OnChangeTitle(Awesomium::WebView* caller, const Awesomium::WebString& title) {
	// Event myEvent1 occurred. Send event to OMNIS 
	EXTCompInfo* eci = new EXTCompInfo(); 
	eci->mParamFirst = 0; 
	
	// ECI mit Parametern befüllen 
	EXTfldval _newTitel;
	OmnisTools::getEXTFldValFromString(_newTitel,OmnisTools::getStringFromWebString(title));
	ECOaddParam(eci,&_newTitel,0,0,0,1,0);  

	// Send event to OMNIS 
	qbool eventOk = ECOsendCompEvent( mHWnd, eci, evOnTitelChange, qtrue ); 

	// Delete parameters from EXTCompInfo structure 
	ECOmemoryDeletion( eci ); 

	// Delete eci structure 
	delete eci; 
}
void WebViewListener::OnChangeAddressBar(Awesomium::WebView* caller,const Awesomium::WebURL& url){

	// Event myEvent1 occurred. Send event to OMNIS 
	EXTCompInfo* eci = new EXTCompInfo(); 
	eci->mParamFirst = 0; 
	
	// ECI mit Parametern befüllen 
	EXTfldval _url;
	OmnisTools::getEXTFldValFromString(_url,OmnisTools::getStringFromWebString(url.spec()));
	ECOaddParam(eci,&_url,0,0,0,1,0);  

	// Send event to OMNIS 
	qbool eventOk = ECOsendCompEvent( mHWnd, eci, evOnAdressBarChanged, qtrue ); 

	// Delete parameters from EXTCompInfo structure 
	ECOmemoryDeletion( eci ); 

	// Delete eci structure 
	delete eci; 

}
void WebViewListener::OnAddConsoleMessage(Awesomium::WebView* caller,const Awesomium::WebString& message,int line_number,const Awesomium::WebString& source){

	// Event myEvent1 occurred. Send event to OMNIS 
	EXTCompInfo* eci = new EXTCompInfo(); 
	eci->mParamFirst = 0; 
	
	// ECI mit Parametern befüllen 
	EXTfldval _message;
	OmnisTools::getEXTFldValFromString(_message,OmnisTools::getStringFromWebString(message));
	ECOaddParam(eci,&_message,0,0,0,1,0);  

	// ECI mit Parametern befüllen 
	EXTfldval _lineNumber;
	OmnisTools::getEXTFldValFromInt(_lineNumber,line_number);
	ECOaddParam(eci,&_lineNumber,0,0,0,2,0);  

	// ECI mit Parametern befüllen 
	EXTfldval _source;
	OmnisTools::getEXTFldValFromString(_source,OmnisTools::getStringFromWebString(source));
	ECOaddParam(eci,&_source,0,0,0,3,0);  

	// Send event to OMNIS 
	qbool eventOk = ECOsendCompEvent( mHWnd, eci, evOnConsoleMessageAdded, qtrue ); 

	// Delete parameters from EXTCompInfo structure 
	ECOmemoryDeletion( eci ); 

	// Delete eci structure 
	delete eci; 
};
void WebViewListener::OnShowCreatedWebView(Awesomium::WebView* caller,Awesomium::WebView* new_view,const Awesomium::WebURL& opener_url,const Awesomium::WebURL& target_url,const Awesomium::Rect& initial_pos,bool is_popup) {

	
	// neu geöffnete web View wieder entfernen
	new_view->Stop();
	new_view->Destroy();
	
	Awesomium::WebCore* mWebCore = Awesomium::WebCore::instance();
	mWebCore->Update();

	// Event myEvent1 occurred. Send event to OMNIS 
	EXTCompInfo* eci = new EXTCompInfo(); 
	eci->mParamFirst = 0; 
	
	// ECI mit Parametern befüllen 
	EXTfldval _opener_url;
	OmnisTools::getEXTFldValFromString(_opener_url,OmnisTools::getStringFromWebString(opener_url.spec()));
	ECOaddParam(eci,&_opener_url,0,0,0,1,0);  

	// ECI mit Parametern befüllen 
	EXTfldval _target_url;
	OmnisTools::getEXTFldValFromString(_target_url,OmnisTools::getStringFromWebString(target_url.spec()));
	ECOaddParam(eci,&_target_url,0,0,0,2,0);  

	// Send event to OMNIS 
	qbool eventOk = ECOsendCompEvent( mHWnd, eci, evOnOpenNewWindow, qtrue ); 

	// Delete parameters from EXTCompInfo structure 
	ECOmemoryDeletion( eci ); 

	// Delete eci structure 
	delete eci;
};
