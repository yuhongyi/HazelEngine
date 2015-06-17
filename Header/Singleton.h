#pragma once

#include <memory>
using namespace std;

template <class T>
class Singleton
{
public:
	static T* GetInstance()
	{
		if(mInstance == nullptr)
		{
			mInstance = new T();
		}

		return mInstance;
	}

	void Destroy()
	{
		if(mInstance != nullptr)
		{
			delete mInstance;
			mInstance = nullptr;
		}
	}
private:
	static T* mInstance;
};

template< class T >
T *Singleton<T>::mInstance = nullptr;