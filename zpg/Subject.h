#pragma once
#include <vector>

using namespace std;

class IObserver;

class Subject
{
public:
	Subject() {};
	virtual ~Subject() {};
	void attach(IObserver* observer);
	void detach(IObserver* observer);
	void notifyObservers();
protected:
	vector<IObserver*> mObservers;
};