#include <gtest/gtest.h>
#include <iostream>
#include <vector>

namespace ex3
{
    int32_t recursive_power(int32_t base, int32_t exponent)
    {
        if (exponent == 0)
            return 1;
        else if (exponent == 1)
            return base;
        else if (exponent % 2 == 0)
            return recursive_power(base * base, exponent / 2);
        else
            return base * recursive_power(base * base, (exponent - 1) / 2);
    }

    int32_t iterative_power(int32_t base, int32_t exponent)
    {
        int32_t result = 1;
        while (exponent > 0)
        {
            if (exponent % 2 == 1)
                result *= base;
            base *= base;
            exponent /= 2;
        }
        return result;
    }

    TEST(Exercise3, test_recursive_power)
    {
        EXPECT_EQ(recursive_power(2, 0), 1);
        EXPECT_EQ(recursive_power(2, 1), 2);
        EXPECT_EQ(recursive_power(2, 2), 4);
        EXPECT_EQ(recursive_power(2, 3), 8);
        EXPECT_EQ(recursive_power(2, 4), 16);
        EXPECT_EQ(recursive_power(2, 5), 32);
        EXPECT_EQ(recursive_power(2, 15), 32768);
        EXPECT_EQ(recursive_power(2, 16), 65536);
    }

    TEST(Exercise3, test_iterative_power)
    {
        EXPECT_EQ(iterative_power(2, 0), 1);
        EXPECT_EQ(iterative_power(2, 1), 2);
        EXPECT_EQ(iterative_power(2, 2), 4);
        EXPECT_EQ(iterative_power(2, 3), 8);
        EXPECT_EQ(iterative_power(2, 4), 16);
        EXPECT_EQ(iterative_power(2, 5), 32);
        EXPECT_EQ(iterative_power(2, 15), 32768);
        EXPECT_EQ(iterative_power(2, 16), 65536);
    }
}

namespace ex4
{
    double binary_search_root(double (*f)(double))
    {
        double left = 0.0;
        double right = 1.0;
        double middle = (left + right) / 2;
        while (middle != left && middle != right)
        {
            if (f(middle) == 0.0)
                return middle;
            else if (f(middle) > 0.0)
                right = middle;
            else
                left = middle;
            middle = (left + right) / 2;
        }
        return middle;
    }

    TEST(Exercise4, test_binary_search_root)
    {
        EXPECT_DOUBLE_EQ(binary_search_root([](double x)
                                            { return x - 0.5; }),
                         0.5);
        EXPECT_DOUBLE_EQ(binary_search_root([](double x)
                                            { return x * x - 0.25; }),
                         0.5);
        EXPECT_DOUBLE_EQ(binary_search_root([](double x)
                                            { return x * x * x - 0.125; }),
                         0.5);
    }
}

namespace ex5
{
    double calculate_horner(std::vector<double> coefficients, double x)
    {
        double result = 0.0;
        for (auto a = coefficients.rbegin(); a != coefficients.rend(); ++a)
            result = result * x + *a;
        return result;
    }

    TEST(Exercise5, test_calculate_horner)
    {
        EXPECT_EQ(calculate_horner({1, 2, 3}, 0), 1);
        EXPECT_EQ(calculate_horner({1, 2, 3}, 1), 6);
        EXPECT_EQ(calculate_horner({1, 2, 3}, 2), 17);
        EXPECT_EQ(calculate_horner({1, 2, 3}, 3), 34);
        EXPECT_EQ(calculate_horner({4, 5, 3, 0, 0, 18}, 10), 1800354);
    }
}

namespace ex6
{
    struct Node
    {
        int32_t value;
        Node *next;
        Node(int32_t value, Node *next = nullptr)
            : value(value), next(next) {}
        Node(std::vector<int32_t> values)
            : value(values[0]), next(nullptr)
        {
            Node *current = this;
            for (size_t i = 1; i < values.size(); ++i)
            {
                current->next = new Node(values[i]);
                current = current->next;
            }
        }
    };

    size_t print(Node *head)
    {
        size_t count = 0;
        while (head != nullptr)
        {
            std::cout << head->value << " ";
            head = head->next;
            ++count;
        }
        std::cout << std::endl;
        return count;
    }

    int32_t sum(Node *head)
    {
        int32_t sum = 0;
        while (head != nullptr)
        {
            sum += head->value;
            head = head->next;
        }
        return sum;
    }

    int32_t nth(Node *head, size_t n)
    {
        for (size_t i = 0; i < n; ++i)
            if (head == nullptr)
                return 0;
            else
                head = head->next;
        return head == nullptr ? 0 : head->value;
    }

    void insert(Node *&head, int32_t value)
    {
        head = new Node(value, head);
    }

    void insert_after_smaller(Node *&head, int32_t value)
    {
        if (head == nullptr || head->value > value)
            insert(head, value);
        else
            insert_after_smaller(head->next, value);
    }

