#include "Sofa.h"
#include <chrono>
#include "MultiAllocSofa.h"
#include <iostream>
using namespace std;
//
//
//struct RenderableObjectData
//{
//	static const size_t size = sizeof(int) + sizeof(int) + sizeof(bool) + sizeof(char);
//	size_t allocated = 0;
//	size_t used = 0;
//	void* data = nullptr;
//	int* key;
//	int* v1;
//	bool* v2;
//	char* v3;
//};
//
//RenderableObjectData renderableObjectInfo;
//std::unordered_map<int, size_t, std::hash<int>> entityToRenderableObjectInfoIndex;
//
//
//
//
//
//
//void Allocate(size_t size)
//{
//	_ASSERT(size > renderableObjectInfo.allocated);
//
//	// Allocate new memory
//	RenderableObjectData newData;
//	newData.allocated = size;
//	newData.data = operator new(size * RenderableObjectData::size);
//	newData.used = renderableObjectInfo.used;
//
//	// Setup the new pointers
//	newData.key = (int*)newData.data;
//	newData.v1 = (int*)(newData.key + newData.allocated);
//	newData.v2 = (bool*)(newData.v1 + newData.allocated);
//	newData.v3 = (char*)(newData.v2 + newData.allocated);
//
//	// Copy data
//	memcpy(newData.key, renderableObjectInfo.key, renderableObjectInfo.used * sizeof(int));
//	memcpy(newData.v1, renderableObjectInfo.v1, renderableObjectInfo.used * sizeof(int));
//	memcpy(newData.v2, renderableObjectInfo.v2, renderableObjectInfo.used * sizeof(bool));
//	memcpy(newData.v3, renderableObjectInfo.v3, renderableObjectInfo.used * sizeof(char));
//
//	// Delete old data;
//	operator delete(renderableObjectInfo.data);
//	renderableObjectInfo = newData;
//}
//
//
//enum Data
//{
//	hp,
//	visible
//};



