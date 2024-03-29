#include "TestFloatList.h"
#include "TestUtil.h"

#include "dreg/dreg.h"

#include <memory>

typedef std::unique_ptr<FloatList, std::function<void(FloatList*)>> FloatListUPtr;

void TestFloatList::test() {
	FloatListUPtr floatListUPtr(createFloatList(), deleteFloatList);
	FloatList* floatList = floatListUPtr.get();
	
	CuAssertPtrNotNull(floatList);
	TestUtil::checkFloatList(tc, floatList, NULL, 0);
	
	float values[] = {66.2, 32.45, 999.1, -0.25};
	setFloatList(floatList, values, 4);
	TestUtil::checkFloatList(tc, floatList, values, 4);
	
	setFloatList(floatList, values + 1, 2);
	TestUtil::checkFloatList(tc, floatList, values + 1, 2);
}