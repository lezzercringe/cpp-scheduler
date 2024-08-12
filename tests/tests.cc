#include <gtest/gtest.h>
#include <scheduler/scheduler.hpp>
#include <stdexcept>

TEST(TaskWithReferences, BasicUseCases) {
  Scheduler scheduler;

  std::vector<int> numbers = {1, 2, 3, 4, 5};

  int expected = 15;

  auto id1 = scheduler.add([](const std::vector<int>& nums) {
    int sum = 0;
    for(int num : nums) {
      sum += num;
    }
    return sum;
  }, numbers);

  auto result = scheduler.getResult<int>(id1);

  EXPECT_EQ(expected, result);
}


TEST(ExceptionHandling, BasicUseCases) {
  Scheduler scheduler;

  scheduler.add([]() { throw std::runtime_error("Err");  return 5; });

  EXPECT_THROW({
    scheduler.executeAll();
  }, std::runtime_error);
}


TEST(RightTriangleSquareTest, BasicUseCases) {
  float a = 3;
  float h = 4;

  Scheduler scheduler;

  auto expected = (a * h) / 2;

  auto id1 = scheduler.add([](float a, float h) { return a * h; }, a, h);
  auto id2 = scheduler.add([](float v) { return v / 2; }, scheduler.getFutureResult<float>(id1));

  scheduler.executeAll();

  auto result = scheduler.getResult<float>(id2);

  EXPECT_EQ(expected, result);
}

TEST(MatrixMultiplicationTestImplicitExecution, BasicUseCases) {
  Scheduler scheduler;

  std::vector<std::vector<int>> matrix1 = {{1, 2}, {3, 4}};
  std::vector<std::vector<int>> matrix2 = {{5, 6}, {7, 8}};

  std::vector<std::vector<int>> expected = {{19, 22}, {43, 50}};

  auto id1 = scheduler.add([](const std::vector<std::vector<int>>& mat1, const std::vector<std::vector<int>>& mat2) {
    std::vector<std::vector<int>> result(2, std::vector<int>(2, 0));

    for(int i = 0; i < 2; i++) {
      for(int j = 0; j < 2; j++) {
	for(int k = 0; k < 2; k++) {
	  result[i][j] += mat1[i][k] * mat2[k][j];
	}
      }
    }

    return result;
  }, matrix1, matrix2);

  auto result = scheduler.getResult<std::vector<std::vector<int>>>(id1);
  EXPECT_EQ(expected, result);
}

TEST(MatrixMultiplicationTestWithExplicitExecution, BasicUseCases) {
  Scheduler scheduler;

  std::vector<std::vector<int>> matrix1 = {{1, 2}, {3, 4}};
  std::vector<std::vector<int>> matrix2 = {{5, 6}, {7, 8}};

  std::vector<std::vector<int>> expected = {{19, 22}, {43, 50}};

  auto id1 = scheduler.add([](const std::vector<std::vector<int>>& mat1, const std::vector<std::vector<int>>& mat2) -> std::vector<std::vector<int>> {
    std::vector<std::vector<int>> result(2, std::vector<int>(2, 0));

    for(int i = 0; i < 2; i++) {
      for(int j = 0; j < 2; j++) {
	for(int k = 0; k < 2; k++) {
	  result[i][j] += mat1[i][k] * mat2[k][j];
	}
      }
    }

    return result;
  }, matrix1, matrix2);

  scheduler.executeAll();
  auto result = scheduler.getResult<std::vector<std::vector<int>>>(id1);

  EXPECT_EQ(expected, result);
}

TEST(MixedDataTaskTypes, BasicUseCases) {
  Scheduler scheduler;

  int num = 5;
  std::string str = "hello";
  float f = 3.14;

  std::string expected = "hello: 5 - 3.140000";

  auto id1 = scheduler.add([](int num, const std::string& str, float f) {
    std::ostringstream oss;
    oss << str << ": " << num << " - " << std::fixed << std::setprecision(6) << f;
    return oss.str();
  }, num, str, f);

  auto result = scheduler.getResult<std::string>(id1);

  EXPECT_EQ(expected, result);
}

