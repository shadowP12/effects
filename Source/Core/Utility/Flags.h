#pragma once

template<typename Enum, typename Storage = uint32_t>
class Flags
{
public:
    using InternalType = Storage;

    Flags() = default;

    Flags(Enum value)
    {
        mBits = static_cast<Storage>(value);
    }

    Flags(const Flags<Enum, Storage>& value)
    {
        mBits = value.mBits;
    }

    explicit Flags(Storage bits)
    {
        mBits = bits;
    }

    bool isSet(Enum value) const
    {
        return (mBits & static_cast<Storage>(value)) == static_cast<Storage>(value);
    }

    bool isSetAny(Enum value) const
    {
        return (mBits & static_cast<Storage>(value)) != 0;
    }

    bool isSetAny(const Flags<Enum, Storage>& value) const
    {
        return (mBits & value.mBits) != 0;
    }

    Flags<Enum, Storage>& set(Enum value)
    {
        mBits |= static_cast<Storage>(value);

        return *this;
    }

    void unset(Enum value)
    {
        mBits &= ~static_cast<Storage>(value);
    }

    bool operator==(Enum rhs) const
    {
        return mBits == static_cast<Storage>(rhs);
    }

    bool operator==(const Flags<Enum, Storage>& rhs) const
    {
        return mBits == rhs.mBits;
    }

    bool operator==(bool rhs) const
    {
        return ((bool)*this) == rhs;
    }

    bool operator!=(Enum rhs) const
    {
        return mBits != static_cast<Storage>(rhs);
    }

    bool operator!=(const Flags<Enum, Storage>& rhs) const
    {
        return mBits != rhs.mBits;
    }

    Flags<Enum, Storage>& operator= (Enum rhs)
    {
        mBits = static_cast<Storage>(rhs);

        return *this;
    }

    Flags<Enum, Storage>& operator= (const Flags<Enum, Storage>& rhs)
    {
        mBits = rhs.mBits;

        return *this;
    }

    Flags<Enum, Storage>& operator|= (Enum rhs)
    {
        mBits |= static_cast<Storage>(rhs);

        return *this;
    }

    Flags<Enum, Storage>& operator|= (const Flags<Enum, Storage>& rhs)
    {
        mBits |= rhs.mBits;

        return *this;
    }

    Flags<Enum, Storage> operator| (Enum rhs) const
    {
        Flags<Enum, Storage> out(*this);
        out |= rhs;

        return out;
    }

    Flags<Enum, Storage> operator| (const Flags<Enum, Storage>& rhs) const
    {
        Flags<Enum, Storage> out(*this);
        out |= rhs;

        return out;
    }

    Flags<Enum, Storage>& operator&= (Enum rhs)
    {
        mBits &= static_cast<Storage>(rhs);

        return *this;
    }

    Flags<Enum, Storage>& operator&= (const Flags<Enum, Storage>& rhs)
    {
        mBits &= rhs.mBits;

        return *this;
    }

    Flags<Enum, Storage> operator& (Enum rhs) const
    {
        Flags<Enum, Storage> out = *this;
        out.mBits &= static_cast<Storage>(rhs);

        return out;
    }

    Flags<Enum, Storage> operator& (const Flags<Enum, Storage>& rhs) const
    {
        Flags<Enum, Storage> out = *this;
        out.mBits &= rhs.mBits;

        return out;
    }

    Flags<Enum, Storage>& operator^= (Enum rhs)
    {
        mBits ^= static_cast<Storage>(rhs);

        return *this;
    }

    Flags<Enum, Storage>& operator^= (const Flags<Enum, Storage>& rhs)
    {
        mBits ^= rhs.mBits;

        return *this;
    }

    Flags<Enum, Storage> operator^ (Enum rhs) const
    {
        Flags<Enum, Storage> out = *this;
        out.mBits ^= static_cast<Storage>(rhs);

        return out;
    }

    Flags<Enum, Storage> operator^ (const Flags<Enum, Storage>& rhs) const
    {
        Flags<Enum, Storage> out = *this;
        out.mBits ^= rhs.mBits;

        return out;
    }

    Flags<Enum, Storage> operator~ () const
    {
        Flags<Enum, Storage> out;
        out.mBits = (Storage)~mBits;

        return out;
    }

    operator bool() const
    {
        return mBits ? true : false;
    }

    operator uint8_t() const
    {
        return static_cast<UINT8>(mBits);
    }

    operator uint16_t () const
    {
        return static_cast<UINT16>(mBits);
    }

    operator uint32_t() const
    {
        return static_cast<UINT32>(mBits);
    }

    friend Flags<Enum, Storage> operator&(Enum a, Flags<Enum, Storage> &b)
    {
        Flags<Enum, Storage> out;
        out.mBits = a & b.mBits;
        return out;
    }

private:
    InternalType mBits{0};
};

#define FLAGS_OPERATORS(Enum) FLAGS_OPERATORS_IMP(Enum, uint32_t)

#define FLAGS_OPERATORS_IMP(Enum, Storage)																   \
		inline Flags<Enum, Storage> operator|(Enum a, Enum b) { Flags<Enum, Storage> r(a); r |= b; return r; } \
		inline Flags<Enum, Storage> operator&(Enum a, Enum b) { Flags<Enum, Storage> r(a); r &= b; return r; } \
		inline Flags<Enum, Storage> operator~(Enum a) { return ~Flags<Enum, Storage>(a); }