# UsoDllLoader 

## Description 

This PoC shows __a technique__ that can be used to __weaponize privileged file write vulnerabilities__ on Windows. It provides an alternative to the DiagHub DLL loading "exploit" found by James Forshaw (a.k.a. [@tiraniddo](https://twitter.com/tiraniddo)), which was fixed by Microsoft starting from build version 1903.

<p align="center">
  <img src="/screenshots/UsoDllLoader.gif">
</p>

## TL;DR

Starting from Windows 10, Microsoft introduced the `Update Session Orchestrator` service. __As a regular user__, you can interact with this service using COM, and start an "update scan" (i.e. check whether updates are available) or start the download of pending updates for example. There is even an undocumented built-in tool called `usoclient.exe`, which serves that purpose. 

From an attacker's standpoint, this service is interesting because it runs as `NT AUTHORITY\System` and it tries to load a __non-existent DLL__ (`windowscoredeviceinfo.dll`) __whenever an Update Session is created__.

This means that, if we found a privileged file write vulnerability in Windows or in some third-party software, we could copy our own version of `windowscoredeviceinfo.dll` into `C:\Windows\Sytem32\` and then have it loaded by the USO service to get arbitrary code execution as `NT AUTHORITY\System`. 

For more information:  
Part 1 - https://itm4n.github.io/usodllloader-part1/  
Part 2 - https://itm4n.github.io/usodllloader-part2/  

## Testing the PoC

### Composition

This solution is composed of two projects: __WindowsCoreDeviceInfo__ and __UsoDllLoader__. 

- __WindowsCoreDeviceInfo__ provides a PoC DLL that will start a bind shell on port 1337 (localhost only), whenever the `QueryDeviceInformation()` function is called. That's the name of the function used by the USO workers.

- __UsoDllLoader__, is a stripped-down version of `usoclient.exe`. It can be run as a regular user to interact with the USO service and have it load `windowscoredeviceinfo.dll`. Then, it will try to connect to the bind shell. 

### Build the solution 

The solution is already preconfigured so compiling should be easy. I'm using __Visual Studio 2019__. It might not work with older versions. 

1. Select `Release` config and `x64` architecure.
2. Build solution.
3. Output files:

    - The DLL: `.\x64\Release\WindowsCoreDeviceInfo.dll`
    - The loader: `.\x64\Release\UsoDllLoader.exe`

### Usage 

For testing purposes, you can:

1. __As an administrator__, copy `WindowsCoreDeviceInfo.dll` to `C:\Windows\System32\`. 
2. Use the loader as a regular user.
3. Hopefully enjoy a shell as `NT AUTHORITY\System`.

## Caveats

This method might not work in the following situations:

- One or several updates are waiting to be installed.
- Updates are being installed.

However, the Windows Update GUI ("Settings > Update & Security > Windows Update") seems to behave differently. Indeed, in this case, the DLL loading succeeds every time so, with some more work, I'm sure this technique can be improved. 
