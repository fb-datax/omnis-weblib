///
/// WebBrowser Impl
/// 
/// 25.07.14/fb 
///
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
#include "WebBrowser.h"


void* pt2Object;

using namespace WebLib;


/*
	Constructor
*/
WebBrowser::WebBrowser(HWND pHWnd, Awesomium::WebSession* pWebSession)
{
	mHWnd = pHWnd;	// Ref auf das Fenster via hwnd
	mWebSession = pWebSession;
	
	// Alle Werte des Objektes inialisieren 
	mWebCore = 0;
	mWebView = 0;
	mWebViewListener = 0;
	mWebLoadListener = 0;
	mWebDownloadListener = 0;
	mInitCompleted = false;
	mBasePath = "";
	mUserPath = "";
	
	// Timer Proc Releated
	mWndTimerSet = qfalse;
	mWndTimerId = WND_TIMER_FIRST;

}

/*
	Destructor
*/
WebBrowser::~WebBrowser()
{	

	OmnisTools::logToTrace("destruct()");

	// Cleanup
	if (mWndTimerSet){
		WNDkillTimer(mHWnd,mWndTimerSet);
	}
	
	if (mWebView){
		mWebView->Destroy();
		mWebView = 0;
	}
	shutDownWebView();
}


//////////////////////// Painting //////////////////////////////

// You need to paint your control
qbool WebBrowser::paint()
{
	WNDpaintStruct ps;
	WNDbeginPaint(mHWnd, &ps);
    
	if ( ECOisDesign(mHWnd) )
	{
		ECOdrawNumber(mHWnd, ps.hdc);
		ECOdrawMultiKnobs(mHWnd, ps.hdc);
	} else {
		if (mWebView) {
			qrect client;
			WNDgetClientRect( mHWnd, &client );
			mWebView->Resize(client.width(), client.height());
		}
	}
	WNDendPaint(mHWnd, &ps);
	return qtrue;
}

// You need to paint your control
qbool WebBrowser::update()
{	
	if (mWebCore){
		mWebCore->Update();
	}
	return qtrue;
}


//////////////////////// Methods //////////////////////////////

Awesomium::WebSession* WebBrowser::getWebSession(){
	return mWebSession;
}

