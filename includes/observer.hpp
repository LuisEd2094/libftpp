#ifndef OBSERVER_HPP
# define OBSERVER_HPP


using namespace std;
# include <unordered_map>
# include <vector>
# include <functional>
template<typename TEvent>
class Observer
{
    private:
        unordered_map<TEvent, vector<function<void()>>> observers; 
    public:
        void subscribe(const TEvent& event, const function<void()>& lambda);
        void notify(const TEvent& event);
        bool empty();
        bool has_event(TEvent);
};

template<typename TEvent>
bool Observer<TEvent>::has_event(TEvent event)
{
    return !this->observers[event].empty();
};


template<typename TEvent>
bool Observer<TEvent>::empty()
{
    return this->observers.empty();
};


template<typename TEvent>
void Observer<TEvent>::subscribe(const TEvent& event, const function<void()>& lambda)
{
    this->observers[event].push_back(lambda);
};

template<typename TEvent>
void Observer<TEvent>::notify(const TEvent& event)
{
    for (auto callback : observers[event])
    {
        callback();
    }
};

#endif