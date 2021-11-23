#pragma once

#include <cmath>
#include <random>
#include <chrono>
#include <type_traits>
#include <iostream>

#include "../../src/Matrix.h"
#include "../../src/Vector.h"

using namespace bm;

// TODO: make same api for matrix, probably move to common.h
template <int N, typename ToFill>
ToFill& fillRand(ToFill& toFill, float min = -1.0f, float max = 1.0f) {
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::normal_distribution<float> distribution((min + max) / 2.0f, max);

	for (int i = 0; i < N; ++i) { toFill[i] = distribution(generator); }

	return toFill;
};

template <int N>
struct BlackBox {

	BlackBox() {
		float const precision = 1e-5f;
		float det = 0;
		do {
			for (int i = 0; i < N; ++i) fillRand<N>(m_A.row(i), -10, 10);
			det = m_A.det();
		} while (det < precision && det > -precision);
		fillRand<N>(m_B);
	}


	Vector<N, float> get(Vector<N, float> const& in) const {
		return m_A * in + m_B;
	}

private:

	Matrix<N, N, float> m_A;
	Vector<N, float> m_B;

};

template <int N>
struct RandomInBlackBox {

	RandomInBlackBox() {
		float const precision = 1e-5f;
		float det = 0;
		do {
			for (int i = 0; i < N; ++i) fillRand<N>(m_A.row(i));
			det = m_A.det();
		} while (det < precision && det > -precision);
		fillRand<N>(m_B);
	}


	void get(Vector<N, float> & in, Vector<N, float> & out) const {
		// TODO: add copy assigment operator
		auto res = m_A * fillRand<N>(in) + m_B;
		for (int i = 0; i < N; ++i) out[i] = res[i];
	}

private:

	Matrix<N, N, float> m_A;
	Vector<N, float> m_B;

};
