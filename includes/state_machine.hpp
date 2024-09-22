#ifndef STATE_MACHINE_HPP
# define STATE_MACHINE_HPP

# include <observer.hpp>
# include <unordered_map>
# include <functional>
# include <stdexcept>

using namespace std;
template<typename TState>
class StateMachine
{
    // Will have a size 2 vector observer. 
    // At 0 we have the actions
    // At 1 we have the transitions
    bool    has_state = false;
    TState current_state;
    unordered_map<TState, vector<Observer<TState>>> states;    
    public:
        void addState(const TState& state);
        void addTransition( const TState& startState, 
                            const TState& finalState,
                            const std::function<void()>&lambda);
        void addAction( const TState& state,
                        const function<void()>& lambda);
        void transitionTo(const TState& state);
        void update();
};

template<typename TState>
void StateMachine<TState>::addState(const TState& state)
{
    if (this->states.find(state) != this->states.end())
    {
        throw invalid_argument("Exception: State has already been created");
    }
    this->states[state] = vector<Observer<TState>>(2);
    if (!this->has_state)
    {
        this->current_state = state;
        this->has_state = true;
    }
}

template<typename TState>
void StateMachine<TState>::addTransition( const TState& startState, 
                    const TState& finalState,
                    const std::function<void()>&lambda)
{
    if (this->states.find(startState) == this->states.end())
    {
        throw invalid_argument("Exception: Handle state not found");
    }
    // Vector 1 has the observers for Transitions
    this->states[startState][1].subscribe(finalState, lambda);
}

template<typename TState>
void StateMachine<TState>::addAction( const TState& state,
                const function<void()>& lambda)
{
    if (this->states.find(state) == this->states.end())
    {
        throw invalid_argument("Exception: Handle state not found");
    }
    // Vector 0 has the observers for ACtions
    this->states[state][0].subscribe(state, lambda);
}

template<typename TState>
void StateMachine<TState>::transitionTo(const TState& state)
{
    // Check if current state exists, if current states [1] (transitions)
    // is not empty, and if it has the transitioning event
    if (this->states.find(this->current_state) == this->states.end() || 
        this->states[this->current_state][1].empty() ||
        !this->states[this->current_state][1].has_event(state))
    {
        throw invalid_argument("Exception: Current state has no transitions");
    }
    // Vector 1 has the observers for ACtions
    this->states[this->current_state][1].notify(state);
    this->current_state = state; 
}

template<typename TState>
void StateMachine<TState>::update()
{
    // No need to check if this observer has an event, as Observer[0] is myself
    if (this->states.find(this->current_state) == this->states.end() || 
        this->states[this->current_state][0].empty())    {
        throw invalid_argument("Exception: Current state has no actions");
    }
    // Vector 0 has the observers for Actions
    this->states[this->current_state][0].notify(this->current_state); 
};

#endif