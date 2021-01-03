#ifndef WINDOWSMATCHINGPID_H
#define WINDOWSMATCHINGPID_H

// Explictly including QActionGroup fixes compile bug on linux
#include <qactiongroup.h>
#include <QDebug>

#include <X11/Xlib.h>
#include <X11/Xatom.h>

// Qt Compatibility
#undef Bool
#ifdef FrameFeature
#undef FrameFeature
#endif

class WindowsMatchingPid
{
public:
    WindowsMatchingPid();
    Window getWID(unsigned long pid);

private:
    void search(Window w);

    unsigned long  _pid;
    Atom           _atomPID;
    Display       *_display;
    Window  _result;
};
#endif // WINDOWSMATCHINGPID_H
