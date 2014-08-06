///
/// WebDownloadListener Header
/// 
/// 25.07.14/fb 
///
///

#ifndef _WEBDOWNLISTNER_HE_
#define _WEBDOWNLISTNER_HE_


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

	class WebDownloadListener : public Awesomium::WebViewListener::Download {
		private:
			// Awsomium Vars
			HWND					mHWnd;
			HWND					mNumberHwnd;

		public:
		
			WebDownloadListener			(HWND pHWnd);
			~WebDownloadListener			();

			/////////////// Impl Methoden  //////////////////////////
			virtual void OnRequestDownload(Awesomium::WebView* caller,
										 int download_id,
										 const Awesomium::WebURL& url,
										 const Awesomium::WebString& suggested_filename,
										 const Awesomium::WebString& mime_type);

			virtual void OnUpdateDownload(Awesomium::WebView* caller,
										int download_id,
										int64 total_bytes,
										int64 received_bytes,
										int64 current_speed);

			virtual void OnFinishDownload(Awesomium::WebView* caller,
										int download_id,
										const Awesomium::WebURL& url,
										const Awesomium::WebString& saved_path);

	};
}  // namespace WebLib
#endif