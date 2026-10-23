#include <gtest/gtest.h>
#include "../include/UnqPtr.h"

// ===== UnqPtr =====

TEST(UnqPtrTest, CreateAndDereference) {
    UnqPtr<int> p(new int(42));
    EXPECT_EQ(*p, 42);
}

TEST(UnqPtrTest, Get) {
    UnqPtr<int> p(new int(42));
    EXPECT_EQ(*p.get(), 42);
}

TEST(UnqPtrTest, MoveConstructor) {
    UnqPtr<int> a(new int(5));
    UnqPtr<int> b(std::move(a));

    EXPECT_EQ(*b, 5);
    EXPECT_EQ(a.get(), nullptr);
}

TEST(UnqPtrTest, MoveAssignment) {
    UnqPtr<int> a(new int(1));
    UnqPtr<int> b(new int(2));

    b = std::move(a);

    EXPECT_EQ(*b, 1);
    EXPECT_EQ(a.get(), nullptr);
}

TEST(UnqPtrTest, Release) {
    UnqPtr<int> p(new int(99));
    int* raw = p.release();

    EXPECT_EQ(*raw, 99);
    EXPECT_EQ(p.get(), nullptr);

    delete raw;
}

TEST(UnqPtrTest, Reset) {
    UnqPtr<int> p(new int(1));
    p.reset(new int(2));

    EXPECT_EQ(*p, 2);
}

TEST(UnqPtrTest, ResetToNull) {
    UnqPtr<int> p(new int(1));
    p.reset();

    EXPECT_EQ(p.get(), nullptr);
}

TEST(UnqPtrTest, DefaultConstructor) {
    UnqPtr<int> p;
    EXPECT_EQ(p.get(), nullptr);
}

#include "../include/ShrdPtr.h"

// ===== ShrdPtr =====

TEST(ShrdPtrTest, CreateAndDereference) {
    ShrdPtr<int> p(new int(42));
    EXPECT_EQ(*p, 42);
    EXPECT_EQ(p.use_count(), 1);
}

TEST(ShrdPtrTest, CopyConstructor) {
    ShrdPtr<int> a(new int(5));
    ShrdPtr<int> b = a;

    EXPECT_EQ(*a, 5);
    EXPECT_EQ(*b, 5);
    EXPECT_EQ(a.use_count(), 2);
    EXPECT_EQ(b.use_count(), 2);
}

TEST(ShrdPtrTest, CopyAssignment) {
    ShrdPtr<int> a(new int(1));
    ShrdPtr<int> b(new int(2));

    b = a;

    EXPECT_EQ(*a, 1);
    EXPECT_EQ(*b, 1);
    EXPECT_EQ(a.use_count(), 2);
}

TEST(ShrdPtrTest, DestructorDecrementsCount) {
    ShrdPtr<int> a(new int(5));
    {
        ShrdPtr<int> b = a;
        EXPECT_EQ(a.use_count(), 2);
    }
    EXPECT_EQ(a.use_count(), 1);
}

TEST(ShrdPtrTest, Reset) {
    ShrdPtr<int> p(new int(1));
    p.reset(new int(2));

    EXPECT_EQ(*p, 2);
    EXPECT_EQ(p.use_count(), 1);
}

TEST(ShrdPtrTest, Get) {
    ShrdPtr<int> p(new int(99));
    EXPECT_EQ(*p.get(), 99);
    EXPECT_NE(p.get(), nullptr);
}

#include "../include/DynamicArray.h"

// ===== Подтипизация =====

class Animal {
public:
    virtual ~Animal() {}
    virtual int legs() const { return 4; }
};

class Dog : public Animal {
public:
    int legs() const override { return 4; }
};

class Bird : public Animal {
public:
    int legs() const override { return 2; }
};

TEST(SubtypingTest, UnqPtrBaseFromDerived) {
    UnqPtr<Dog> d(new Dog());
    UnqPtr<Animal> a = std::move(d);

    EXPECT_EQ(a->legs(), 4);
    EXPECT_EQ(d.get(), nullptr);
}

TEST(SubtypingTest, ShrdPtrBaseFromDerived) {
    ShrdPtr<Dog> d(new Dog());
    ShrdPtr<Animal> a = d;

    EXPECT_EQ(a->legs(), 4);
    EXPECT_EQ(a.use_count(), 2);
    EXPECT_EQ(d.use_count(), 2);
}