    void remove(Node *&head, int32_t value)
    {
        if (head == nullptr)
            return;
        if (head->value == value)
        {
            Node *to_delete = head;
            head = head->next;
            delete to_delete;
            remove(head, value);
        }
        else
            remove(head->next, value);
    }

    void filter(Node *&head, bool (*predicate)(int32_t))
    {
        if (head == nullptr)
            return;
        if (predicate(head->value))
            filter(head->next, predicate);
        else
        {
            Node *to_delete = head;
            head = head->next;
            delete to_delete;
            filter(head, predicate);
        }
    }

    void destroy(Node *&head)
    {
        while (head != nullptr)
        {
            Node *to_delete = head;
            head = head->next;
            delete to_delete;
        }
    }

    void reverse(Node *&head)
    {
        Node *previous = nullptr;
        while (head != nullptr)
        {
            Node *next = head->next;
            head->next = previous;
            previous = head;
            head = next;
        }
        head = previous;
    }

    Node *merge(Node *first, Node *second)
    {
        if (first == nullptr)
            return second;
        if (second == nullptr)
            return first;
        if (first->value < second->value)
        {
            first->next = merge(first->next, second);
            return first;
        }
        else
        {
            second->next = merge(first, second->next);
            return second;
        }
    }

    TEST(Exercise6, test_print)
    {
        Node *head = new Node({1, 2, 3, 4, 5});
        EXPECT_EQ(print(head), 5);
    }

    TEST(Exercise6, test_sum)
    {
        Node *head = new Node({1, 2, 3});
        EXPECT_EQ(sum(head), 6);
    }

    TEST(Exercise6, test_nth)
    {
        Node *head = new Node({1, 2, 3});
        EXPECT_EQ(nth(head, 0), 1);
        EXPECT_EQ(nth(head, 1), 2);
        EXPECT_EQ(nth(head, 2), 3);
        EXPECT_EQ(nth(head, 3), 0);
    }

    TEST(Exercise6, test_insert)
    {
        Node *head = new Node({1, 2, 3});
        insert(head, 0);
        EXPECT_EQ(nth(head, 0), 0);
        EXPECT_EQ(nth(head, 1), 1);
        EXPECT_EQ(nth(head, 2), 2);
        EXPECT_EQ(nth(head, 3), 3);
        EXPECT_EQ(nth(head, 4), 0);
    }

    TEST(Exercise6, test_insert_after_smaller)
    {
        Node *head = new Node({1, 2, 4});
        insert_after_smaller(head, 3);
        EXPECT_EQ(nth(head, 2), 3);
        EXPECT_EQ(print(head), 4);
    }

    TEST(Exercise6, test_remove)
    {
        Node *head = new Node({1, 2, 3, 4, 5});
        remove(head, 3);
        EXPECT_EQ(print(head), 4);

        head = new Node({7, 7, 7});
        remove(head, 7);
        EXPECT_EQ(print(head), 0);

        head = new Node({1, 2, 1, 1});
        remove(head, 1);
        EXPECT_EQ(print(head), 1);
    }

    TEST(Exercise6, test_filter)
    {
        Node *head = new Node({1, 2, 3, 4, 5});
        filter(head, [](int32_t x)
               { return x % 2 == 0; });
        EXPECT_EQ(print(head), 2);

        head = new Node({1, 2, 3, 4, 5});
        filter(head, [](int32_t x)
               { return x % 2 == 1; });
        EXPECT_EQ(print(head), 3);

        head = new Node({1, 2, 3, 4, 5});
        filter(head, [](int32_t x)
               { return x > 0; });
        EXPECT_EQ(print(head), 5);

        head = new Node({1, 2, 3, 4, 5});
        filter(head, [](int32_t x)
               { return x < 0; });
        EXPECT_EQ(print(head), 0);
    }

    TEST(Exercise6, test_destroy)
    {
        Node *head = new Node({1, 2, 3, 4, 5});
        destroy(head);
        EXPECT_EQ(head, nullptr);
    }

    TEST(Exercise6, test_reverse)
    {
        Node *head = new Node({1, 2, 3});
        reverse(head);
        EXPECT_EQ(nth(head, 0), 3);
        EXPECT_EQ(nth(head, 1), 2);
        EXPECT_EQ(nth(head, 2), 1);
        EXPECT_EQ(print(head), 3);
    }

    TEST(Exercise6, test_merge)
    {
        Node *first = new Node({1, 3, 5});
        Node *second = new Node({2, 4, 6});
        Node *merged = merge(first, second);
        EXPECT_EQ(nth(merged, 0), 1);
        EXPECT_EQ(nth(merged, 1), 2);
        EXPECT_EQ(nth(merged, 2), 3);
        EXPECT_EQ(nth(merged, 3), 4);
        EXPECT_EQ(nth(merged, 4), 5);
        EXPECT_EQ(nth(merged, 5), 6);
        EXPECT_EQ(print(merged), 6);
    }
}