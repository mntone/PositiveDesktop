#include "pch.h"
#include "Common/Reps.h"

#include <condition_variable>
#include <optional>
#include <mutex>
#include <thread>

using namespace reps;

template<typename T>
class RepsTest: public testing::Test, public reps::basic_observer_t<int> {
protected:
	RepsTest() = default;

	void SetUp() override {
		subject_.addObserver(reps::observer_t<int>(this));
	}

	void next(int value) noexcept {
		subject_.next(value);
		++id_;
	}

	void FASTCALL on(reps::bag_t<int> const& value) noexcept override {
		latest_ = value.data;
	}

	std::optional<int> latest() const noexcept { return latest_; }

protected:
	int id_ { 0 };
	std::optional<int> latest_;
	T subject_;
};

using MultipleRepsTest = RepsTest<reps::subject_t<int>>;

TEST_F(MultipleRepsTest, SingleThread) {
	next(4);
	EXPECT_EQ(latest_, 4);
	next(6);
	EXPECT_EQ(latest_, 6);
}

TEST_F(MultipleRepsTest, MultiThread) {
	std::mutex mutex;
	std::condition_variable cond;
	std::thread thread([&cond, this] {
		next(12);
		cond.notify_one();
		Sleep(5);
		next(51);
		cond.notify_one();
	});

	std::unique_lock<std::mutex> locker(mutex);
	cond.wait(locker, [this] { return id_ == 1; });
	EXPECT_EQ(latest_, 12);
	cond.wait(locker, [this] { return id_ == 2; });
	EXPECT_EQ(latest_, 51);

	// Wait thread
	thread.join();
}


using SingleRepsTest = RepsTest<reps::single_subject_t<int>>;
TEST_F(SingleRepsTest, SingleThread) {
	next(4);
	EXPECT_EQ(latest_, 4);
	next(6);
	EXPECT_EQ(latest_, 6);
}
TEST_F(SingleRepsTest, MultiThread) {
	std::mutex mutex;
	std::condition_variable cond;
	std::thread thread([&cond, this] {
		next(12);
		cond.notify_one();
		Sleep(5);
		next(51);
		cond.notify_one();
	});

	std::unique_lock<std::mutex> locker(mutex);
	cond.wait(locker, [this] { return id_ == 1; });
	EXPECT_EQ(latest_, 12);
	cond.wait(locker, [this] { return id_ == 2; });
	EXPECT_EQ(latest_, 51);

	// Wait thread
	thread.join();
}