qlong WebBrowser::initWebView()
{
	OmnisTools::logToTrace("initWebView()");
	
	// Prüfen ob der WebCore bereits initialisiert wurde
	mWebCore = Awesomium::WebCore::instance();
	if (!mWebCore){
		OmnisTools::logToTrace("Webcore intializing");
				
		// Setup path Vars
		Awesomium::WebString _logfile		=	Awesomium::ToWebString(mUserPath);
		Awesomium::WebString _pluginPath	=	Awesomium::ToWebString(mBasePath);
		Awesomium::WebString _sysPath		=	Awesomium::ToWebString(mBasePath);
			
		_logfile.Append(Awesomium::WSLit("\\debug.log"));
		_pluginPath.Append(Awesomium::WSLit("plugins\\"));
		_sysPath.Append(Awesomium::WSLit("system\\"));
			
		// Setup config 
		Awesomium::WebConfig config;
		config.log_path = _logfile;
		config.plugin_path = _pluginPath;	
		config.package_path = _sysPath;

		// log config 
		config.log_level = Awesomium::kLogLevel_Normal;		
		#if defined(IS_DEBUG)
			config.log_level = Awesomium::kLogLevel_Verbose;
			config.remote_debugging_port = 1337;
		#endif
		Awesomium::WebCore::Initialize(config);
		mWebCore = Awesomium::WebCore::instance();
		
	} 



	// Setup Buffer, WebView and Timer Proc 
	if ( !ECOisDesign(mHWnd) ){
		
		qrect client;
		WNDgetClientRect( mHWnd, &client );
		
		

		// init Web View 
		if (!mWebSession){
			// prefs
			Awesomium::WebPreferences prefs;
			prefs.enable_plugins = false;
			prefs.enable_smooth_scrolling = true;
			prefs.enable_app_cache = true;
			prefs.enable_databases = true;
			prefs.enable_dart = true;
			prefs.enable_web_security = false;

			prefs.enable_gpu_acceleration = true;
			prefs.enable_web_gl = true;
			
			prefs.allow_file_access_from_file_url = true;
			prefs.allow_universal_access_from_file_url = true;
			prefs.allow_running_insecure_content = true;
			prefs.allow_scripts_to_access_clipboard = true;
			prefs.allow_scripts_to_close_windows = true;
			prefs.allow_scripts_to_open_windows = true;

			prefs.accept_language = Awesomium::WSLit("de-DE,de;q=0.8,en-US;q=0.6,en;q=0.4");
			prefs.default_encoding = Awesomium::WSLit("UTF-8");

			Awesomium::WebString _sessionPath = Awesomium::ToWebString(mUserPath);
			_sessionPath.Append(Awesomium::WSLit(""));
			
			if (mUserPath.length() > 0) {
				// Persitent
				OmnisTools::logToTrace("persitent-session");
				mWebSession = mWebCore->CreateWebSession(_sessionPath, prefs);
			}else {
				// In memory
				OmnisTools::logToTrace("in-memory-session");
				mWebSession = mWebCore->CreateWebSession(Awesomium::WSLit(""), prefs);
			}

			Awesomium::WebString _pakPath = Awesomium::ToWebString(mBasePath);
			_pakPath.Append(Awesomium::WSLit("paks\\weblib.pak"));

			Awesomium::DataSource* data_source = new Awesomium::DataPakSource(_pakPath);
			mWebSession->AddDataSource(Awesomium::WSLit("weblib"), data_source);
		}
	}

	// Setup Buffer, WebView and Timer Proc 
	if ( !ECOisDesign(mHWnd) ){
		
		qrect client;
		WNDgetClientRect( mHWnd, &client );

		// init Web View 
		if (!mWebView){
			qrect client;
			WNDgetClientRect( mHWnd, &client );
			mWebView = mWebCore -> CreateWebView(client.width(), client.height(), mWebSession , Awesomium::kWebViewType_Window);
			setupWebView();
		}

		if (!mWndTimerSet) {
			mWndTimerSet = qtrue;
			WNDsetTimer( mHWnd, mWndTimerId, 5);
		}
	}
	return 0;
}

void WebBrowser::setupWebView(){

	if (!mWebViewListener){
		mWebViewListener = new WebViewListener(mHWnd);
	}
	if (!mWebLoadListener){
		mWebLoadListener = new WebLoadListener(mHWnd);
	}
	if (!mWebDownloadListener){
		mWebDownloadListener = new WebDownloadListener(mHWnd);
	}
	
	// Anzahl verdoppelt
	mWebView -> set_sync_message_timeout(1600);
	mWebView -> set_parent_window(mHWnd);
	mWebView -> set_view_listener(mWebViewListener);
	mWebView -> set_load_listener(mWebLoadListener);
	mWebView -> set_download_listener(mWebDownloadListener);
	mWebView -> set_js_method_handler(&mMethodDispatcher);

	// Setup Base dorgv4 obj 
	Awesomium::JSValue result = mWebView->CreateGlobalJavascriptObject(Awesomium::WSLit("weblib"));
	if (result.IsObject()){
		Awesomium::JSObject& jsObj = result.ToObject();
		mMethodDispatcher.Bind(jsObj,Awesomium::WSLit("doCloseModule"),JSDelegate(this, &WebBrowser::jsDoCloseModule));
		mMethodDispatcher.Bind(jsObj,Awesomium::WSLit("doShowMsg"),JSDelegate(this, &WebBrowser::jsDoShowMsg));
		mMethodDispatcher.Bind(jsObj,Awesomium::WSLit("doCompCustAction"),JSDelegate(this, &WebBrowser::jsDoCompCustAction));

		mMethodDispatcher.Bind(jsObj,Awesomium::WSLit("doCompInit"),JSDelegate(this, &WebBrowser::jsDoCompInit));
		
		jsObj.SetProperty(Awesomium::WSLit("basePath"), Awesomium::ToWebString(mBasePath));
		jsObj.SetProperty(Awesomium::WSLit("userPath"), Awesomium::ToWebString(mUserPath));
		
	} else {
		// Event evOnJsInitFailed occurred. Send event to OMNIS 
		EXTCompInfo* eci = new EXTCompInfo(); 
		int test = 55;
		eci->mParamFirst = 0; 
		// ECI mit Parametern befüllen 
		EXTfldval _errorCode;
		OmnisTools::getEXTFldValFromInt(_errorCode,test);
		ECOaddParam(eci,&_errorCode,0,0,0,1,0);  

		qbool eventOk = ECOsendCompEvent( mHWnd, eci, evOnJsInitFailed, qtrue ); 

		// Delete parameters from EXTCompInfo structure 
		ECOmemoryDeletion( eci ); 

		// Delete eci structure 
		delete eci; 
	}
}
//////////////////////// Js Callbacks //////////////////////////////

