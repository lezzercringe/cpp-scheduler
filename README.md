# C++ Scheduler

This was originally a part of university labwork, but I've decided to publish it in case I apply to some internship.\
It implements a simple synchronous scheduler, with a single thread, and a single graph of tasks.\
Usecases & examples are available in the tests directory.

Probably I should put a single example here, so that it's easier to see how it works.\
The main point is that tasks depending on each other can be added in any order, and the scheduler will execute them based on their dependencies.

```cpp
int main () {
  float a = 1;
  float b = -2;
  float c = 0;

  Scheduler scheduler;

  auto id1 = scheduler.add([](float a, float c){return -4 * a * c;}, a, c);
  auto id2 = scheduler.add([](float b, float v){return b * b + v;}, b, scheduler.getFutureResult<float>(id1));
  auto id3 = scheduler.add([](float b, float d){return -b + std::sqrt(d);}, b, scheduler.getFutureResult<float>(id2));
  auto id4 = scheduler.add([](float b, float d){return -b - std::sqrt(d);}, b, scheduler.getFutureResult<float>(id2));
  auto id5 = scheduler.add([]( float a, float v ){return v/(2*a);}, a, scheduler.getFutureResult<float>(id3));
  auto id6 = scheduler.add([]( float a, float v ){return v/(2*a);}, a, scheduler.getFutureResult<float>(id4));

  scheduler.executeAll();

  std::cout << "x1 = " << scheduler.getResult<float>(id5) << std::endl; std::cout << "x2 = " << scheduler.getResult<float>(id6) << std::endl;

}
```
