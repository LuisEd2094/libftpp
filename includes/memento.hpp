#ifndef MEMENTO_HPP
# define MEMENTO_HPP

# include <data_structures.hpp>

class Memento
{
    public:
        class Snapshot
        {
            friend class Memento;
            private:
                DataBuffer data;
            public:
                Snapshot(){};
                template<typename T>
                Snapshot& operator<<(const T& obj)
                {
                    this->data << obj;
                    return *this;
                }
                template<typename T>
                Snapshot& operator>>(T& obj)
                {
                    this->data >> obj;
                    return *this;
                }
        };
        
        virtual void _loadFromSnapshot(Snapshot& snapshot) = 0;
        virtual void _saveToSnapshot(Snapshot& snapshot) = 0;
        Snapshot save();
        void load(Memento::Snapshot& state);
        virtual ~Memento(){};

};

Memento::Snapshot Memento::save()
{
    Memento::Snapshot snap;
    this->_saveToSnapshot(snap);
    return snap;
}
void Memento::load(Memento::Snapshot& state)
{
    this->_loadFromSnapshot(state);
}

#endif 