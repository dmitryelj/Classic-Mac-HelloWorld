/* Classic Macintosh Hello World example. Code can be built with "Think C". App tested on System 7.5. */

WindowPtr windowPtr;
Rect  windowBounds = {40, 40, 150, 150}, dragRect;
MenuHandle appleMenu, fileMenu;

enum {
 appleID = 1,
 fileID
};
enum {
 quitItem = 1
};
 
void InitMacintosh(void);
void SetUpMenus(void);
void SetUpWindow(void);
void HandleMenu (long mSelect);
void HandleMouseDown (EventRecord *theEvent);
void HandleEvent(void);
void DrawCenteredText(WindowPtr theWindow, StringPtr pascalString);

void InitMacintosh(void) {
  MaxApplZone();
 
  InitGraf(&thePort);
  InitFonts();
  FlushEvents(everyEvent, 0);
  InitWindows();
  InitMenus();
  TEInit();
  InitDialogs(0L);
  InitCursor();
}

void SetUpWindow(void) {
  dragRect = screenBits.bounds; 
  windowPtr = NewWindow(0L, &windowBounds, "\pHello", true, noGrowDocProc, (WindowPtr) -1L, true, 0);
  SetPort(windowPtr);
}

void DrawCenteredText(WindowPtr theWindow, StringPtr pascalString) {
  Rect windowRect = theWindow->portRect;
  Point penLocation;
  penLocation.h = (windowRect.left + windowRect.right)/2;
  penLocation.v = (windowRect.top + windowRect.bottom) / 2;
  penLocation.h -= StringWidth(pascalString) / 2;

  MoveTo(penLocation.h, penLocation.v);
  DrawString(pascalString);
}

void SetUpMenus(void) {
  InsertMenu(appleMenu = NewMenu(appleID, "\p\024"), 0);
  InsertMenu(fileMenu = NewMenu(fileID, "\pFile"), 0);
  DrawMenuBar();
  AddResMenu(appleMenu, 'DRVR');
  AppendMenu(fileMenu, "\pQuit/Q");
}

void HandleMenu (long mSelect) {
  int   menuID = HiWord(mSelect);
  int   menuItem = LoWord(mSelect);
  Str255  name;
  GrafPtr  savePort;
  WindowPeek frontWindow;
 
  switch (menuID) {
    case appleID:
       GetPort(&savePort);
       GetItem(appleMenu, menuItem, name);
       OpenDeskAcc(name);
       SetPort(savePort);
       break;
    case fileID:
       switch (menuItem) {
          case quitItem:
          ExitToShell();
          break;
       }
       break;
  }
}

void HandleMouseDown (EventRecord *theEvent) {
  WindowPtr theWindow;
  int   windowCode = FindWindow (theEvent->where, &theWindow);
 
  switch (windowCode) {
   case inSysWindow: 
     SystemClick (theEvent, theWindow);
     break;     
   case inMenuBar:
     HandleMenu(MenuSelect(theEvent->where));
     break;     
   case inDrag:
     if (theWindow == windowPtr)
       DragWindow(windowPtr, theEvent->where, &dragRect);
     break;      
   case inContent:
     if (theWindow == windowPtr) {
      if (theWindow != FrontWindow())
        SelectWindow(windowPtr);
      else
        InvalRect(&windowPtr->portRect);
      }
     break;    
   case inGoAway:
     if (theWindow == windowPtr && TrackGoAway(windowPtr, theEvent->where))
       HideWindow(windowPtr);
     break;
   }
}

void HandleEvent(void) {
 int   ok;
 EventRecord theEvent;

 HiliteMenu(0);
 SystemTask();  /* Handle desk accessories */ 
 ok = GetNextEvent (everyEvent, &theEvent);
 if (ok)
   switch (theEvent.what) {
   case mouseDown:
      HandleMouseDown(&theEvent);
      break;   
   case keyDown: 
   case autoKey:
      if ((theEvent.modifiers & cmdKey) != 0) {
         HandleMenu(MenuKey((char) (theEvent.message & charCodeMask)));
      }
      break;
   case updateEvt:
      BeginUpdate(windowPtr);
      DrawCenteredText(windowPtr, "\pHello, World!");
      EndUpdate(windowPtr);
      break;      
   case activateEvt:
      InvalRect(&windowPtr->portRect);
      break;
  }
}

main() {
   InitMacintosh();
   SetUpMenus();
   SetUpWindow();
 
   for (;;)
    HandleEvent();
}
