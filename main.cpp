#include <iostream>

class Queue
{
private:
    int max_size;
    int front;
    int back;
    int *queue;
    int real_size;
public:
    Queue(int size)
    {
        queue = new int[size];
        max_size = size;
        front = 0;
        back = 0;
        real_size = 0;
    }
    ~Queue()
    {
        delete [] queue;
    }
    Queue& operator<<(int value)
    {
        if (real_size != max_size)
        {
            queue[back] = value;
            back = (back + 1) % max_size;
            real_size++;
        }
        return *this;
    }
    Queue& operator>>(int &value)
    {
        if (real_size != 0)
        {
            value = queue[front];
            front = (front + 1) % max_size;
            real_size--;
        }
        return *this;
    }
    int capacity() const
    {
        return max_size;
    }
    int size() const
    {
        return real_size;
    }
};

int main()
{
    Queue q(5);
    q << 1 << 2 << 3 << 4 << 5;
    int num;
    std::cout<<q.size()<<std::endl;
    q >> num>>num>>num>>num>>num>>num;
    std::cout<< num<<std::endl;
    std::cout<<q.capacity()<<std::endl;
    std::cout<<q.size()<<std::endl;
    return 0;
}
