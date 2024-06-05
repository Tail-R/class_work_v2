// Credit: https://github.com/TechHara/cpp_channel

#pragma once

#include "shared.hpp"

template <typename T>
class Channel;

// The interface to create channel
template <typename T>
std::pair<Channel<T>, Channel<T>> channel() {
    auto state = std::make_shared<typename Channel<T>::ChannelState>();

    Channel<T> sender(state, true);
    Channel<T> receiver(std::move(state), false);

    debug_log("DEBUG: Channel has been established");

    return {
        std::move(sender),
        std::move(receiver)
    };
}

/*
NOTE: Channel is not intended to be used as an instance.
Be aware the constructor is private, It means the class is
suppose to be created by the call of "channel()"
 */
template <typename T>
class Channel {
public:
    Channel(Channel const&) = delete;
    Channel(Channel&&) = default;
    Channel& operator = (Channel const&) = delete;
    Channel& operator = (Channel&&) = default;

    bool send(T item) {
        bool success = true;

        if (!is_sender_ || state_ == nullptr)
        {
            success = false;
        }
        else
        {
            std::lock_guard lock(state_->mtx);

            if (state_->closed)
            {
                success = false;
            }
            else
            {
                state_->queue.push(std::move(item));
                state_->cv.notify_one();
            }
        }

        return success;
    }

    std::optional<T> recv() {
        bool success = true;
        std::optional<T> item = std::nullopt;

        if (is_sender_ || state_ == nullptr)
        {
            success = false;
        }
        else
        {
            std::unique_lock lock(state_->mtx);

            state_->cv.wait(lock, [this] {
                return !state_->queue.empty() || state_->closed;
            });

            if (state_->queue.empty())
            {
                success = false;
            }
            else
            {
                item = std::move(state_->queue.front());
                state_->queue.pop();
            }
        }

        return success ? item : std::nullopt;
    }

    bool closed() {
        std::lock_guard lock(state_->mtx);
        return state_->closed;
    }

    void close() {
        if (state_ != nullptr) {
            std::lock_guard lock(state_->mtx);

            if (!state_->closed) {
                state_->closed = true;
                state_->cv.notify_one();

                debug_log("DEBUG: Channel has been closed");
            }
        }
    }

    ~Channel() { close(); }

private:
    using ChannelState = struct {
        std::queue<T> queue;
        std::mutex mtx;
        std::condition_variable cv;
        bool closed = false;
    };

    std::shared_ptr<ChannelState> state_;
    bool is_sender_;

    explicit Channel(std::shared_ptr<ChannelState> state, bool is_sender)
        : state_{std::move(state)}
        , is_sender_{is_sender} {}

    friend std::pair<Channel<T>, Channel<T>> channel<T>();
};