TEST(SubtypingTest, ShrdPtrPolymorphism) {
    ShrdPtr<Animal> a(new Dog());
    EXPECT_EQ(a->legs(), 4);

    ShrdPtr<Animal> b(new Bird());
    EXPECT_EQ(b->legs(), 2);
}

TEST(SubtypingTest, ShrdPtrCopyShareCounter) {
    ShrdPtr<Dog> d(new Dog());
    {
        ShrdPtr<Animal> a = d;
        EXPECT_EQ(a.use_count(), 2);
    }
    EXPECT_EQ(d.use_count(), 1);
}

// ===== DynamicArray =====

TEST(DynamicArrayTest, EmptyOnCreate) {
    DynamicArray<int> arr;
    EXPECT_EQ(arr.GetSize(), 0);
}

TEST(DynamicArrayTest, AppendAndGet) {
    DynamicArray<int> arr;
    arr.Append(10);
    arr.Append(20);
    arr.Append(30);

    EXPECT_EQ(arr.GetSize(), 3);
    EXPECT_EQ(*arr.Get(0), 10);
    EXPECT_EQ(*arr.Get(1), 20);
    EXPECT_EQ(*arr.Get(2), 30);
}

TEST(DynamicArrayTest, Set) {
    DynamicArray<int> arr;
    arr.Append(10);
    arr.Set(0, 99);

    EXPECT_EQ(*arr.Get(0), 99);
}

TEST(DynamicArrayTest, GetOutOfRange) {
    DynamicArray<int> arr;
    arr.Append(10);

    EXPECT_THROW(arr.Get(5), std::out_of_range);
    EXPECT_THROW(arr.Get(-1), std::out_of_range);
}

TEST(DynamicArrayTest, Growth) {
    DynamicArray<int> arr;
    for (int i = 0; i < 100; i++) {
        arr.Append(i);
    }
    EXPECT_EQ(arr.GetSize(), 100);
    EXPECT_EQ(*arr.Get(99), 99);
}

// ===== UnqPtr<int[]> (массивы) =====

TEST(UnqPtrArrayTest, CreateAndIndex) {
    UnqPtr<int[]> arr(new int[5]);
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = 30;

    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[1], 20);
    EXPECT_EQ(arr[2], 30);
}

TEST(UnqPtrArrayTest, Get) {
    UnqPtr<int[]> arr(new int[3]);
    EXPECT_NE(arr.get(), nullptr);
}

TEST(UnqPtrArrayTest, Release) {
    UnqPtr<int[]> arr(new int[3]);
    int* raw = arr.release();

    EXPECT_NE(raw, nullptr);
    EXPECT_EQ(arr.get(), nullptr);

    delete[] raw;
}

TEST(UnqPtrArrayTest, Reset) {
    UnqPtr<int[]> arr(new int[3]);
    arr[0] = 42;
    arr.reset(new int[5]);
    arr[0] = 99;

    EXPECT_EQ(arr[0], 99);
}

TEST(UnqPtrArrayTest, Move) {
    UnqPtr<int[]> a(new int[3]);
    a[0] = 42;

    UnqPtr<int[]> b(std::move(a));

    EXPECT_EQ(b[0], 42);
    EXPECT_EQ(a.get(), nullptr);
}

// ===== ShrdPtr<int[]> (массивы) =====

TEST(ShrdPtrArrayTest, CreateAndIndex) {
    ShrdPtr<int[]> arr(new int[5]);
    arr[0] = 10;
    arr[1] = 20;

    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[1], 20);
    EXPECT_EQ(arr.use_count(), 1);
}

TEST(ShrdPtrArrayTest, Copy) {
    ShrdPtr<int[]> a(new int[3]);
    a[0] = 42;

    ShrdPtr<int[]> b = a;

    EXPECT_EQ(b[0], 42);
    EXPECT_EQ(a.use_count(), 2);
    EXPECT_EQ(b.use_count(), 2);
}

TEST(ShrdPtrArrayTest, DestructorDecrements) {
    ShrdPtr<int[]> a(new int[3]);
    {
        ShrdPtr<int[]> b = a;
        EXPECT_EQ(a.use_count(), 2);
    }
    EXPECT_EQ(a.use_count(), 1);
}

TEST(ShrdPtrArrayTest, Reset) {
    ShrdPtr<int[]> arr(new int[3]);
    arr[0] = 42;
    arr.reset(new int[5]);
    arr[0] = 99;

    EXPECT_EQ(arr[0], 99);
    EXPECT_EQ(arr.use_count(), 1);
}