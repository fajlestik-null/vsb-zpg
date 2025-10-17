#include "Subject.h"
#include "IObserver.h"

void Subject::attach(IObserver* observer)
{
	mObservers.push_back(observer);
}
void Subject::detach(IObserver* observer)
{
	mObservers.erase(remove(mObservers.begin(), mObservers.end(), observer), mObservers.end());
}

void Subject::notifyObservers()
{
	for (IObserver* observer : mObservers) {
		observer->notify(this);
	}
}