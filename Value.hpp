#pragma once

#include "Serializable.hpp"
#include "Stringable.hpp"
#include "Dumpable.hpp"
#include "implode.hpp"
#include "array_shift.hpp"
#include <stdint.h>
#include <unordered_map>
#include <cmath>
#include "EWHAT.hpp"
#include "explode.hpp"
#include "trim.hpp"
#include "parse.hpp"
#include "EMPTY_OR.hpp"
#include <iostream>
#include "str_serialize.hpp"
#include "cast.hpp"
#include "str_deserialize.hpp"

using namespace std;

template<typename real = float, typename S = uint32_t>
class ValueT: public Serializable<S>, public Stringable, public Dumpable {
    static_assert(
        is_floating_point<real>::value, 
        "ValueT<T> only supports floating-point types"
    );
public:
    inline ValueT(): Serializable<S>(), Stringable(), Dumpable(),
        name(""),
        value(real(0)),
        lower(-INFINITY),
        upper(INFINITY),
        step(NAN),
        constant(false),
        rounded(false),
        clips(false),
        throws(true)
    {
        fix();
    }

    template<typename Ts>
    inline ValueT(
        Ts value = real(0), const string& name = "", 
        real lower = -INFINITY, 
        real upper = INFINITY,
        real step = NAN, 
        bool constant = false,
        bool rounded = false, 
        bool clips = false,
        bool throws = true
    ):
        Serializable<S>(), Stringable(), Dumpable(),
        name(name),
        value(real(value)), 
        lower(real(lower)), 
        upper(real(upper)), 
        step(step), 
        constant(constant),
        rounded(rounded), 
        clips(clips), 
        throws(throws)
    {
        if (lower > upper)
            throw ERROR(ERR_INVALID_BOUNDS);
        if (!isnan(step) && step <= 0)
            throw ERROR(ERR_INVALID_STEP);
        fix();
    }

    template<typename Ts>
    inline ValueT(
        const string& name = "", 
        Ts value = real(0), 
        real lower = -INFINITY, 
        real upper = INFINITY,
        real step = NAN, 
        bool constant = false,
        bool rounded = false, 
        bool clips = false,
        bool throws = true
    ): 
        ValueT<Ts>(
            value, name, 
            lower, 
            upper, 
            step, 
            constant,
            rounded, 
            clips, 
            throws
        )
    {}
    
    inline ValueT(const ValueT& other):
        Serializable<S>(), Stringable(), Dumpable(),
        name(other.name),
        value(other.value),
        lower(other.lower), 
        upper(other.upper),
        step(other.step), 
        constant(other.constant),
        rounded(other.rounded),
        clips(other.clips),
        throws(other.throws)
    {
        fix(); // Ensure constraints are enforced after copying
    }

    // Move constructor
    inline ValueT(ValueT&& other) noexcept :
        Serializable<S>(), Stringable(), Dumpable(),
        name(std::move(other.name)),
        value(other.value),
        lower(other.lower),
        upper(other.upper),
        step(other.step),
        constant(other.constant),
        rounded(other.rounded),
        clips(other.clips),
        throws(other.throws)
    {
        fix();
        // No need to reset other; move(name) leaves it in a valid state
    }

    inline virtual ~ValueT() {}

    // Assignment operator

    inline ValueT& operator=(const ValueT& other) {
        if (constant)
            throw ERROR(ERR_ASSIGN_TO_CONSTANT);
        name = other.name;
        value = other.value;
        lower = other.lower;
        upper = other.upper;
        step = other.step;
        constant = other.constant;
        rounded = other.rounded;
        clips = other.clips;
        throws = other.throws;
        fix();
        return *this;
    }
    inline ValueT& operator=(real other) {
        if (constant)
            throw ERROR(ERR_ASSIGN_TO_CONSTANT);
        value = other;
        fix();
        return *this;
    }

    // Move assignment operator
    // inline ValueT& operator=(ValueT&& other) noexcept {
    //     if (constant)
    //         throw ERROR(ERR_ASSIGN_TO_CONSTANT);
    //     if (this != &other) {
    //         name = std::move(other.name);
    //         value = other.value;
    //         lower = other.lower;
    //         upper = other.upper;
    //         step = other.step;
    //         constant = other.constant;
    //         rounded = other.rounded;
    //         clips = other.clips;
    //         throws = other.throws;
    //         fix();
    //     }
    //     return *this;
    // }

    // Implements Serializable

    inline vector<S> serialize() override {
        vector<S> serialized = str_serialize<S>(name);
        serialized.push_back(cast<S>(value));
        serialized.push_back(cast<S>(lower));
        serialized.push_back(cast<S>(upper));
        serialized.push_back(cast<S>(step));
        serialized.push_back(constant);
        serialized.push_back(rounded);
        serialized.push_back(clips);
        serialized.push_back(throws);
        return serialized;
    }

