# SITL gnss

Usage example:

```c++

#include <thread>
#include "gnss_sitl.hpp"

void application() {
    std::thread gnss_sitl_task(GnssSitlStartTask);
    gnss_sitl_task.detach();
}
```
