#ifndef SAFE_THREAD_H
#define SAFE_THREAD_H

#include <thread>
#include <vector>
#include <functional>
#include <iostream>
#include <atomic>
#define ST SafeThread
class SafeThread;
extern std::vector<SafeThread*> open_safe_threads;
class SafeThread {
  std::thread thread;
  std::atomic<bool> running{false};  // Stop flag
  int id = 0;
 public:
  std::string attribute = "";
  bool open = false;
  SafeThread(std::function<void(std::atomic<bool>&)> func, std::string attr) {
    id = open_safe_threads.size();
    open_safe_threads.push_back(this);
    Start(func, attr);
    std::cout << "SafeThread " << attribute << " created\n";
  }
  SafeThread() {
    id = open_safe_threads.size();
    open_safe_threads.push_back(this);
    std::cout << "SafeThread " << attribute << " created\n";
  }
  ~SafeThread() {
    Close();
  }
  void Start(std::function<void(std::atomic<bool>&)> func, std::string attr = "") {
    std::cout << "Starting thread\n";
    if (open) {
      std::cout << "Closing thread\n";
      Close();
    }
    running = true;
      std::cout << "Starting thread\n";
    thread = std::thread(func, std::ref(running)); 
    std::cout << "Finished starting thread\n";
    open = true;
    attribute = attr;
    std::cout << "SafeThread " << attribute << " opened\n";
  }
  void Close() {
    running = false;
    if (open) {
      if (thread.joinable())
        thread.join();
    }
    open = false;
    std::cout << "SafeThread " << attribute << " closed\n";
  }
  int GetID() { return id; }
};
bool AllThreadsClosed();
void CloseAllThreads();
bool ThreadIsValid(SafeThread* thread);

#endif // SAFE_THREAD_H
