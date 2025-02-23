#include <safe_thread.h>

std::vector<SafeThread*> open_safe_threads;

bool AllThreadsClosed() {
  for (SafeThread* thread : open_safe_threads) {
    if (thread->open) {
      return false;
    }
  }
  return true;
}

void CloseAllThreads() {
  for (SafeThread* thread : open_safe_threads) {
    if (thread == nullptr) {
      continue;
    }
    std::cout << "Closing thread w/ attr " << thread->attribute << "\n";
    thread->Close();
    std::cout << "\rClosed thread w/ attr " << thread->attribute << "\n";
  }
  open_safe_threads.clear();
}

bool ThreadIsValid(SafeThread* thread) {
  for (SafeThread* open_thread : open_safe_threads) {
    if (open_thread == thread) {
      return true;
    }
  }
  return false;
}
