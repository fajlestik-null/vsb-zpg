#pragma once
#include "Includes.h"
#include "SubjectType.h"

class IObserver;

class Subject
{
protected:
	vector<IObserver*> mObservers;
public:
	Subject();
	virtual ~Subject();
	virtual SubjectType getType() const = 0;
	void attach(IObserver* observer);
	void detach(IObserver* observer);
	void notifyObservers();
};