    inline void deserialize(const vector<S>& serialized, size_t& nxt) override {
        try {  
            name = str_deserialize<S>(serialized, nxt);
            value = cast<real>(serialized[nxt++]);
            lower = cast<real>(serialized[nxt++]);
            upper = cast<real>(serialized[nxt++]);
            step = cast<real>(serialized[nxt++]);
            constant = serialized[nxt++];
            rounded = serialized[nxt++];
            clips = serialized[nxt++];
            throws = serialized[nxt++];
            if (!fix())
                throw ERROR("Couldn't sanitize deserialized value");
        } catch (exception& e) {
            throw ERROR("ValueT deserialization failed" + EWHAT);
        }
    }

    // Implements Stringable

    inline string toString() const override {
        return toString("=", "\n");
    }
    inline string toString(const string ksep, const string vsep) const {
        vector<string> lines = {
            "name" + ksep + name,
            "value" + ksep + to_string(value),
            "lower" + ksep + to_string(lower),
            "upper" + ksep + to_string(upper),
            "step" + ksep + to_string(step),
            "constant" + ksep + to_string(constant),
            "rounded" + ksep + to_string(rounded),
            "clips" + ksep + to_string(clips),
            "throws" + ksep + to_string(throws),
        };
        return implode(vsep, lines);
    }

    inline void fromString(const string& input) override {
        return fromString(input, "=", "\n");
    }
    inline void fromString(const string& input, const string ksep, const string vsep) {
        vector<string> lines = explode(vsep, input);
        for (const string& line: lines) {
            vector<string> splits = explode(ksep, trim(line));
            if (splits.size() < 2)
                throw ERROR(ERR_PARSE_LINE + line);            
            string key = array_shift(splits);
            string val = implode("=", splits);
            if (key == "name") name = val;
            else if (key == "value") value = parse<real>(val);
            else if (key == "lower") lower = parse<real>(val);
            else if (key == "upper") upper = parse<real>(val);
            else if (key == "step") step = parse<real>(val);
            else if (key == "constant") constant = parse<bool>(val);
            else if (key == "rounded") rounded = parse<bool>(val);
            else if (key == "clips") clips = parse<bool>(val);
            else if (key == "throws") throws = parse<bool>(val);
            else throw ERROR(ERR_PARSE_KEY + EMPTY_OR(line));
        }
    }

    unordered_map<string, real> getAsUMap() const {
        return {
            { "value", value },
            { "lower", lower },
            { "upper", upper },
            { "step", step },
            { "constant", constant },
            { "rounded", rounded },
            { "clips", clips },
            { "throws", throws },
        };
    }

    // TODO: use getAsUMap() instead!!
    unordered_map<string, string> getAsUMapStr(bool withName = true) const {
        unordered_map<string, string> map = {
            { "value", to_string(value) },
            { "lower", to_string(lower) },
            { "upper", to_string(upper) },
            { "step", to_string(step) },
            { "constant", to_string(constant) },
            { "rounded", to_string(rounded) },
            { "clips", to_string(clips) },
            { "throws", to_string(throws) },
        };
        if (withName) map["name"] = name;
        return map;
    }

    // Implements Dumpable
    string dump(bool show = false) const override {
        string output = toString();
        if (show) cout << output << endl;
        return output;
    }

    // Setters and Getters

    inline const string& getNameCRef() const { return name; }

    inline const real getValue() const { return value; }

    inline void setLower(real lower) {
        this->lower = lower;
        fix();
    }
    inline real getLower() const { return lower; }
    inline void setUpper(real upper) { 
        this->upper = upper; 
        fix();
    }
    inline real getUpper() const { return upper; }
    inline void setBounds(real lower, real upper) {
        setLower(lower);
        setUpper(upper);
        fix();
    }
    
    inline void setStep(real step) { 
        this->step = step; 
        fix();
    }
    inline real getStep() const {return step; }

    inline void setConstant(bool constant) { this->constant = constant; }
    inline bool isConstant() const { return constant; }

    inline bool isDiscrete() const { return !isnan(step) && step > 0; }

    inline void setRounded(bool rounded) { this->rounded = rounded; }
    inline bool isRounded() const { return rounded; }

    inline void setClips(bool clips) { this->clips = clips; }
    inline bool isClips() const { return clips; }

    inline void setThrows(bool throws) { this->throws = throws; }
    inline bool isThrows() const { return throws; }

    // Conversion operator to allow implicit conversion to real
    inline operator real() const { return value; }

    // Comparison operators

    inline bool operator==(const ValueT& other) const {
        return abs(value - other.value) <= numeric_limits<real>::epsilon();
    }
    inline bool operator==(real other) const {
        return *this == ValueT(other);
    }
    inline bool operator==(int other) const {
        return *this == ValueT((real)other);
    }


