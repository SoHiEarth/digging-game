#include <error.h>

#include <Cocoa/Cocoa.h>
void errorWindow(const char* message) {
  @autoreleasepool {
    // Check if app is running in a GUI session
    if (!NSApp) {
      fprintf(stderr, "Error: Cannot show message box (not in GUI session).\n");
      return;
    }
    dispatch_async(dispatch_get_main_queue(), ^{
      NSAlert *alert = [[NSAlert alloc] init];
      [alert setMessageText:@"Error"];
      [alert setInformativeText:[NSString stringWithUTF8String:message]];
      [alert setAlertStyle:NSAlertStyleCritical];
      [alert addButtonWithTitle:@"OK"];
      [alert runModal];
    });
  }
}