void WebBrowser::jsDoCloseModule(Awesomium::WebView* caller, const Awesomium::JSArray& args) {
	ECOsendEvent( mHWnd, evDoCloseModule );
}


void WebBrowser::jsDoCompInit(Awesomium::WebView* caller, const Awesomium::JSArray& args) {
	// Event myEvent1 occurred. Send event to OMNIS 
	EXTCompInfo* eci = new EXTCompInfo(); 
	eci->mParamFirst = 0; 
	Awesomium::WebString webstring;
	
	if (args.size() >= 1) {
		EXTfldval _type;
		webstring = args[0].ToString();
		OmnisTools::getEXTFldValFromString(_type,OmnisTools::getStringFromWebString(webstring));
		ECOaddParam(eci,&_type,0,0,0,1,0);
	}

	ECOsendEvent( mHWnd, evOnCompInit );
}

void WebBrowser::jsDoShowMsg(Awesomium::WebView* caller, const Awesomium::JSArray& args) {

	// Event myEvent1 occurred. Send event to OMNIS 
	EXTCompInfo* eci = new EXTCompInfo(); 
	eci->mParamFirst = 0; 
	Awesomium::WebString webstring;
	
	if (args.size() >= 1) {
		EXTfldval _type;
		webstring = args[0].ToString();
		OmnisTools::getEXTFldValFromString(_type,OmnisTools::getStringFromWebString(webstring));
		ECOaddParam(eci,&_type,0,0,0,1,0);
	}

	if (args.size() >= 2) {
		EXTfldval _param1;
		webstring = args[1].ToString();
		OmnisTools::getEXTFldValFromString(_param1,OmnisTools::getStringFromWebString(webstring));
		ECOaddParam(eci,&_param1,0,0,0,2,0);
	}

	if (args.size() >= 3) {
		EXTfldval _param2;
		webstring = args[2].ToString();
		OmnisTools::getEXTFldValFromString(_param2,OmnisTools::getStringFromWebString(webstring));
		ECOaddParam(eci,&_param2,0,0,0,3,0);
	}

	if (args.size() >= 4) {
		EXTfldval _param3;
		webstring = args[3].ToString();
		OmnisTools::getEXTFldValFromString(_param3,OmnisTools::getStringFromWebString(webstring));
		ECOaddParam(eci,&_param3,0,0,0,4,0);
	}
	
	// Send event to OMNIS 
	qbool eventOk = ECOsendCompEvent( mHWnd, eci, evDoShowMessage, qtrue ); 

	// Delete parameters from EXTCompInfo structure 
	ECOmemoryDeletion( eci ); 

	// Delete eci structure 
	delete eci; 

}


