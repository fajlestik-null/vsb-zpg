#pragma once

class ISubject;

class IObserver {
public:

    virtual ~IObserver() = default;
    virtual void notify(ISubject * subject) = 0;
};