#pragma once

#include <memory>
using namespace std;

template <class T>
class Singleton
{
public:
	static T* GetInstance()
	{
		if(mInstance == NULL)
		{
			mInstance = new T();
		}

		return mInstance;
	}

	void Destroy()
	{
		if(mInstance != NULL)
		{
			delete mInstance;
			mInstance = NULL;
		}
	}
private:
	static T* mInstance;
};

template< class T >
T *Singleton<T>::mInstance = NULL;