int main()
{
	//Allocate(64);

	Sofa<int, std::hash<int>, int, bool, char> sofa;

	sofa.add(123, 321, false, 'asd');
	if (const auto find = sofa.find(123); find.has_value())
	{
		sofa.destroy(find->second);
	}
	sofa.add(123, 321, false, 'asd');
	sofa.erase(123);


	SofaMA<int, std::hash<int>, int, bool> sofaMA;
	sofaMA.add(123, 321, true);
	sofaMA.add(124, 322, true);
	int i = 0;
	sofaMA.erase(123);
	//Sofa<int, std::hash<int>, int, bool, char> sofa2;
	//Sofa<int, std::hash<int>, int, bool, char> sofa3;
	//Sofa<int, std::hash<int>, int, bool, char> sofa4;
	//static const std::size_t count = 1000;
	//{
	//	auto s = std::chrono::high_resolution_clock::now();
	//	for (int i = 0; i < count; i++)
	//	{
	//		sofa4.add(i, i * 10, i % 2, 'c');
	//	}
	//	auto e = std::chrono::high_resolution_clock::now();
	//	auto diff = std::chrono::duration<float, std::milli>(e - s).count();
	//	printf("Add Sofa T: %f\n", diff);
	//}
	////{
	////	auto s = std::chrono::high_resolution_clock::now();
	////	for (int i = 0; i < count; i++)
	////	{
	////		auto index = sofa3.add(i);
	////		std::get<0>(sofa3.typePointers)[index] = i * 10;
	////		std::get<1>(sofa3.typePointers)[index] = i % 2;
	////		std::get<2>(sofa3.typePointers)[index] = 'c';
	////	}
	////	auto e = std::chrono::high_resolution_clock::now();
	////	auto diff = std::chrono::duration<float, std::milli>(e - s).count();
	////	printf("Add Sofa P: %f\n", diff);
	////}


	////{

	////	for (int i = 0; i < 100; i++)
	////	{
	////		auto index = sofa3.add(i);
	////		sofa3.get<0>()[index] = i * 10;
	////		sofa3.get<1>()[index] = i % 2;
	////		sofa3.get<2>()[index] = 'c';
	////	}



	////	for (size_t i = 0; i < sofa3.size(); i++)
	////	{
	////		cout << sofa3.get<hp>()[i] << endl;
	////		cout << sofa3.get<visible>()[i] << endl;
	////	}

	////}


	//{
	//	auto s = std::chrono::high_resolution_clock::now();
	//	for (int i = 0; i < count; i++)
	//	{
	//		auto index = sofa.add(i);
	//		sofa.set<hp>(index, i * 10);
	//		sofa.set<1>(index, i % 2);
	//		sofa.set<2>(index, 'c');
	//	}
	//	auto e = std::chrono::high_resolution_clock::now();
	//	auto diff = std::chrono::duration<float, std::milli>(e - s).count();
	//	printf("Add Sofa: %f\n", diff);
	//}
	//{
	//	auto s = std::chrono::high_resolution_clock::now();
	//	for (int i = 0; i < count; i++)
	//	{
	//		auto index = sofa2.add(i);
	//		sofa2.get<0>()[index] = i*10;
	//		sofa2.get<1>()[index] = i % 2;
	//		sofa2.get<2>()[index] = 'c';
	//	}
	//	auto e = std::chrono::high_resolution_clock::now();
	//	auto diff = std::chrono::duration<float, std::milli>(e - s).count();
	//	printf("Add SofaD: %f\n", diff);
	//}
	//{
	//	auto s = std::chrono::high_resolution_clock::now();
	//	for (int i = 0; i < count; i++)
	//	{
	//		auto index = sofa3.add(i);
	//		sofa3.get<0>(index) = i * 10;
	//		sofa3.get<1>(index) = i % 2;
	//		sofa3.get<2>(index) = 'c';
	//	}
	//	auto e = std::chrono::high_resolution_clock::now();
	//	auto diff = std::chrono::duration<float, std::milli>(e - s).count();
	//	printf("Add SofaDI: %f\n", diff);
	//}
	//{
	//	auto s = std::chrono::high_resolution_clock::now();
	//	auto sum = 0;
	//	for (size_t i = 0; i < sofa.size(); i++)
	//	{
	//	//	printf("i: %d ", sofa.get<0>(i));
	//		//printf("b: %s\n", sofa.get<1>(i) ? "true" : "false");
	//		sum += sofa.get<2>(i) ? sofa.get<1>(i) : 1337;
	//	}
	//	auto e = std::chrono::high_resolution_clock::now();
	//	auto diff = std::chrono::duration<float, std::milli>(e - s).count();
	//	printf("Iter Sofa: %f, %d\n", diff, sum);
	//}
	//{
	//	auto s = std::chrono::high_resolution_clock::now();
	//	auto sum = 0;
	//	for (size_t i = 0; i < sofa.size(); i++)
	//	{
	//		if (auto const find = sofa.find(i); find.has_value())
	//			sum += sofa.get<2>(find->second) ? sofa.get<1>(find->second) : 1337;
	//	}
	//	auto e = std::chrono::high_resolution_clock::now();
	//	auto diff = std::chrono::duration<float, std::milli>(e - s).count();
	//	printf("Find sofa: %f, %d\n", diff, sum);
	//}
	//{
	//	auto s = std::chrono::high_resolution_clock::now();
	//	for (int i = 0; i < count; i++)
	//	{
	//		if (renderableObjectInfo.used + 1 > renderableObjectInfo.allocated)
	//			Allocate(renderableObjectInfo.allocated * 2);
	//		auto index = entityToRenderableObjectInfoIndex[i] = renderableObjectInfo.used++;
	//		renderableObjectInfo.key[index] = i;
	//		renderableObjectInfo.v1[index] = i*10;
	//		renderableObjectInfo.v2[index] = i % 2;
	//		renderableObjectInfo.v3[index] = 'c';
	//	}
	//	auto e = std::chrono::high_resolution_clock::now();
	//	auto diff = std::chrono::duration<float, std::milli>(e - s).count();
	//	printf("Add: %f\n", diff);
	//}
	//

	//{
	//	auto s = std::chrono::high_resolution_clock::now();
	//	auto sum = 0;
	//	for (size_t i = 0; i < renderableObjectInfo.used; i++)
	//	{
	//
	//		sum += renderableObjectInfo.v2[i] ? renderableObjectInfo.v1[i] : 1337;
	//	}
	//	auto e = std::chrono::high_resolution_clock::now();
	//	auto diff = std::chrono::duration<float, std::milli>(e - s).count();
	//	printf("Iter: %f, %d\n", diff, sum);
	//}

	//{
	//	auto s = std::chrono::high_resolution_clock::now();
	//	auto sum = 0;
	//	for (size_t i = 0; i < renderableObjectInfo.used; i++)
	//	{
	//		if (auto const find = entityToRenderableObjectInfoIndex.find(i); find != entityToRenderableObjectInfoIndex.end())
	//			sum += renderableObjectInfo.v2[find->second] ? renderableObjectInfo.v1[find->second] : 1337;
	//	}
	//	auto e = std::chrono::high_resolution_clock::now();
	//	auto diff = std::chrono::duration<float, std::milli>(e - s).count();
	//	printf("Find: %f, %d\n", diff, sum);
	//}
	//getchar();
	return 0;
}