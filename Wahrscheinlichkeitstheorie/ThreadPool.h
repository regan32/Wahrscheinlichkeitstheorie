#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <atomic>
#include <functional>
#include <stdexcept>

    class ThreadPool {
    public:
        explicit ThreadPool(std::size_t threads
            = std::max(2u, std::thread::hardware_concurrency() * 2));
        template<class F, class... Args>
        auto enqueue(F&& f, Args&&... args)
            ->std::future<typename std::result_of<F(Args...)>::type>;
        void wait_until_empty();
        void wait_until_nothing_in_flight();
        ~ThreadPool();

    private:
        // need to keep track of threads so we can join them
        std::vector< std::thread > workers;
        // the task queue
        std::queue< std::function<void()> > tasks;

        // synchronization
        std::mutex queue_mutex;
        std::condition_variable condition;
        bool stop;

        std::mutex in_flight_mutex;
        std::condition_variable in_flight_condition;
        std::atomic<std::size_t> in_flight;

        struct handle_in_flight
        {
            ThreadPool & tp;

            handle_in_flight(ThreadPool & tp_)
                : tp(tp_)
            {
                std::atomic_fetch_add_explicit(&tp.in_flight,
                    std::size_t(1),
                    std::memory_order_relaxed);
            }

            ~handle_in_flight()
            {
                std::size_t prev
                    = std::atomic_fetch_sub_explicit(&tp.in_flight,
                        std::size_t(1),
                        std::memory_order_consume);
                if (prev == 1)
                    tp.in_flight_condition.notify_all();
            }
        };
    };

    // the constructor just launches some amount of workers
    inline ThreadPool::ThreadPool(std::size_t threads)
        : stop(false)
        , in_flight(0)
    {
        for (size_t i = 0; i<threads; ++i)
            workers.emplace_back(
                [this]
        {
            for (;;)
            {
                std::function<void()> task;
                bool empty;

                {
                    std::unique_lock<std::mutex> lock(this->queue_mutex);
                    this->condition.wait(lock,
                        [this] { return this->stop || !this->tasks.empty(); });
                    if (this->stop && this->tasks.empty())
                        return;
                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                    empty = this->tasks.empty();
                }

                handle_in_flight guard(*this);

                if (empty)
                    condition.notify_all();

                task();
            }
        }
        );
    }

    // add new work item to the pool
    template<class F, class... Args>
    auto ThreadPool::enqueue(F&& f, Args&&... args)
        -> std::future<typename std::result_of<F(Args...)>::type>
    {
        using return_type = typename std::result_of<F(Args...)>::type;

        auto task = std::make_shared< std::packaged_task<return_type()> >(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
            );

        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queue_mutex);

            // don't allow enqueueing after stopping the pool
            if (stop)
                throw std::runtime_error("enqueue on stopped ThreadPool");

            tasks.emplace([task]() { (*task)(); });
        }
        condition.notify_one();
        return res;
    }

    // the destructor joins all threads
    inline ThreadPool::~ThreadPool()
    {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            stop = true;
        }
        condition.notify_all();
        for (std::thread &worker : workers)
            worker.join();
    }

    inline void ThreadPool::wait_until_empty()
    {
        std::unique_lock<std::mutex> lock(this->queue_mutex);
        this->condition.wait(lock,
            [this] { return this->tasks.empty(); });
    }

    inline void ThreadPool::wait_until_nothing_in_flight()
    {
        std::unique_lock<std::mutex> lock(this->in_flight_mutex);
        this->in_flight_condition.wait(lock,
            [this] { return this->in_flight == 0; });
    }

// namespace progschj