void WebBrowser::jsDoCompCustAction(Awesomium::WebView* caller, const Awesomium::JSArray& args) {
	
	// Event myEvent1 occurred. Send event to OMNIS 
	EXTCompInfo* eci = new EXTCompInfo(); 
	eci->mParamFirst = 0; 
	Awesomium::WebString webstring;
	
	if (args.size() >= 1) {
		EXTfldval _type;
		webstring = args[0].ToString();
		OmnisTools::getEXTFldValFromString(_type,OmnisTools::getStringFromWebString(webstring));
		ECOaddParam(eci,&_type,0,0,0,1,0);
	}

	if (args.size() >= 2) {
		EXTfldval _param1;
		webstring = args[1].ToString();
		OmnisTools::getEXTFldValFromString(_param1,OmnisTools::getStringFromWebString(webstring));
		ECOaddParam(eci,&_param1,0,0,0,2,0);
	}

	if (args.size() >= 3) {
		EXTfldval _param2;
		webstring = args[2].ToString();
		OmnisTools::getEXTFldValFromString(_param2,OmnisTools::getStringFromWebString(webstring));
		ECOaddParam(eci,&_param2,0,0,0,3,0);
	}

	if (args.size() >= 4) {
		EXTfldval _param3;
		webstring = args[3].ToString();
		OmnisTools::getEXTFldValFromString(_param3,OmnisTools::getStringFromWebString(webstring));
		ECOaddParam(eci,&_param3,0,0,0,4,0);
	}

	if (args.size() >= 5) {
		EXTfldval _param4;
		webstring = args[4].ToString();
		OmnisTools::getEXTFldValFromString(_param4,OmnisTools::getStringFromWebString(webstring));
		ECOaddParam(eci,&_param4,0,0,0,5,0);
	}

	if (args.size() >= 6) {
		EXTfldval _param5;
		webstring = args[5].ToString();
		OmnisTools::getEXTFldValFromString(_param5,OmnisTools::getStringFromWebString(webstring));
		ECOaddParam(eci,&_param5,0,0,0,6,0);
	}

	if (args.size() >= 7) {
		EXTfldval _param6;
		webstring = args[6].ToString();
		OmnisTools::getEXTFldValFromString(_param6,OmnisTools::getStringFromWebString(webstring));
		ECOaddParam(eci,&_param6,0,0,0,7,0);
	}

	if (args.size() >= 8) {
		EXTfldval _param7;
		webstring = args[7].ToString();
		OmnisTools::getEXTFldValFromString(_param7,OmnisTools::getStringFromWebString(webstring));
		ECOaddParam(eci,&_param7,0,0,0,8,0);
	}
	if (args.size() >= 9) {
		EXTfldval _param8;
		webstring = args[8].ToString();
		OmnisTools::getEXTFldValFromString(_param8,OmnisTools::getStringFromWebString(webstring));
		ECOaddParam(eci,&_param8,0,0,0,9,0);
	}
	if (args.size() >= 10) {
		EXTfldval _param9;
		webstring = args[9].ToString();
		OmnisTools::getEXTFldValFromString(_param9,OmnisTools::getStringFromWebString(webstring));
		ECOaddParam(eci,&_param9,0,0,0,10,0);
	}
	
	// Send event to OMNIS 
	qbool eventOk = ECOsendCompEvent( mHWnd, eci, evOnCustomCompAction, qtrue ); 

	// Delete parameters from EXTCompInfo structure 
	ECOmemoryDeletion( eci ); 

	// Delete eci structure 
	delete eci; 
}


