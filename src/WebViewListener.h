///
/// WebViewListener Header
/// 
/// 25.07.14/fb 
///
///

#ifndef _WEBVIEWLISTNER_HE_
#define _WEBVIEWLISTNER_HE_


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

	class WebViewListener : public Awesomium::WebViewListener::View {
		private:
			// Awsomium Vars
			HWND					mHWnd;
			HWND					mNumberHwnd;
			
		public:
		
			WebViewListener			(HWND pHWnd);
			~WebViewListener			();

			/////////////// Impl Methoden  //////////////////////////
			
			/// This event occurs when the page title has changed.
			void OnChangeTitle(Awesomium::WebView* caller, const Awesomium::WebString& title);

			/// This event occurs when the page URL has changed.
			void OnChangeAddressBar(Awesomium::WebView* caller,const Awesomium::WebURL& url);

			
			/// This event occurs when a message is added to the console on the page.
			/// This is usually the result of a JavaScript error being encountered
			/// on a page.
			void OnAddConsoleMessage(Awesomium::WebView* caller,
										   const Awesomium::WebString& message,
										   int line_number,
										   const Awesomium::WebString& source);
			
			/// This event occurs when a WebView creates a new child WebView
			/// (usually the result of window.open or an external link). It
			/// is your responsibility to display this child WebView in your
			/// application. You should call Resize on the child WebView
			/// immediately after this event to make it match your container
			/// size.
			///
			/// If this is a child of a Windowed WebView, you should call
			/// WebView::set_parent_window on the new view immediately within
			/// this event.
			///
			void OnShowCreatedWebView(Awesomium::WebView* caller,
											Awesomium::WebView* new_view,
											const Awesomium::WebURL& opener_url,
											const Awesomium::WebURL& target_url,
											const Awesomium::Rect& initial_pos,
											bool is_popup);


			/////////////// Leere Methoden ohne Inhalt //////////////////////////

			/// This event occurs when the tooltip text has changed. You
			/// should hide the tooltip when the text is empty.
			void OnChangeTooltip(Awesomium::WebView* caller,
									   const Awesomium::WebString& tooltip){};

			/// This event occurs when the target URL has changed. This
			/// is usually the result of hovering over a link on a page.
			void OnChangeTargetURL(Awesomium::WebView* caller,
										 const Awesomium::WebURL& url) {};

			/// This event occurs when the cursor has changed. This is
			/// is usually the result of hovering over different content.
			void OnChangeCursor(Awesomium::WebView* caller,
									  Awesomium::Cursor cursor) {};

			/// This event occurs when the focused element changes on the page.
			/// This is usually the result of textbox being focused or some other
			/// user-interaction event.
			void OnChangeFocus(Awesomium::WebView* caller,
									 Awesomium::FocusedElementType focused_type) {};

	};
}  // namespace WebLib
#endif