#pragma once

#include "Inline/Assert.h"
#include "Inline/BasicTypes.h"
#include "Inline/Hash.h"
#include "Inline/HashTable.h"

#include <initializer_list>

template<typename Element>
struct HashSetIterator
{
	template<typename,typename>
	friend struct HashSet;

	bool operator!=(const HashSetIterator& other);
	bool operator==(const HashSetIterator& other);
	operator bool() const;

	void operator++();

	const Element& operator*() const;
	const Element* operator->() const;

private:

	const HashTableBucket<Element>* bucket;
	const HashTableBucket<Element>* endBucket;

	HashSetIterator(
		const HashTableBucket<Element>* inBucket,
		const HashTableBucket<Element>* inEndBucket
		);
};

template<typename Element, typename ElementHashPolicy = DefaultHashPolicy<Element>>
struct HashSet
{
	HashSet(Uptr reserveNumElements = 0);
	HashSet(const std::initializer_list<Element>& initializerList);

	// If the set contains the element already, returns false.
	// If the set didn't contain the element, adds it and returns true.
	bool add(const Element& element);
	
	// If the set contains the element, removes it and returns true.
	// If the set doesn't contain the element, returns false.
	bool remove(const Element& element);
	
	// Returns a reference to the element in the set matching the given element. Assumes that the
	// map contains the key. This is useful if the hash policy allows distinct elements to compare
	// as equal; e.g. for deduplicating equivalent values.
	const Element& operator[](const Element& element) const;

	// Returns true if the set contains the element.
	bool contains(const Element& element) const;

	// If the set contains the element, returns a pointer to it. This is useful if the hash policy
	// allows distinct elements to compare as equal; e.g. for deduplicating equivalent values.
	const Element* get(const Element& element) const;

	HashSetIterator<Element> begin() const;
	HashSetIterator<Element> end() const;

	Uptr num() const;
	
	// Compute some statistics about the space usage of this set.
	void analyzeSpaceUsage(
		Uptr& outTotalMemoryBytes,
		Uptr& outMaxProbeCount,
		F32& outOccupancy,
		F32& outAverageProbeCount
		) const;

private:

	struct HashTablePolicy
	{
		FORCEINLINE static const Element& getKey(const Element& element)
		{
			return element;
		}
		FORCEINLINE static bool areKeysEqual(const Element& left, const Element& right)
		{
			return ElementHashPolicy::areKeysEqual(left, right);
		}
	};

	HashTable<Element, Element, HashTablePolicy> table;
};

// The implementation is defined in a separate file.
#include "HashSetImpl.h"