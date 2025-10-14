#pragma once
#include <vector>

using namespace std;

class IObserver;

class ISubject
{
public:
	ISubject() {};
	virtual ~ISubject() {};
	void attach(IObserver* observer);
	void detach(IObserver* observer);
	void notifyObservers();
protected:
	vector<IObserver*> mObservers;
};