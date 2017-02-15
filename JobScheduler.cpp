#include "JobScheduler.h"
#include <iostream>
extern int thread_flag;
extern int stop_threads;

using namespace std;
#define RESULTSIZE 1024
JobScheduler::JobScheduler(uint32_t size_)
{
	size=size_;
	workers = (pthread_t*)malloc(sizeof(pthread_t)*size);
	assert(workers!=NULL);
	//mut = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t)*size);
    //for(int i=0;i<size;i++)
        //pthread_mutex_init(&mut[i],0);	//attribute?
	//mtx = (pthread_mutex_t)malloc(sizeof(pthread_mutex_t));

	pthread_mutex_init(&mtx, NULL);
	pthread_mutex_init(&mut,NULL);
	pthread_mutex_init(&mylock,NULL);
	pthread_cond_init(&cond_nonempty, 0);
    pthread_cond_init(&non_done, 0);
    results_size = RESULTSIZE;
    counter=0;
    queue = new JobList();
    results = (int*)malloc(sizeof(int)*RESULTSIZE);
    for(int i=0;i<RESULTSIZE;i++)
    { 
    	results[i] = -2;
    }

    for(int i=0;i<size;i++)
    {   
        pthread_create(&workers[i], NULL, &JobScheduler::send_wrapper, this);  //dimiourgeia threads
    }
    

}

JobScheduler::~JobScheduler()
{
	free(workers);
	free(results);
	pthread_mutex_destroy(&mtx);
	pthread_mutex_destroy(&mut);
	pthread_mutex_destroy(&mylock);
	pthread_cond_destroy(&cond_nonempty);
	pthread_cond_destroy(&non_done);
}
void JobScheduler::increase()
{
	pthread_mutex_lock(&mylock);
	uint32_t s = results_size;
	results_size = s*2;
	 cerr<<"ROPALO "<<results_size<<endl;
	results=(int*)realloc(results,sizeof(int)*results_size);
	for(int i=s;i<results_size;i++)
    { 
    	results[i] = -2;
    }

	assert(results!=NULL);
	pthread_mutex_unlock(&mylock);
}


uint32_t JobScheduler::get_resultsize()
{
	return results_size;
}

int JobScheduler::get_results(uint32_t i){
	return results[i];
}

void JobScheduler::reset_results()
{
	for(int i=0;i<results_size;i++)
    { 
    	results[i] = -2;
    }
}

void JobScheduler::submit_job(Job job)
{
    queue->Insert(job);
    pthread_mutex_lock(&mylock);
    counter++;
    pthread_mutex_unlock(&mylock);
}

void JobScheduler::wait_all_jobs()
{	
	pthread_mutex_lock(&mtx);
	stop_threads = 0;
	pthread_cond_signal(&cond_nonempty);
	pthread_mutex_unlock(&mtx);
	pthread_mutex_lock(&mylock);
	while(counter>0)
		pthread_cond_wait(&non_done,&mylock);
	pthread_mutex_unlock(&mylock);
}

void* JobScheduler::execute_all_jobs()
{
	Jobs* jobs;
	//cout<<"execute_all_jobs"<<endl;
	while(thread_flag)
	{
		jobs=NULL;
		pthread_mutex_lock(&mtx);
			while(queue->empty() || stop_threads)
				pthread_cond_wait(&cond_nonempty,&mtx);
		    jobs=queue->pop();
		    queue->remove();
		if(!queue->empty())
			pthread_cond_signal(&cond_nonempty);
	    pthread_mutex_unlock(&mtx);

	    for(int i=0;i<JOBS_NUMBER;i++)
	    {
	    	 if(jobs->isValid(i)==false)
	    	 {	//cerr<<"ropalo2"<<endl;
	    	 	break;
	    	 }
	    	 	
	    	 int re=jobs->getJob(i).run();
	    	 results[jobs->getJob(i).getOrder()] = re;
	    	 jobs->setInValid(i);
	    }
	   
	    pthread_mutex_lock(&mylock);
		    counter-=JOBS_NUMBER;
		  	jobs->setJobsCounterZero();
		    if(counter<=0)
		    {
		    	counter=0;
				pthread_cond_signal(&non_done);
			}
	    pthread_mutex_unlock(&mylock);
	   
   	}
   	pthread_exit(0);
}	

void JobScheduler::print_results()
{
	pthread_mutex_lock(&mylock);
	//cerr<< results_size<<"------------------------------"<<endl;
	for(int i=0; i< results_size; i++)
	{
		//cout<<"i="<<i<<" size="<<results_size<<endl;
		if(results[i]==-2)
			break;
		printf("%d\n",results[i]);
		results[i]=-2;
	}
	queue->Set();
	//cerr<<"------------------------------"<<endl;
	pthread_mutex_unlock(&mylock);
}
