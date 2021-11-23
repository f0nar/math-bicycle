#pragma once
#include "BlackBox.h"
#include "../../src/Matrix.h"
#include "../../src/Vector.h"

using namespace bm;

template <int N>
struct BlackBoxModel {

	explicit BlackBoxModel(BlackBox<N> const& bbox) {
		Vector<N, float> input_vec;
		auto B = bbox.get(input_vec);
		for (int i = 0; i < N; ++i) {
			m_B[i] = B[i];
		}
		for (int i = 0; i < N; ++i) {
			input_vec[i] = 1.0f;

			auto out_vec = bbox.get(input_vec);
			for (int j = 0; j < N; ++j) {
				m_A.at(j, i) = out_vec[j] - m_B[j];
			}

			input_vec[i] = 0.0f;
		}
	}

	explicit BlackBoxModel(RandomInBlackBox<N> const& bbox) {
		Matrix<N + 1, N + 1, float> extended_in_vectors_mat;
		Matrix<N + 1, N + 1, float> extended_out_vectors_mat;
		
		for (int i = 0; i < N + 1; ++i) {
			Vector<N, float> in, out;
			bbox.get(in, out);
			for (int j = 0; j < N; ++j) {
				// TODO: vector assigment constructor for row
				extended_in_vectors_mat.at(j, i) = in.at(j);
				extended_out_vectors_mat.at(j, i) = out.at(j);
			}
		}

		for (int i = 0; i < N + 1; ++i) {
			// TODO: add fill method to row
			extended_in_vectors_mat.at(N, i) = extended_out_vectors_mat.at(N, i) = 1.0f;
		}

		auto res = extended_out_vectors_mat * extended_in_vectors_mat.inv();

		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				m_A.at(i, j) = res.at(i, j);
			}
		}
		for (int j = 0; j < N; ++j) {
			m_B[j] = res.at(N, j);
		}
	}

private:

	Matrix<N, N, float> m_A;
	Vector<N, float> m_B;

};