//////////////////////// Js Invokes //////////////////////////////
qlong WebBrowser::setDataForComp(std::string compId,std::string data) {
	int result = 0;
	
	if (mWebView) {
		Awesomium::WebString webStrCompId = OmnisTools::getWebStringFromStr(compId);
		Awesomium::WebString webStrCompData = OmnisTools::getWebStringFromStr(data);

		Awesomium::WebString webStrCompPath	=	Awesomium::WSLit("window.weblibcomps.");
		webStrCompPath.Append(webStrCompId);

		// Retrieve the global 'window' object from the page
		Awesomium::JSValue window = mWebView->ExecuteJavascriptWithResult(webStrCompPath, Awesomium::WSLit(""));
		if (window.IsObject()) {
		  Awesomium::JSArray args;
		  args.Push(Awesomium::JSValue(webStrCompData));
		  window.ToObject().Invoke(Awesomium::WSLit("setData"), args);
		} else {
			result =  -1;
		}
	}
	return result;
}

qlong WebBrowser::sendActionToComp(std::string compId,std::string pType,std::string pParam1,std::string pParam2,std::string pParam3,std::string pParam4,std::string pParam5,std::string pParam6,std::string pParam7,std::string pParam8,std::string pParam9)
{
	int result = 0;
	
	if (mWebView) {
		Awesomium::WebString webStrCompId = OmnisTools::getWebStringFromStr(compId);
		Awesomium::WebString webStrCompPath	=	Awesomium::WSLit("window.weblibcomps.");
		webStrCompPath.Append(webStrCompId);

		// Retrieve the global 'window' object from the page
		Awesomium::JSValue window = mWebView->ExecuteJavascriptWithResult(webStrCompPath, Awesomium::WSLit(""));
		if (window.IsObject()) {
		  Awesomium::JSArray args;
		  args.Push(Awesomium::JSValue(OmnisTools::getWebStringFromStr(pType)));
		  args.Push(Awesomium::JSValue(OmnisTools::getWebStringFromStr(pParam1)));
		  args.Push(Awesomium::JSValue(OmnisTools::getWebStringFromStr(pParam2)));
		  args.Push(Awesomium::JSValue(OmnisTools::getWebStringFromStr(pParam3)));
		  args.Push(Awesomium::JSValue(OmnisTools::getWebStringFromStr(pParam4)));
		  args.Push(Awesomium::JSValue(OmnisTools::getWebStringFromStr(pParam5)));
		  args.Push(Awesomium::JSValue(OmnisTools::getWebStringFromStr(pParam6)));
		  args.Push(Awesomium::JSValue(OmnisTools::getWebStringFromStr(pParam7)));
		  args.Push(Awesomium::JSValue(OmnisTools::getWebStringFromStr(pParam8)));
		  args.Push(Awesomium::JSValue(OmnisTools::getWebStringFromStr(pParam9)));

		  window.ToObject().Invoke(Awesomium::WSLit("doCustomAction"), args);
		} else {
			result =  -1;
		}
	}
	return result;
}


std::string WebBrowser::getDataFromComp(std::string compId) {
	std::string result = "";
	if (mWebView) {
		// Retrieve the global 'window' object from the page
		Awesomium::WebString webStrCompId = OmnisTools::getWebStringFromStr(compId);
		Awesomium::WebString webStrCompPath	=	Awesomium::WSLit("window.weblibcomps.");
		webStrCompPath.Append(webStrCompId);

		Awesomium::JSValue window = mWebView->ExecuteJavascriptWithResult(webStrCompPath, Awesomium::WSLit(""));
		if (window.IsObject()) {
		  Awesomium::JSArray args;
		  args.Push(Awesomium::JSValue(OmnisTools::getWebStringFromStr(compId)));
		  Awesomium::JSValue resultValue = window.ToObject().Invoke(Awesomium::WSLit("getData"), args);
		  if (resultValue.IsString()) {
			  result =  OmnisTools::getStringFromWebString(resultValue.ToString());
		  }
		} else {
			result =  "#ERROR";
		}
	}
	return result;
}


//////////////////////// Omnis Callbacks //////////////////////////////