    inline bool operator!=(const ValueT& other) const {
        return !(*this == other);
    }
    inline bool operator!=(real other) const {
        return !(*this == other);
    }
    inline bool operator!=(int other) const {
        return !(*this == ValueT((real)other));
    }


    inline bool operator<(const ValueT& other) const {
        return (value - other.value) < -numeric_limits<real>::epsilon();
    }
    inline bool operator<(real other) const {
        return *this < ValueT(other);
    }


    inline bool operator<=(const ValueT& other) const {
        return *this < other || *this == other;
    }
    inline bool operator<=(real other) const {
        return *this < other || *this == other;
    }


    inline bool operator>(const ValueT& other) const {
        return (value - other.value) > numeric_limits<real>::epsilon();
    }
    inline bool operator>(real other) const {
        return *this > ValueT(other);
    }


    inline bool operator>=(const ValueT& other) const {
        return *this > other || *this == other;
    }
    inline bool operator>=(real other) const {
        return *this > other || *this == other;
    }


    // Arithmetic operators

    inline ValueT operator+(real other) const {
        ValueT result(*this);
        result.value += other;
        result.fix();
        return result;
    }

    inline ValueT operator-(real other) const {
        ValueT result(*this);
        result.value -= other;
        result.fix();
        return result;
    }

    inline ValueT operator*(real other) const {
        ValueT result(*this);
        result.value *= other;
        result.fix();
        return result;
    }

    inline ValueT operator/(real other) const {
        if (abs(other) <= numeric_limits<real>::epsilon())
            throw ERROR(ERR_DIVISION_BY_ZERO);
        ValueT result(*this);
        result.value /= other;
        result.fix();
        return result;
    }

    inline ValueT operator%(real other) const {
        if (abs(other) <= numeric_limits<real>::epsilon())
            throw ERROR(ERR_DIVISION_BY_ZERO);
        if (constant)
            throw ERROR(ERR_ASSIGN_TO_CONSTANT);
        ValueT result(*this);
        result.value = value - other * ::floor(value / other);
        result.fix();
        return result;
    }

    // Compound assignment operators

    inline ValueT& operator+=(const ValueT& other) {
        return *this = *this + other;
    }
    inline ValueT& operator+=(real other) {
        return *this = *this + ValueT(other);
    }

    inline ValueT& operator-=(const ValueT& other) {
        return *this = *this - other;
    }
    inline ValueT& operator-=(real other) {
        return *this = *this - ValueT(other);
    }

    inline ValueT& operator*=(const ValueT& other) {
        return *this = *this * other;
    }
    inline ValueT& operator*=(real other) {
        return *this = *this * ValueT(other);
    }

    inline ValueT& operator/=(const ValueT& other) {
        return *this = *this / other;
    }
    inline ValueT& operator/=(real other) {
        return *this = *this / ValueT(other);
    }

    inline ValueT& operator%=(const ValueT& other) {
        return *this = *this % other;
    }
    inline ValueT& operator%=(real other) {
        return *this = *this % ValueT(other);
    }

    // Stream 
    friend ostream& operator<<(ostream& os, const ValueT& r) {
        return os << r.value;
    }

    friend istream& operator>>(istream& is, ValueT& r) {
        real temp;
        is >> temp;
        r = temp;
        return is;
    }

protected:
    static constexpr const char* ERR_INVALID_BOUNDS = "Invalid bounds";
    static constexpr const char* ERR_INVALID_STEP = "Invalid discretization step";
    static constexpr const char* ERR_ASSIGN_TO_CONSTANT = "Cannot assign to constant";
    static constexpr const char* ERR_OUT_OF_BOUNDS = "Out of bounds";
    static constexpr const char* ERR_DIVISION_BY_ZERO = "Division by zero";
    static constexpr const char* ERR_PARSE_KEY = "Parse error: Unrecognised key in line: ";
    static constexpr const char* ERR_PARSE_LINE = "Parse error: Line is invalid: ";

    string name = "";
    real value = 0;
    real lower = -INFINITY, upper = INFINITY;
    real step = NAN;
    bool constant = false;
    bool rounded = false;
    bool clips = false;
    bool throws = true;

    inline bool fix() {
        discretize();
        round();
        clip();
        return validate();
    }

    inline void clip() {
        if (!clips) return;
        if (value < lower) value = lower;
        if (value > upper) value = upper;
    }

    inline void discretize() {
        if (!isDiscrete()) return;
        real steps = ::round((value - lower) / step);
        value = lower + steps * step;
    }

    inline void round() {
        if (rounded) value = ::round(value);
    }

    inline bool validate() const {
        if (value < lower || value > upper) {
            if (throws)
                throw ERROR(ERR_OUT_OF_BOUNDS);
            else return false;
        }
        return true;
    }
};

// template<typename... Ts>
// ValueT(...) -> ValueT<float, uint32_t>;

using Value = ValueT<float, uint32_t>;

