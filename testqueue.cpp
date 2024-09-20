#include "testqueue.h"
void test_size(){
    queue q(5);
    q.enqueue(1);
    q.enqueue(2);
    if(q.size() == 2){
        cout << "Test Size Passed" << endl;
    }
    else{
        cout << "Test Size Failed" << endl;
    }
}
void test_isFull(){
    queue q(2);
    q.enqueue(1);
    q.enqueue(2);
    if(q.isFull()){
        cout << "Test isFull Passed" << endl;
    }
    else{
        cout << "Test isFull Failed" << endl;
    }
}
void test_isEmpty(){
    queue q(2);
    if(q.isEmpty()){
        cout << "Test isEmpty Passed" << endl;
    }
    else{
        cout << "Test isEmpty Failed" << endl;
    }
}

void test_enqueue(){
    queue q(5);
    q.enqueue(1);
    q.enqueue(2);
    if(q.size() == 2){
         cout << "Test enqueue Passed" << endl;
    }
    else{
        cout << "Test enqueue Failed" << endl;
    }
}
void test_enqueue2(){
    queue q(1);
    q.enqueue(1);
    q.enqueue(2);
     if(q.size() == 1){
         cout << "Test enqueue2 Passed" << endl;
    }
    else{
        cout << "Test enqueue2 Failed" << endl;
    }
}

void test_dequeue(){
    queue q(1);
    q.enqueue(1);
    q.enqueue(2);
    q.dequeue();
     if(q.size() == 1){
         cout << "Test dequeue Passed" << endl;
    }
    else{
        cout << "Test dequeue Failed" << endl;
    }
}
void test_dequeue2(){
    queue q(1);
    q.dequeue();
     if(q.size() == 0){
         cout << "Test dequeue2 Passed" << endl;
    }
    else{
        cout << "Test dequeue2 Failed" << endl;
    }
}

void test_peek(){
    queue q(1);
    q.enqueue(23);
    if(q.peek() == 23){
        cout << "Test peek Passed" << endl;
    }
    else{
        cout << "Test peek Failed" << endl;
    }
}
void test_peak(){
    queue q(1);
    if(q.peek() > 0){
        cout << "Test peek2 Passed" << endl;
    }
    else{
        cout << "Test peek2 Failed" << endl;
    }
}