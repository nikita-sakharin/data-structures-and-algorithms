#ifndef _BOYER_MOORE_H_
#define _BOYER_MOORE_H_

#include <cctype>
#include <cstddef>
#include <array>
#include <deque>
#include <map>
#include <vector>
#include <algorithm>

template <class T>
static std::map<T, std::vector<std::size_t>> RFunction(const std::vector<T> &patern) {
	std::map<T, std::vector<std::size_t>> result;
	const register std::size_t length = patern.size();
	for (std::size_t i = 0u; i < length; ++i) {
		result[patern[i]].push_back(i);
	}

	return result;
}

static const std::size_t UPPER = 0u, LOWER = 1u;

template <class T>
static std::array<std::vector<std::size_t>, 2u> LFunction(const std::vector<T> &patern) {
	const std::size_t length = patern.size();
	std::vector<std::size_t> n_func(length, 0u);
	for (register std::ptrdiff_t i = std::ptrdiff_t(length) - 2, l = std::ptrdiff_t(length) - 1, r = std::ptrdiff_t(length) - 1; i >= 0; --i) {
		if (i >= l) {
			n_func[i] = std::min(i - l + 1, std::ptrdiff_t(n_func[std::ptrdiff_t(length) - 1 - r + i]));
		}

		while (i - std::ptrdiff_t(n_func[i]) >= 0 && patern[length - 1u - n_func[i]] == patern[i - std::ptrdiff_t(n_func[i])]) {
			++n_func[i];
		}

		if (i - std::ptrdiff_t(n_func[i]) + 1 < l) {
			l = i - std::ptrdiff_t(n_func[i]) + 1;
			r = i;
		}
	}

	std::array<std::vector<std::size_t>, 2u> result({ std::vector<std::size_t>(length, 0u), std::vector<std::size_t>(length, 0u) });
	for (register std::size_t i = 0u, max = 0u; i < length - 1u; ++i) {
		if (n_func[i]) {
			result[UPPER][length - n_func[i]] = i;
		}

		if (n_func[i] > max && n_func[i] == i + 1u) {
			max = n_func[i];
		}
		result[LOWER][length - 1u - i] = max;
	}

	return result;
}

template <class T>
static std::size_t BadChar(const std::map<T, std::vector<std::size_t>> &r, const T &tChar, std::size_t pIdx) {
	typename std::map<T, std::vector<std::size_t>>::const_iterator elemMatch = r.find(tChar);
	if (elemMatch != r.cend()) {
		std::vector<std::size_t>::const_iterator newIdx = std::lower_bound(elemMatch->second.cbegin(), elemMatch->second.cend(), pIdx);
		if (newIdx != elemMatch->second.cbegin()) {
			--newIdx;
			return pIdx - *newIdx;
		}
	}

	return pIdx + 1u;
}

static std::size_t GoodSuff(const std::array<std::vector<std::size_t>, 2u> &l, register std::size_t pIdx) {
	const register std::size_t lLen = l[UPPER].size();
	if (pIdx < lLen - 1u) {
		if (l[UPPER][pIdx + 1u]) {
			return lLen - 1u - l[UPPER][pIdx + 1u];
		}
		else if (l[LOWER][pIdx + 1u]) {
			return lLen - l[LOWER][pIdx + 1u];
		}
		else {
			return lLen;
		}
	}

	return 1u;
}

static bool InputPatern(std::istream &in_stream, std::vector<std::string> &patern) {
	char ch;
	register std::size_t i = 0u;
	for (in_stream.get(ch); !in_stream.rdstate(); ++i) {
		while (std::isblank(ch) && (in_stream.get(ch), !in_stream.rdstate()));
		if (std::isspace(ch) || in_stream.rdstate()) {
			break;
		}
		for (patern.push_back(std::string("")); std::isalpha(ch) && !in_stream.rdstate(); in_stream.get(ch)) {
			patern[i].push_back(char(std::tolower(ch)));
		}
	}

	if (!patern.size() || in_stream.rdstate()) {
		return false;
	}

	return true;
}

static bool InputText(std::istream &in_stream, std::deque<std::pair<std::string, std::array<std::size_t, 2u>>> &text, const std::size_t count) {
	static char ch;
	static std::size_t lineIdx = 0u, wordIdx = 0u;
	register std::size_t i = 0u;
	for (; i < count && !in_stream.rdstate();) {
		while (in_stream.get(ch), std::isblank(ch) && !in_stream.rdstate());
		if (in_stream.rdstate()) {
			return false;
		}
		if (std::isspace(ch)) {
			++lineIdx;
			wordIdx = 0u;
			continue;
		}
		for (text.push_back(std::pair<std::string, std::array<std::size_t, 2u>>(std::string(""), std::array<std::size_t, 2u>({ lineIdx, wordIdx }))); std::isalpha(ch) && !in_stream.rdstate(); in_stream.get(ch)) {
			text.back().first.push_back(char(std::tolower(ch)));
		}
		++wordIdx;
		++i;
		in_stream.unget();
	}

	if (i != count) {
		return false;
	}

	if (text.size() > count) {
		for (register std::size_t j = 0u; j < count; ++j) {
			text.pop_front();
		}
	}

	return true;
}

static const std::size_t LINE_IDX = 0u, WORD_IDX = 1u;

void BoyerMoore(std::istream &in_stream, std::ostream &out_stream) {
	bool local_result;

	std::vector<std::string> patern;
	local_result = InputPatern(in_stream, patern);
	if (!local_result) {
		return;
	}
	register const std::size_t pLen = patern.size();

	const std::map<std::string, std::vector<std::size_t>> r = RFunction(patern);
	const std::array<std::vector<std::size_t>, 2u> l = LFunction(patern);

	std::deque<std::pair<std::string, std::array<std::size_t, 2u>>> text;
	for (register std::size_t shift = pLen;;) {
		local_result = InputText(in_stream, text, shift);
		if (!local_result) {
			return;
		}

		std::deque<std::pair<std::string, std::array<std::size_t, 2u>>>::iterator tIdx = --text.end();
		register std::ptrdiff_t pIdx = ptrdiff_t(pLen) - 1;
		for (; pIdx >= 0 && patern[pIdx] == tIdx->first; --pIdx, --tIdx);
		if (pIdx < 0) {
			out_stream << text.begin()->second[LINE_IDX] + 1u << ", " << text.begin()->second[WORD_IDX] + 1u << "\n";
			shift = pLen - l[LOWER][1u];
		}
		else {
			std::size_t bad_char = BadChar(r, tIdx->first, std::size_t(pIdx));
			std::size_t good_suff = GoodSuff(l, std::size_t(pIdx));
			shift = std::max(bad_char, good_suff);
		}
	}
}

#endif