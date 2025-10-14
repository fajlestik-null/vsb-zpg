#include "ISubject.h"
#include "IObserver.h"

void ISubject::attach(IObserver* observer)
{
	mObservers.push_back(observer);
}
void ISubject::detach(IObserver* observer)
{
	mObservers.erase(remove(mObservers.begin(), mObservers.end(), observer), mObservers.end());
}

void ISubject::notifyObservers()
{
	for (IObserver* observer : mObservers) {
		observer->notify(this);
	}
}