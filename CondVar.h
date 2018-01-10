#ifndef COND_VAR_H
#define COND_VAR_H

// pthread conditional variable class

#include <pthread.h>
#include <unistd.h>
#include <iostream>

using namespace std;

class CondVar {
public:

  CondVar();
  ~CondVar();
  void wait(pthread_mutex_t* mutex);
  void signal();
  void broadcast();

private:
  pthread_cond_t m_cond_var;
};

#endif //COND_VAR_H
