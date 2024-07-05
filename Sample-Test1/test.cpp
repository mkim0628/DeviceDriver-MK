#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../DeviceDriver/DeviceDriver.cpp"

using namespace testing;

class MockFlashMemoryDevice : public FlashMemoryDevice {
public:
	MOCK_METHOD(unsigned char, read, (long), (override));
	MOCK_METHOD(void, write, (long, unsigned char), (override));
};

class DeviceDeriverTestFixture : public testing::Test {
public:
	MockFlashMemoryDevice mockFlash;
	DeviceDriver dd{ &mockFlash };
};

TEST_F(DeviceDeriverTestFixture, ReadOK) {
	EXPECT_CALL(mockFlash, read(_)).Times(5).WillRepeatedly(Return(1));
	int actual = dd.read(0);

	EXPECT_EQ(1, actual);
}

TEST_F(DeviceDeriverTestFixture, ReadNotSame) {
	EXPECT_CALL(mockFlash, read(_))
		.Times(5)
		.WillOnce(Return(1))
		.WillOnce(Return(1))
		.WillOnce(Return(1))
		.WillOnce(Return(1))
		.WillOnce(Return(0));

	EXPECT_THROW(dd.read(0), std::runtime_error);
}

TEST_F(DeviceDeriverTestFixture, WriteOK) {
	int data[1000] = { 0 };
	EXPECT_CALL(mockFlash, read(_)).WillRepeatedly(Return(0xFF));
	EXPECT_CALL(mockFlash, write(_,_))
		.WillOnce(Invoke([&data](int address, int value) {
		data[address] = value;
		}));
	
	dd.write(10, 2);
	
	EXPECT_EQ(2, data[10]);
}


TEST_F(DeviceDeriverTestFixture, WriteNotEmpty) {
	int data[1000] = { 0 };
	EXPECT_CALL(mockFlash, read(_)).WillRepeatedly(Return(3));

	EXPECT_THROW(dd.write(10,2), std::runtime_error);
}

