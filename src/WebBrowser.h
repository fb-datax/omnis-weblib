///
/// WebBrowser Header
/// 
/// 25.07.14/fb 
///
///

/// define Consts 
#ifndef _WEBBROWESER_HE_
#define _WEBBROWESER_HE_

/// Include required Header Files 

#include <Awesomium/WebCore.h>
#include <Awesomium/WebConfig.h>
#include <Awesomium/BitmapSurface.h>
#include <Awesomium/STLHelpers.h>
#include <Awesomium/Platform.h>
#include <Awesomium/WebString.h>
#include <Awesomium/WebStringArray.h>
#include <Awesomium/WebViewListener.h>
#include <Awesomium/DataPak.h>
#include <Awesomium/JsArray.h>
#include "WebViewListener.h"
#include "WebLoadListener.h"
#include "WebDownloadListener.h"
#include "method_dispatcher.h"
#include "webLib.h"
#include "OmnisTools.h"



// MethodIds
const qlong 
	// -------Obj Methods ------------	
	ofnavigateToUrl = 1000,
	ofHistoryGoBack = 1001,
	ofHistoryGoForward = 1002,
	ofInitWebView = 1003,
	ofFocus = 1004,
	ofUnFocus = 1005,
	ofShutDownWebView = 1006,
	ofCancelDownload = 1007,
	ofStartDownload = 1008,
	
	ofGetCompData = 1009,
	ofSetCompData = 1010,
	ofSendActionToComp = 1014,

	// -------Obj Event Methods ------------	
	evDoCloseModule = 1100,
	
	evDoShowMessage = 1102,
	
	
	// -------Obj Event der Web View Listener ------------	
	evOnConsoleMessageAdded = 1110,
	evOnDocumentReady = 1111,
	evOnFrameLoadingFailed = 1112,
	evOnTitelChange = 1113,
	evOnAdressBarChanged = 1114,
	evOnOpenNewWindow = 1115,
	evOnDownloadRequest = 1116,
	evOnDownloadUpdate = 1117,
	evOnDownloadFinish = 1118,
	evOnJsInitFailed = 1120,
	evOnCustomCompAction = 1121,
	evOnCompInit = 1122,
	
	

	// -------Static Methods ------------	

	// -------Properties	------------	
	pBasePath	= 4000,
	pUserPath	= 4001
	
;


// define NamesSpace 
namespace WebLib {

	class WebBrowser
	{
		private:
			HWND					mHWnd;
			HWND					mNumberHwnd;

			
			
			// Awsomium Vars
			Awesomium::WebCore*			mWebCore;
			Awesomium::WebSession*		mWebSession;
			Awesomium::WebView*			mWebView;
			
			WebViewListener*			mWebViewListener;
			WebLoadListener*			mWebLoadListener;
			WebDownloadListener*		mWebDownloadListener;
			MethodDispatcher			mMethodDispatcher;
			
			// State Vars
			qbool					mInitCompleted;

			// Omnis Props 
			std::string					mBasePath;
			std::string					mUserPath;
			
			// Window Proc 
			qbool					mWndTimerSet;
			qlong 					mWndTimerId;
			
			// Framebuffer Vars
			unsigned char*			mBitMapBuf;
	
		public:
			
			WebBrowser			(HWND pHWnd, Awesomium::WebSession* pWebSession);
			~WebBrowser			();

			// Standard Methoden 
			qbool		paint();
			qbool		update();
			
			// Omnis 
			qbool		methodCall(EXTCompInfo* pEci); 
			qlong		attributeSupport( LPARAM pMessage, WPARAM wParam, LPARAM lParam, EXTCompInfo* eci );

			// Helper 
			HWND		hwnd()		 { return mHWnd; }								
			HWND		numberHwnd() { return mNumberHwnd; }
			
			Awesomium::WebSession*	getWebSession();

			// Maus & Tstatursteuerung 
			qbool		onMouseAction(LPARAM Msg,WPARAM wParam,LPARAM lParam);
			qbool		onKeyBoardAction(LPARAM Msg,WPARAM wParam,LPARAM lParam);

			// Methoden 
			qlong		initWebView();
			qlong		shutDownWebView();
			qlong		navigateToUrl(std::string url);
			qlong		historyBack();
			qlong		historyForward();
			qlong		focusWebView();
			qlong		unFocusWebView();
			
			std::string	getDataFromComp(std::string compId);
			qlong		setDataForComp(std::string compId,std::string data);
			qlong		sendActionToComp(std::string compId,std::string pType,std::string pParam1,std::string pParam2,std::string pParam3,std::string pParam4,std::string pParam5,std::string pParam6,std::string pParam7,std::string pParam8,std::string pParam9);

			qlong		startDownload(int downloadId,std::string downloadPath);
			qlong		cancelDownload(int downloadId);

			// Inhaltliche Views
			void		setupWebView();

			// Js Callbacks
			void		jsDoShowMsg(Awesomium::WebView* caller, const Awesomium::JSArray& args);
			void		jsDoCloseModule(Awesomium::WebView* caller, const Awesomium::JSArray& args);

			void		jsDoCompCustAction(Awesomium::WebView* caller, const Awesomium::JSArray& args);
			void		jsDoCompInit(Awesomium::WebView* caller, const Awesomium::JSArray& args);
			
			// Methoden Helper Methoden
			static		WebBrowser  *objectFromHwnd(HWND pHwnd, EXTCompInfo *eci);	
			static		void logToTrace(const char* msg);	
	};
	
}  // namespace WebLib

#endif