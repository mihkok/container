#include <vector>
#include <mutex>
#include <thread>
#include <iostream>

class ThreadSafeVector {
private:
    std::vector<int> data;
    mutable std::mutex mtx; // Добавлено mutable

public:
    void push_back(int value) {
        std::lock_guard<std::mutex> lock(mtx);
        data.push_back(value);
    }

    int pop_back() {
        std::lock_guard<std::mutex> lock(mtx);
        if (data.empty()) {
            throw std::runtime_error("Vector is empty");
        }
        int value = data.back();
        data.pop_back();
        return value;
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(mtx); // Теперь это работает
        return data.size();
    }
};

int main() {
    ThreadSafeVector tsv;

    // Добавление элементов в несколько потоков
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([&tsv, i]() {
            for (int j = 0; j < 1000; ++j) {
                tsv.push_back(i * 1000 + j);
            }
        });
    }

    // Ожидание завершения потоков
    for (auto& thread : threads) {
        thread.join();
    }

    // Вывод размера вектора
    std::cout << "Vector size: " << tsv.size() << std::endl;

    return 0;
}
