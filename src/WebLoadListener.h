///
/// WebLoadListener Header
/// 
/// 25.07.14/fb 
///
///

#ifndef _WEBLOADLISTNER_HE_
#define _WEBLOADLISTNER_HE_


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
#include "webLib.h"


// define NamesSpace 
namespace WebLib {

	class WebLoadListener : public Awesomium::WebViewListener::Load {
		private:
			// Awsomium Vars
			HWND					mHWnd;
			HWND					mNumberHwnd;
	
		public:
		
			WebLoadListener			(HWND pHWnd);
			~WebLoadListener			();

			/////////////// Impl Methoden  //////////////////////////

			/// This event occurs when the DOM has finished parsing and the
			/// window object is available for JavaScript execution.
			virtual void OnDocumentReady(Awesomium::WebView* caller,
									   const Awesomium::WebURL& url);

			/// This event occurs when a frame fails to load. See error_desc
			/// for additional information.
			virtual void OnFailLoadingFrame(Awesomium::WebView* caller,
										  int64 frame_id,
										  bool is_main_frame,
										  const Awesomium::WebURL& url,
										  int error_code,
										  const Awesomium::WebString& error_desc);


			/////////////// Leere Methoden ohne Inhalt //////////////////////////

			/// This event occurs when the page begins loading a frame.
			virtual void OnBeginLoadingFrame(Awesomium::WebView* caller,
										   int64 frame_id,
										   bool is_main_frame,
										   const Awesomium::WebURL& url,
										   bool is_error_page) {};

			
			/// This event occurs when the page finishes loading a frame.
			/// The main frame always finishes loading last for a given page load.
			virtual void OnFinishLoadingFrame(Awesomium::WebView* caller,
											int64 frame_id,
											bool is_main_frame,
											const Awesomium::WebURL& url){};

			

			
	};
}  // namespace WebLib
#endif