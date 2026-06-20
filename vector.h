//
// Created by cartercpp on 6/20/26.
//

#ifndef NBODYGRAVITATIONALSIMULATION_VEC_H
#define NBODYGRAVITATIONALSIMULATION_VEC_H

#include <initializer_list>
#include <array>
#include <tuple>
#include <concepts>
#include <cstddef>

/*
    Minimal Vec3 methods you actually need for n-body:

    Vec3 add(Vec3 a, Vec3 b)
    Vec3 sub(Vec3 a, Vec3 b)
    Vec3 scale(Vec3 v, double s)
    double dot(Vec3 a, Vec3 b)
    double length2(Vec3 v)   ← dot(v, v) is fine

    Nice to have (recommended):

    double length(Vec3 v)
    Vec3 normalize(Vec3 v)
 */

template <typename ValueType, std::size_t Dimension>
    requires ((std::integral<ValueType> || std::floating_point<ValueType>) && (Dimension > 0))
class vector
{
public:

    // CONSTRUCTORS & STUFF

    constexpr vector() noexcept = default;

    constexpr vector(std::initializer_list<ValueType> data)
    {
        if (data.size() != Dimension)
            throw std::invalid_argument{"Insufficient # of arguments given"};

        for (std::size_t i = 0; i < Dimension; ++i)
            m_data[i] = *(data.begin() + i);
    }

    // METHODS

    constexpr vector& operator+=(const vector& other)
    {
        for (std::size_t i = 0; i < Dimension; ++i)
            m_data[i] += other.m_data[i];

        return *this;
    }

    constexpr vector& operator-=(const vector& other)
    {
        for (std::size_t i = 0; i < Dimension; ++i)
            m_data[i] -= other.m_data[i];

        return *this;
    }

    constexpr vector& operator*=(ValueType scalar)
    {
        for (std::size_t i = 0; i < Dimension; ++i)
            m_data[i] *= scalar;

        return *this;
    }

    constexpr vector& operator/=(ValueType scalar)
    {
        for (std::size_t i = 0; i < Dimension; ++i)
            m_data[i] /= scalar;

        return *this;
    }

    constexpr ValueType squared_length() const
    {
        ValueType output = 0;

        for (const ValueType value : m_data)
            output += value * value;

        return output;
    }

private:

    template <std::size_t Index>
    friend constexpr ValueType& get(vector<ValueType, Dimension>& vecRef)
    {
        return vecRef.m_data[Index];
    }

    template <std::size_t Index>
    friend constexpr const ValueType& get(const vector<ValueType, Dimension>& vecRef)
    {
        return vecRef.m_data[Index];
    }

    friend constexpr ValueType operator*(const vector<ValueType, Dimension>& lArg, const vector<ValueType, Dimension>& rArg)
    {
        ValueType output = 0;

        for (std::size_t i = 0; i < Dimension; ++i)
            output += lArg.m_data[i] * rArg.m_data[i];

        return output;
    }

    std::array<ValueType, Dimension> m_data;
};

template <typename ValueType, std::size_t Dimension>
constexpr auto operator+(vector<ValueType, Dimension> lArg, const vector<ValueType, Dimension>& rArg)
{
    return lArg += rArg;
}

template <typename ValueType, std::size_t Dimension>
constexpr auto operator-(vector<ValueType, Dimension> lArg, const vector<ValueType, Dimension>& rArg)
{
    return lArg -= rArg;
}

template <typename ValueType, std::size_t Dimension>
constexpr auto operator*(vector<ValueType, Dimension> vec, ValueType scalar)
{
    return vec *= scalar;
}

template <typename ValueType, std::size_t Dimension>
constexpr auto operator*(ValueType scalar, vector<ValueType, Dimension> vec)
{
    return vec *= scalar;
}

template <typename ValueType, std::size_t Dimension>
constexpr auto operator/(vector<ValueType, Dimension> vec, ValueType scalar)
{
    return vec /= scalar;
}

template <typename ValueType, std::size_t Dimension>
struct std::tuple_size<vector<ValueType, Dimension>> : std::integral_constant<std::size_t, Dimension> {};

template <std::size_t Index, typename ValueType, std::size_t Dimension>
struct std::tuple_element<Index, vector<ValueType, Dimension>> { using type = ValueType; };

#endif //NBODYGRAVITATIONALSIMULATION_VEC_H