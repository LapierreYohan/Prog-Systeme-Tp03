 #include<mutex>  
 #include<condition_variable>  
 #include<thread>  
 #include<chrono>  
 #include<random>  
 #include<iostream>  
 typedef std::ranlux48 TRandomGenerator;  
 class CSemaphore  
 {  
 private:  
   std::mutex mutex_;  
   std::condition_variable condition_;  
   unsigned int count_;  
 public:  
   CSemaphore(int c=0)  
     : count_(c)  
   {}  
   void reset()  
   {  
        std::unique_lock<std::mutex> lock(mutex_);  
     count_=0;  
   }  
   void notify()  
   {  
        std::unique_lock<std::mutex> lock(mutex_);  
     ++count_;  
     condition_.notify_one();  
   }  
   void wait()  
   {  
        std::unique_lock<std::mutex> lock(mutex_);  
     while(!count_)  
       condition_.wait(lock);  
     --count_;  
   }  
 }; 