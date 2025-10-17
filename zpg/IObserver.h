#pragma once

class Subject;

class IObserver {
public:

    virtual ~IObserver() = default;
    virtual void notify(Subject * subject) = 0;
};