qlong WebBrowser::cancelDownload(int downloadId){
	int result = 0;
	if (mWebView) {
		mWebView->DidCancelDownload(downloadId);
	}
	return result;
}
qlong WebBrowser::startDownload(int downloadId,std::string downloadPath){
	int result = 0;
	
	if (mWebView) {
		Awesomium::WebString _path = OmnisTools::getWebStringFromStr(downloadPath);
		mWebView->DidChooseDownloadPath(downloadId,_path);
	}
	return result;
}

qlong WebBrowser::shutDownWebView() {
	int result = 0;
	
	OmnisTools::logToTrace("shutDown WebView()");
	
	if (mWebView) {
		mWebView->Stop();
	}
	if (mWebView){
		HWND mainWindow = (HWND) mWebView->window();
		WNDdestroyWindow( mainWindow );
	}
	
	if (mWebViewListener) {
		delete mWebViewListener;
		mWebViewListener = 0;
	}

	if (mWebLoadListener) {
		delete mWebLoadListener;
		mWebLoadListener = 0;
	}

	return result;
}

qlong WebBrowser::navigateToUrl(std::string url) {

	int result = 0;
	if (mWebView) {
		Awesomium::WebString _url = OmnisTools::getWebStringFromStr(url);
		Awesomium::WebURL webUrl = Awesomium::WebURL(_url);
		mWebView->LoadURL(webUrl);
		mWebView->Focus();
	}
	return result;
}


qlong WebBrowser::historyBack() {
	int result = 0;
	
	if (mWebView){
		mWebView->GoBack();
	}
	return result;
}

qlong WebBrowser::historyForward() {
	int result = 0;
	
	if (mWebView) {
		mWebView->GoForward();
	}
	return result;
}

qlong WebBrowser::focusWebView() {
	int result = 0;
	
	if (mWebView){
		mWebView->Focus();
	}
	return result;
}

qlong WebBrowser::unFocusWebView() {
	int result = 0;
	
	if (mWebView){
		mWebView->Unfocus();
	}
	return result;
}



//////////////////////// OMNIS Integration //////////////////////////////

qlong WebBrowser::attributeSupport( LPARAM pMessage, WPARAM wParam, LPARAM lParam, EXTCompInfo* eci )
{
	switch( pMessage )
	{ 
		case ECM_PROPERTYCANASSIGN: 
		{
			return 1L;		
		}
		case ECM_SETPROPERTY: 
		{
			// get the externals parameter
			EXTParamInfo* param = ECOfindParamNum( eci, 1 );
			if ( param )
			{                             
				// get the fldvpPassal from the parameter.
				EXTfldval fval( (qfldval)param->mData );
				// fldvals are containers that store data. All values in and out of OMNIS are passes in fldvals.
				// switch on property id
				switch( ECOgetId(eci) )
				{  
					case pBasePath:
					{
						mBasePath = OmnisTools::getStringFromEXTFldVal(fval);						                                
						break;
					}  
					case pUserPath:
					{
						mUserPath = OmnisTools::getStringFromEXTFldVal(fval);						                                
						break;
					}  
				}   
				return 1L;
			}
			break;
		}
		case ECM_GETPROPERTY: 
		{
			EXTfldval fval;
			// switch on property id
			switch( ECOgetId(eci) )
			{    
				// we are setting our color property
				case pBasePath:
				{ 
					OmnisTools::getEXTFldValFromString(fval, mBasePath);
					break;
				}   
				case pUserPath:
				{
					OmnisTools::getEXTFldValFromString(fval, mUserPath);
					break;
				} 
			}
			// to send it back, we add a parameter which OMNIS will check on return.
			ECOaddParam(eci,&fval);
			return 1L;
		}    
		
	}
	return 0L;		// no property found or message was wrong
}

