#include <safe_thread.h>

std::vector<SafeThread*> open_safe_threads;

bool all_threads_closed() {
  for (SafeThread* thread : open_safe_threads) {
    if (thread->open) {
      return false;
    }
  }
  return true;
}

void close_all_threads() {
  for (SafeThread* thread : open_safe_threads) {
    if (thread->open) {
      thread->Close();
    }
  }
}
