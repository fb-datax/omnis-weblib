Awesomium running inside O$ 4.3.X
=======================

#### Introduction

The following project is a Tech-Demo demonstrating the usage of [Awesomium](http://awesomium.com/) as an Omnis Studio External to display HTML5 content and to invoke Omnis Studio code from JavaScript and vice versa.

Awesomium is a commercial product for commercial apps, which exposes Chromium through an easy to use C++ API.

As it is currently unclear if the source/header/bin files from Omnis Studio and/or Awesomium can freely be distributed, we removed them from the initial release.

Any feedback regarding mistakes, possible errors and improvements is welcome.

To experience the Tech-Demo download the repo and follow the instructions.

#### Requirements

+ Win 7
+ O$ 4.3.X Non-Unicode
+ Omnis Externals SDK for O$ 4.3.1
+ Awesomium 1.7.X SDK with C++ examples
+ Visual Studio C++ 2008 (Express)

#### Installation

+ Install Awesomium and copy the following files into the specified project directory:
    * AWESOMIUM\build\bin to deploy\bin\ext\web\browser\system
    * AWESOMIUM\build\bin\awesomium.dll to deploy\bin\xcomp
    * AWESOMIUM\build\lib\awesomium.lib to include\lib
    * AWESOMIUM\include\Awesomium to include\awesomium
    * C:\Users\Public\Documents\Awesomium SDK Samples\1.7.4.X\Awesomium\BuildSamples\common\js_delegate.h to src\js_delegate.h#
    * C:\Users\Public\Documents\Awesomium SDK Samples\1.7.4.X\Awesomium\BuildSamples\common\method_dispatcher.h to src\method_dispatcher.h
    * C:\Users\Public\Documents\Awesomium SDK Samples\1.7.4.X\Awesomium\BuildSamples\common\method_dispatcher.cpp to src\method_dispatcher.cpp

+ Install the Omnis External SDK and copy the following files into the specified project directory:
    * OMNISDK\LIBS\omnis.lib to include\lib
    * OMNISDK\COMPLIB to include\omnis

+ Copy the contents of deploy\bin\xcomp into OMNIS431INSTALLDIR\xcomp
+ Modify the files resources\testdebug.bat and testtrelease.bat to match your local installation
+ Start Visual Studio 2008 C++ and open the proj\webLib.vcproj file

#### Building the external

+ Build the C++ Project inside Visual Studio and make sure no errors occur during the build process
+ After a successful build run the C++ program in Debug Mode
+ When asked for an executable, select your omnis.exe and the path to you're lbs\demo.lbs from this project as an commandline parameter
+ When the lbs\demo.lbs is launched for the first time a modification inside the StartupTask is necessary. The variable iBasePath has to match the path of the root folder of this project

#### Samples

The demo.lbs contains the following examples:

+ Websites

    + html5test.com
    + google.com
    + media.tojicode.com/q3bsp/ (WebGL Demo)

![Websites](resources/screenshot-1.png?raw=true "Websites")

+ Event Demo
    * Demonstrates how to read properties in JavaScript from Omnis Studio and how to use these inside JavaScript
    * Shows how to send events from JavaScript to Omnis Studio (show Omnis OK message, Close current window from JavaScript)

![Event](resources/screenshot-2.png?raw=true "Event")

+ Ace9 Code Editor inside Omnis Studio
    * Demonstrates how to wrap a HTML component and how a two-way interaction is possible (setData & getData)

![CodeEditor](resources/screenshot-3.png?raw=true "CodeEditor")

+ Download 
	* Download of a pdf File with a callback in Omnis Studio to return the path, where the file should be saved to.

#### Notes

+ Lacks documentation
+ Ugly C++ char conversion
+ Windows only
+ Every instance of the component creates its own process, which could be changed so that they every WebView is a child of one master WebView, which would save cpu & memory
+ It should be possible to us the JsClient inside Omnis and communicate between Omnis Fat-Client and Omnis JsClient

#### Thanks

+ Thanks to David McKeone for the NVObjTemplate (https://github.com/dmckeone/NVObjTemplate) he published on github

#### License

LGPL

(C) Copyright 2014 DATAX-AG (http://datax-ag.de/) and others.

All rights reserved. This program and the accompanying materials
are made available under the terms of the GNU Lesser General Public License
(LGPL) version 2.1 which accompanies this distribution, and is available at
http://www.gnu.org/licenses/lgpl-2.1.html

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.