qbool WebBrowser::methodCall(EXTCompInfo* pEci)
{
	// switch on function id
	EXTfldval rtnVal; 
	qbool rtnCode		=	qfalse;
	qbool hasRtnVal		=	qfalse;
	qlong funcId		=	ECOgetId(pEci);
	
	
	switch ( funcId )
	{

		case ofInitWebView:
		{
			rtnCode = qtrue;
			hasRtnVal = qtrue;
			rtnVal.setLong(WebBrowser::initWebView());
			break;
		}

		case ofShutDownWebView:
		{
			rtnCode = qtrue;
			hasRtnVal = qtrue;
			rtnVal.setLong(WebBrowser::shutDownWebView());
			break;
		}

		case ofnavigateToUrl:
		{
			EXTParamInfo* paramInfo = ECOfindParamNum(pEci,1);
			if (paramInfo) {
				rtnCode = qtrue;
				hasRtnVal = qtrue;	
				EXTfldval fval( (qfldval)paramInfo->mData);
				std::string url = OmnisTools::getStringFromEXTFldVal(fval);
				rtnVal.setLong(WebBrowser::navigateToUrl(url));
			}
			break;
		}
		
		case ofCancelDownload:
		{
			EXTParamInfo* paramInfo = ECOfindParamNum(pEci,1);
			if (paramInfo) {
				rtnCode = qtrue;
				hasRtnVal = qtrue;	
				EXTfldval fval( (qfldval)paramInfo->mData);
				int downloadId = OmnisTools::getIntFromEXTFldVal(fval);
				rtnVal.setLong(WebBrowser::cancelDownload(downloadId));
			}
			break;
		}

		case ofStartDownload:
		{
			EXTParamInfo* pDownloadId = ECOfindParamNum(pEci,1);
			EXTParamInfo* pPathParam = ECOfindParamNum(pEci,2);
			
			rtnCode = qtrue;
			hasRtnVal = qtrue;	
			
			EXTfldval fvalDownloadId((qfldval)pDownloadId->mData);
			int downloadId = OmnisTools::getIntFromEXTFldVal(fvalDownloadId);

			EXTfldval fvalPath((qfldval)pPathParam->mData);
			std::string path = OmnisTools::getStringFromEXTFldVal(fvalPath);


			rtnVal.setLong(WebBrowser::startDownload(downloadId,path));
			
			break;
		}

		case ofHistoryGoBack:
		{
			rtnCode = qtrue;
			hasRtnVal = qtrue;
			rtnVal.setLong(WebBrowser::historyBack());
			break;
		}

		case ofHistoryGoForward:
		{
			rtnCode = qtrue;
			hasRtnVal = qtrue;
			rtnVal.setLong(WebBrowser::historyForward());
			break;
		}
		case ofFocus:
		{
			rtnCode = qtrue;
			hasRtnVal = qtrue;
			rtnVal.setLong(WebBrowser::focusWebView());
			break;
		}
		case ofUnFocus:
		{
			rtnCode = qtrue;
			hasRtnVal = qtrue;
			rtnVal.setLong(WebBrowser::unFocusWebView());
			break;
		}
		case ofGetCompData:
		{
			EXTParamInfo* paramInfo = ECOfindParamNum(pEci,1);
			if (paramInfo) {
				rtnCode = qtrue;
				hasRtnVal = qtrue;	
				
				EXTfldval fval( (qfldval)paramInfo->mData);
				std::string url = OmnisTools::getStringFromEXTFldVal(fval);
				std::string result = WebBrowser::getDataFromComp(url);
				OmnisTools::getEXTFldValFromString(rtnVal,result);
			}
			break;
		}

		case ofSetCompData:
		{
			EXTParamInfo* pCompId = ECOfindParamNum(pEci,1);
			EXTParamInfo* pData = ECOfindParamNum(pEci,2);
			if (pCompId) {
				EXTfldval fvalComp((qfldval)pCompId->mData);
				EXTfldval fvalData((qfldval)pData->mData);
				std::string comp = OmnisTools::getStringFromEXTFldVal(fvalComp);
				std::string data = OmnisTools::getStringFromEXTFldVal(fvalData);
				rtnVal.setLong(WebBrowser::setDataForComp(comp,data));
				rtnCode = qtrue;
				hasRtnVal = qtrue;	
			}
			break;
		}
		case ofSendActionToComp:
		{


			EXTParamInfo* pCompId = ECOfindParamNum(pEci,1);
			if (pCompId) {
				EXTParamInfo* pCompId = ECOfindParamNum(pEci,1);
				EXTfldval fvalCompId ((qfldval)pCompId->mData);
				std::string compId = OmnisTools::getStringFromEXTFldVal(fvalCompId);

				EXTParamInfo* pType = ECOfindParamNum(pEci,2);
				EXTfldval fvalType ((qfldval)pType->mData);
				std::string type = OmnisTools::getStringFromEXTFldVal(fvalType);

				EXTParamInfo* pParam1 = ECOfindParamNum(pEci,3);
				EXTfldval fvalParam1 ((qfldval)pParam1->mData);
				std::string param1 = OmnisTools::getStringFromEXTFldVal(fvalParam1);

				EXTParamInfo* pParam2 = ECOfindParamNum(pEci,4);
				EXTfldval fvalParam2 ((qfldval)pParam2->mData);
				std::string param2 = OmnisTools::getStringFromEXTFldVal(fvalParam2);

				EXTParamInfo* pParam3 = ECOfindParamNum(pEci,5);
				EXTfldval fvalParam3 ((qfldval)pParam3->mData);
				std::string param3 = OmnisTools::getStringFromEXTFldVal(fvalParam3);

				EXTParamInfo* pParam4 = ECOfindParamNum(pEci,6);
				EXTfldval fvalParam4 ((qfldval)pParam4->mData);
				std::string param4 = OmnisTools::getStringFromEXTFldVal(fvalParam4);

				EXTParamInfo* pParam5 = ECOfindParamNum(pEci,7);
				EXTfldval fvalParam5 ((qfldval)pParam5->mData);
				std::string param5 = OmnisTools::getStringFromEXTFldVal(fvalParam5);

				EXTParamInfo* pParam6 = ECOfindParamNum(pEci,8);
				EXTfldval fvalParam6 ((qfldval)pParam6->mData);
				std::string param6 = OmnisTools::getStringFromEXTFldVal(fvalParam6);

				EXTParamInfo* pParam7 = ECOfindParamNum(pEci,9);
				EXTfldval fvalParam7 ((qfldval)pParam7->mData);
				std::string param7 = OmnisTools::getStringFromEXTFldVal(fvalParam7);

				EXTParamInfo* pParam8 = ECOfindParamNum(pEci,10);
				EXTfldval fvalParam8 ((qfldval)pParam8->mData);
				std::string param8 = OmnisTools::getStringFromEXTFldVal(fvalParam8);

				EXTParamInfo* pParam9 = ECOfindParamNum(pEci,11);
				EXTfldval fvalParam9 ((qfldval)pParam9->mData);
				std::string param9 = OmnisTools::getStringFromEXTFldVal(fvalParam9);
				
				rtnVal.setLong(WebBrowser::sendActionToComp(compId,type,param1,param2,param3,param4,param5,param6,param7,param8,param9));
				rtnCode = qtrue;
				hasRtnVal = qtrue;	
			}
			break;
		}
	}


	if ( hasRtnVal ){
		ECOaddParam(pEci,&rtnVal);
	}

	return rtnCode;
}

//////////////////////// Static Methods  //////////////////////////////


//////////////////////// Static Helper  //////////////////////////////

WebBrowser *WebBrowser::objectFromHwnd(HWND pHwnd, EXTCompInfo *eci)
{
	WebBrowser *object = (WebBrowser *) ECOfindObject(eci, pHwnd);
	// Look for number hwnd, if hwnd does not locate the object
	if (!object)
	{
		HWND parentHwnd = WNDgetParent(pHwnd);
		if (parentHwnd) object = (WebBrowser *) ECOfindObject(eci, parentHwnd);
	}
	return object;
}
