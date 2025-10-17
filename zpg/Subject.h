#pragma once
#include "Includes.h"

class IObserver;

class Subject
{
protected:
	vector<IObserver*> mObservers;
public:
	Subject();
	virtual ~Subject();
	void attach(IObserver* observer);
	void detach(IObserver* observer);
	void notifyObservers();
};