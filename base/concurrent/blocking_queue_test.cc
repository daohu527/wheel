#include "gtest/gtest.h"

namespace wheel {
namespace base {

TEST(PushTest, single) {
  BlockingQueue<int> q = new BlockingQueue<int>();
  q.Push(1);
  q.Push(2);

  EXPECT_EQ(q.Size() == 2);
}

}
}
