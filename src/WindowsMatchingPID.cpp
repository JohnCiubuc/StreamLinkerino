#include "WindowsMatchingPID.h"
// Attempt to identify a window by name or attribute.
// by Adam Pierce <adam@doctort.org>
// modified by John Ciubuc <john.ciubuc@ttuhsc.edu>

WindowsMatchingPid::WindowsMatchingPid()
{
    _display = XOpenDisplay(0);
    // Get the PID property atom.
    _atomPID = XInternAtom(_display, "_NET_WM_PID", True);
    if(_atomPID == None)
    {
        qWarning("No Such Atom");
        return;
    }
}

Window WindowsMatchingPid::getWID(unsigned long pid)
{
    _pid = pid;
    _result = 0;
    search(XDefaultRootWindow(_display));
    return _result;
}

void WindowsMatchingPid::search(Window w)
{
    // Get the PID for the current Window.
    Atom           type;
    int            format;
    unsigned long  nItems;
    unsigned long  bytesAfter;
    unsigned char *propPID = 0;
    if(Success == XGetWindowProperty(_display, w, _atomPID, 0, 1, False, XA_CARDINAL,
                                     &type, &format, &nItems, &bytesAfter, &propPID))
    {
        if(propPID != 0)
        {
            // If the PID matches, add this window to the result and end.
            if(_pid == *((unsigned long *)propPID))
            {
                _result = w;
                XFree(propPID);
                return;
            }
        }
    }

    // Recurse into child windows.
    Window    wRoot;
    Window    wParent;
    Window   *wChild;
    unsigned  nChildren;
    if(0 != XQueryTree(_display, w, &wRoot, &wParent, &wChild, &nChildren))
    {
        for(unsigned i = 0; i < nChildren; i++)
            search(wChild[i]);
    }
}
