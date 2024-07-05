#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <iostream>
#include "../DeviceDriver/DeviceDriver.cpp"
#include <string>

using namespace testing;
//using namespace std;

class MockFlashMemoryDevice : public FlashMemoryDevice {
public:
	MOCK_METHOD(unsigned char, read, (long), (override));
	MOCK_METHOD(void, write, (long, unsigned char), (override));
};

class DeviceDeriverTestFixture : public testing::Test {
public:
	NiceMock<MockFlashMemoryDevice> mockFlash;
	NiceMock<DeviceDriver> dd{ &mockFlash };
	App app{ dd };

	string getPrintResult(int address, int value) {
		ostringstream oss;
		auto oldCoutStreamBuf = cout.rdbuf();
		cout.rdbuf(oss.rdbuf());

		app.ReadAndPrint(address, value);

		cout.rdbuf(oldCoutStreamBuf);
		return oss.str();
	}
	
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

	try {
		dd.read(0);
	}
	catch (std::exception& e) {
		EXPECT_EQ(std::string{ "ReadFailException" }, e.what());
	}
}

//Write
TEST_F(DeviceDeriverTestFixture, ReadBeforeWrite) {
	EXPECT_CALL(mockFlash, read(0x00))
		.Times(5)
		.WillRepeatedly(Return(0xFF));
	dd.write(0, 1);
}

TEST_F(DeviceDeriverTestFixture, WriteNotEmpty) {
	EXPECT_CALL(mockFlash, read(0x00))
		.Times(5)
		.WillRepeatedly(Return(0xDA));
	try {
		dd.write(0,1);
	}
	catch (std::exception& e) {
		EXPECT_EQ(std::string{ "WriteFailException" }, e.what());
	}
}

TEST_F(DeviceDeriverTestFixture, WriteAndReadValueSame) {
	int data[1000] = { 0 };
	EXPECT_CALL(mockFlash, read(_)).WillRepeatedly(Return(0xFF));
	EXPECT_CALL(mockFlash, write(_,_))
		.WillOnce(Invoke([&data](int address, int value) {
		data[address] = value;
		}));
	
	dd.write(10, 2);
	
	EXPECT_EQ(2, data[10]);
}


//App
TEST_F(DeviceDeriverTestFixture, ReadAndPrint) {
	int startAddr = 0;
	int endAddr = 3;
	int value = 3;
	string expected = "";
	for (int addr = startAddr; addr <= endAddr; addr++) {
		EXPECT_CALL(mockFlash, read(addr))
			.Times(5)
			.WillRepeatedly(Return(value));
		expected += to_string(value) + " ";
	}
	expected += "\n";
	EXPECT_EQ(expected, getPrintResult(startAddr, endAddr));
}

TEST_F(DeviceDeriverTestFixture, WiteAll) {
	int value = 5;
	EXPECT_CALL(mockFlash, read(_))
		.Times(25)
		.WillRepeatedly(Return(0xFF));

	app.WriteAll(value);


}
