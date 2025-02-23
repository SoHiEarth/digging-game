#ifndef SAFE_THREAD_H
#define SAFE_THREAD_H

#include <thread>
#include <vector>
#define ST SafeThread
class SafeThread;
extern std::vector<SafeThread*> open_safe_threads;
class SafeThread {
  std::thread thread;
 public:
  bool open = true;
  SafeThread() {
    open_safe_threads.push_back(this);
  }
  void Start(std::function<void()> func) {
    thread = std::thread(func);
    open = true;
  }
  void Close() {
    thread.join();
    open = false;
  }
};
bool all_threads_closed();
void close_all_threads();
#endif // SAFE_THREAD_H
