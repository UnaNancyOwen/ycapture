ycapture with CMake
===================

This repository contains the source code for building ycapture using CMake.  
Please note this is not to guarantee same behavior as the original.  

Environment
-----------
* Visual Studio 2019 (or later)  
* CMake 3.15.4 (latest release is preferred)  

How To Build and Install
------------------------
Run following commands at a command prompt with administrative privileges.  
The ycapture will be built and installed.
```
git clone https://github.com/UnaNancyOwen/ycapture.git
cd ycapture
mkdir build
cd build
cmake -G "Visual Studio 16 2019" -A x64 -DCMAKE_INSTALL_PREFIX="C:/Program Files/ycapture" ..
cmake --build . --config Release
cmake --install .
```

How To Regist DLL and Run Test Program
--------------------------------------
Run following commands at a command prompt with administrative privileges.  
The testclient is input sample image that red band with color changes over time is to virtual video capture source of DirectShow.  
You can capture it in another application such as VLC.  
```
cd C:/Program Files/ycapture/bin
regsvr32 ycapture.dll
testclient.exe
```

License
-------
Copyright &copy; 2020 Tsukasa SUGIURA  
Distributed under the [MIT License](http://www.opensource.org/licenses/mit-license.php "MIT License | Open Source Initiative").  

Original License  
* ycapture | YzwLab  
  Copyright &copy; 2010 Satoshi YAZAWA  
  Distributed under the [MIT License](http://www.opensource.org/licenses/mit-license.php "MIT License | Open Source Initiative").  

* DirectShow Base Classes | Microsoft  
  Copyright &copy; 1992-2001 Microsoft Corporation  
  https://www.microsoft.com/en-us/download/details.aspx?id=8442  

Contact
-------
* Tsukasa Sugiura  
    * <t.sugiura0204@gmail.com>  
    * <http://unanancyowen.com>  

Reference
---------
* ycapture | YzwLab  
  <http://yzwlab.net/ycapture/>

* Microsoft Windows SDK for Windows 7 and .NET Framework 4 (ISO) | Microsoft  
  <https://www.microsoft.com/en-us/download/details.aspx?id=8442>  

* DirectShow Base Classes | Microsoft Docs  
  <https://docs.microsoft.com/en-us/windows/win32/directshow/directshow-base-classes>