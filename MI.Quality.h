#pragma once

#include "Base/MI.DoubleEq.h"
#include "Common/MI.AngleBetweenNormals.h"
#include "Container/MI.Matrix.h"
#include "Mesh/MI.IsDegenerated.h"
#include "Mesh/MeshElement/MI.Quad.h"
#include "Mesh/MeshElement/MI.Triangle.h"

// Качество элемента (mean ratio quality).
// =======================================
//
// - R² или R2 - 2D сетка.
// - R³ или R3 - 3D сетка.
// - E := (p1, ..., p|E|) обозначает элемент с |E| узлами, которые могут быть треугольными или четырехугольными в R²,
//   или тетраэдрический, шестигранный, пирамидальный или призматический в R³. Каждая вершина p(i) ∈ Rm, m ∈ {2, 3},
//   любого элемента соединена ребрами с m другими вершинами p(j) того же элемента.
// - [Ntri, Nquad, Ntet, Nhex] - Симплекс узел - симплекс, который определяют каждый узел и его реберно-связные соседи.
// - |N| - Число симплексов узлов, т.е. количество элементов в кортеже N.
// - p(i) - i-ая вершина элемента.
// - k - номер элемента. tri == 0, quad == 1, tet == 2, hex == 3.
// - m - кол-во векторов в симплекс узле (2 для R2, 3 для R3).
// - W - Весовая матрица. Матрица симплекс узла для идеального элемента с длиной сторон равной 1.
//
// Рис. 1: https://disk.yandex.ru/i/7fCrqZlYqoiDQA
// ===============================================
//
// Даны примеры таких симплексов, связанных с pис. 1, которые отмечены серым цветом для всех типов элементов. Здесь
// данные элементы являются правильными, и первые два узла были установлены в
// - p1 = (0, 0)    и p2 = (1, 0),    при m = 2.
// - p1 = (0, 0, 0) и p2 = (1, 0, 0), при m = 2.
//
// В качестве предварительного условия определения критерия качества среднего отношения должны быть определены
// конкретные условия достоверности элемента. Валидность элемента также будет оцениваться на основе симплекс-узлов,
// которые сначала будут отображаться более точно. В дальнейшем элемент определенного типа будет обозначаться
// как Etri, Equad, Etet, Ehex соответственно. Индексы симплекс-узлов для соответствующего типа
// элемента задаются кортежами:
// Ntri  := {(0, 1, 2)},
// Nquad := {(0, 1, 3), (1, 2, 0), (2, 3, 1), (3, 0, 2)},
// Ntet  := {(0, 1, 2, 3)},
// Nhex  := {(0, 3, 4, 1), (1, 0, 5, 2), (2, 1, 6, 3), (3, 2, 7, 0),
//           (4, 7, 5, 0), (5, 4, 6, 1), (6, 5, 7, 2), (7, 6, 4, 3)}.
//
// В этом представлении k-й элемент N ∈ {Ntri, Nquad, Ntet, Nhex} представляет m + 1 узловых индексов N(k, i),
// i ∈ {0, ..., m} симплекс-узла, связанного с p(k). Следует отметить, что в случае Etri и Etet каждый из m + 1
// узловых симплексов представляет сам элемент. Следовательно, достаточно использовать только симплекс узла, связанный с
// p , что приводит к |Ntri1| == |Ntet| == 1. Следовательно, для треугольного и тетраэдрического элементов справедливо
// выражение |E| != |N|.
//
// Определение 1 (Действительность элемента).
// ==========================================
//
// Пусть E обозначает произвольный элемент, взятый из {Etri, Equad, Etet, Ehex} со связанным с ним кортежем симплексов
// узлов, заданным N ∈ {Ntri, Nquad, Ntet, Nhex}. Для каждого узла pk, k ∈ {0, ..., |N-1|}, пусть:
// - Tk    := pN(k, 1), ..., pN(k, m + 1) - симплекс-узел и
// - D(Tk) := (pN(k, 2) − pN(k, 1), ..., pN(k, m + 1) − pN(k, 1)) - матрица m x m ее остовных краевых векторов.
//   Элемент называется действительным, если det(D(Tk)) > 0. выполняется для всех k ∈ {0, ..., |N-1|}.
//   В противном случае он называется недействительным.
//
// Определение 2 (Качество элемента).
// ==================================
//
// Среднее качество элемента задается формулой:
//
//         1    |N| m * det(Sk) ^ (2. / m)
// q(E) = --- *  ∑  ----------------------,  (1)
//        |N|   k=1    trace(Skt * Sk)
//
// где:
// - Sk = D(Tk) * W ^ (-1),
// - trace(Skt * Sk) - это сумма квадратов всех элементов (квадрат нормы Фробениуса матрицы Sk),
// - E - произвольный допустимый элемент, взятый из {Etri, Equad, Etet, Ehex}
// - N - связанный с E кортеж индексов узловых тетраэдров Tk ∈ {Ntri, Nquad, Ntet, Nhex} в соответствии с рис. 1.
//
// Качество элемента q(E) ∈ (0, 1], со значениями близкими к нулю, представляющими элементы низкого качества,
// и q(E) = 1, представляющими идеальные регулярные элементы.
// q(E) == 0. говорит о том, что элемент недействительный.
// Следует также отметить, что знаменатель в (1) представляет квадрат нормы Фробениуса матрицы Sk. Здесь и далее
// максимальное количество недействительных элементов в сетке устанавливается равным нулю.
//
// Каждая узловая координата элемента представляет собой степень свободы. Следовательно, представляя один элемент
// вещественным числом качества, очевидно, что разные элементы, имеющие одно и то же качество, могут отличаться
// по форме.
//
// Если элемент недействительный (используется "MI is_degenerated"), то сразу возвращается 0.0.
//
// Весовые матрицы.
// ================
//
// Весовая матрица - это три вектора для одного симплекс-узла идеального элемента.
// Длина каждого вектора должна быть равной единице (В ВЕСОВОЙ МАТРИЦЕ).
//
// Возьмем элемент,            Симплекс узел
// например quad.              для вершины 'A'.
//
// D     C                     D
//  +---+                       +
//  |\  |                       ^\
//  | \ |                       | \
//  |  \|                       |  \
//  +---+                       +-->+
// A     B                     A     B
//
// v1 = B - A;
// v2 = D - A;
//
// w1 = | v1.x, v2.x |
//      | v1.y, v2.y |
//
// Для 2d элементов количество векторов у симплекс узла должно быть 2 (m = 2).
// Для 3d элементов количество векторов у симплекс узла должно быть 3 (m = 3).
// (НО!) Тогда все 2d элементы должны лежать в плоскости 'z', но наша сетка не может быть не плоской, элементы не
// будут лежать в плоскости 'z'. Тогда весовая матрица будет (НЕ) квадратной, потому что нам нужно будет использовать
// все координаты (x, y, z).
//
// w2 = | v1.x, v2.x |
//      | v1.y, v2.y |
//      | v1.z, v2.z |
//
// С такой матрицей нельзя будет работать в дальнейшем.
// Решить эту проблему можно несколькими способами:
// - Спроецировать элемент на плоскость 'z' и использовать w1.
// - Добавить третий вектор (v3), который будет перпендикулярным к v1 и к v2
//    и иметь длину v3.len == (v1.len + b2.len) / 2.
//    Тогда матрица будет квадратной:
//
//    w3 = | v1.x, v2.x, v3.x |
//         | v1.y, v2.y, v3.y |
//         | v1.z, v2.z, v3.z |
//
// Я решил выбрать второй способ, потому что quad может быть искривленным, тогда спроецировать его на плоскость 'z'
// никак не получится.
//
// Весовые матрицы для основных элементов:
// ---------------------------------------
//
// (a) Triangle:
//     w = | 1.,  1. / 2. |
//         | 0., √3. / 2. |
//
// (b) Quad:
//     w = | 1., 0. |
//         | 0., 1. |
//
// (c) Tetrahedron:
//     w = |1.,  1. / 2.,   1. / 2. |
//         |0., √3. / 2.,  √3. / 6. |
//         |0.,       0., √2. / √3. |
//
// (d) Hexahedron:
//     w = | 1., 0., 0. |
//         | 0., 1., 0. |
//         | 0., 0., 1. |
//
//
namespace mi {

// Вокруг оси X.
MI_NODISCARD inline MI point3d rotate_x(const MI point3d& vector, const double angle_radian) {
  const double x     = vector.x();
  const double y     = vector.y();
  const double z     = vector.z();
  const double cos_a = cos(angle_radian);
  const double sin_a = sin(angle_radian);

  MI point3d res = {x,                      //
                    y * cos_a - z * sin_a,  //
                    y * sin_a + z * cos_a};

  const double n1 = res.squared_euclidean_norm();
  const double n2 = vector.squared_euclidean_norm();

  if (n1 > 0.) {
    return res = res * (n2 / n1);
  }

  return res;
}

// Вокруг оси Y.
MI_NODISCARD inline MI point3d rotate_y(const MI point3d& vector, const double angle_radian) {
  const double x     = vector.x();
  const double y     = vector.y();
  const double z     = vector.z();
  const double cos_a = cos(angle_radian);
  const double sin_a = sin(angle_radian);

  MI point3d res = {x * cos_a + z * sin_a,  //
                    y,                      //
                    -x * sin_a + z * cos_a};

  const double n1 = res.squared_euclidean_norm();
  const double n2 = vector.squared_euclidean_norm();

  if (n1 > 0.) {
    return res = res * (n2 / n1);
  }

  return res;
}

// Вокруг оси Z.
MI_NODISCARD inline MI point3d rotate_z(const MI point3d& vector, const double angle_radian) {
  const double x     = vector.x();
  const double y     = vector.y();
  const double z     = vector.z();
  const double cos_a = cos(angle_radian);
  const double sin_a = sin(angle_radian);

  MI point3d res = {x * cos_a - y * sin_a,  //
                    x * sin_a + y * cos_a,  //
                    z};

  const double n1 = res.squared_euclidean_norm();
  const double n2 = vector.squared_euclidean_norm();

  if (n1 > 0.) {
    const double coeff = n2 / n1;
    return res         = res * coeff;
  }

  return res;
}

// Повернуть вектор вокруг трех осей.
MI_NODISCARD inline MI point3d rotate_xyz(const MI point3d& vector,
                                          const double      angle_radian_x,
                                          const double      angle_radian_y,
                                          const double      angle_radian_z) {
  return rotate_z(rotate_y(rotate_x(vector, angle_radian_x), angle_radian_y), angle_radian_z);
}

// Возвращает матрицу перехода для некоторого вектора из Декартовой системы координат в новый базис.
MI_NODISCARD inline MI matrix3d get_transition_matrix(const MI point3d& new_axis_x,
                                                      const MI point3d& new_axis_y,
                                                      const MI point3d& new_axis_z) {
  return matrix3d{
    {new_axis_x.x(), new_axis_x.y(), new_axis_x.z()},
    {new_axis_y.x(), new_axis_y.y(), new_axis_y.z()},
    {new_axis_z.x(), new_axis_z.y(), new_axis_z.z()}
  };
}

// Переносит вектор из Декартовой системы координат в новый базис с помощью матрицы перехода.
//
// v - vector.
// ax - axis_x.
// ay - axis_y.
// az - axis_z.
//
//  | ax.x, ax.y, ax.z |   | v.x |
//  | ay.x, ay.y, ay.z | * | v.y | = {v.x', v.y', v.z'}
//  | az.x, az.y, az.z |   | v.y |
//
MI_NODISCARD inline MI point3d transition_another_basis(const MI matrix3d& transition_matrix,
                                                        const MI point3d&  vector) {
  return transition_matrix * vector;
}

MI_NODISCARD inline MI static_vector<MI point3d, 3> transfer_to_plane_z(const MI point3d& v0,
                                                                        const MI point3d& v1,
                                                                        const MI point3d& v2) {
  const MI point3d vec0 = (v1 - v0).normalized();
  const MI point3d vec1 = (v2 - v0).normalized();

  // Новый базис

  const MI point3d& axis_x = vec0;
  const MI point3d  axis_z = axis_x.cross(vec1).normalized();
  const MI point3d  axis_y = axis_z.cross(axis_x).normalized();

  const MI matrix3d transition_matrix = get_transition_matrix(axis_x, axis_y, axis_z);

  MI static_vector<MI point3d, 3> new_vertices;

  new_vertices.emplace_back(transition_another_basis(transition_matrix, v0));
  new_vertices.emplace_back(transition_another_basis(transition_matrix, v1));
  new_vertices.emplace_back(transition_another_basis(transition_matrix, v2));

  return {
    {transition_another_basis(transition_matrix, v0),
     transition_another_basis(transition_matrix, v1),
     transition_another_basis(transition_matrix, v2)}
  };
}

// ---------------------------------------------------------------------------------------------------------------------
// for triangle.

MI_NODISCARD inline double quality(const MI point3d& v0, const MI point3d& v1, const MI point3d& v2) {
  double new_quality = 0.;

  // (a) Triangle:
  //     w = | 1.,  1. / 2. |
  //         | 0., √3. / 2. |

  constexpr MI matrix2d w = {
    {1.,                                0.5},
    {0., 0.86602540378443864676372317075294}
  };

  constexpr size_t mid   = 0;
  constexpr size_t left  = 2;
  constexpr size_t right = 1;

  const MI static_vector<MI point3d, 3> rotated_vertices = MI transfer_to_plane_z(v0, v1, v2);

  MI point3d vec1 = rotated_vertices[right] - rotated_vertices[mid];
  MI point3d vec2 = rotated_vertices[left] - rotated_vertices[mid];

  // D(Tk) - Это матрица для симплекс-узла.
  const MI matrix2d dtk = {
    {vec1.x(), vec2.x()},
    {vec1.y(), vec2.y()}
  };

  // Следует из определения 2.
  MI_CHECK(dtk.determinant() > 0.);

  constexpr size_t m               = 2;
  const auto       sk              = dtk * w.inverse();
  const auto       numerator       = m * STD pow(sk.determinant(), 2. / m);
  const auto       denominator     = sk.squared_euclidean_norm();
  const auto       current_quality = numerator / denominator;

  new_quality += current_quality;

  MI_CHECK(new_quality > 0. && new_quality <= 1.);

  return new_quality;
}

template<class Property, class Mesh>
MI_NODISCARD double quality(const MI triangle_with<Property>& triangle, const Mesh& mesh) {
  return quality(mesh.get_vertex(triangle.get_global_index(0)),
                 mesh.get_vertex(triangle.get_global_index(1)),
                 mesh.get_vertex(triangle.get_global_index(2)));
}

// ---------------------------------------------------------------------------------------------------------------------
// for quad.

template<class AnyProperty, class MeshType>
MI_NODISCARD double quality(const MI quad_with<AnyProperty>& element, const MeshType& mesh) {
  // Если элемент будет частично или полностью вырожден в прямую, то сработает MI_CHECK, который проверяет определитель
  // матрицы.
  // Если элемент будет вогнутым, то алгоритм отработает без ошибок, поэтому проверим это принудительно.
  MI_CHECK(MI internal::is_curved(element, mesh));

  double new_quality = 0.;

  constexpr MI matrix2d w = {
    {1., 0.},
    {0., 1.}
  };

  for (size_t n_node = 0; n_node < MI quad::n_vertices(); ++n_node) {
    const size_t mid   = MI quad::simplex_node(n_node).mid();
    const size_t left  = MI quad::simplex_node(n_node).left();
    const size_t right = MI quad::simplex_node(n_node).right();

    const MI static_vector<MI point3d, 3> rotated_vertices =
      MI transfer_to_plane_z(mesh.get_vertex(element.global_index(left)),
                             mesh.get_vertex(element.global_index(mid)),
                             mesh.get_vertex(element.global_index(right)));

    // Вектор от центра симплекс узла до правого края
    MI point3d v1 = rotated_vertices[2] - rotated_vertices[1];

    // Вектор от центра симплекс узла до левого края
    MI point3d v2 = rotated_vertices[0] - rotated_vertices[1];

    // D(Tk) - Это матрица для симплекс-узла.
    const MI matrix2d dtk = {
      {v1.x(), v2.x()},
      {v1.y(), v2.y()}
    };

    // Следует из определения 2.
    MI_CHECK(dtk.determinant() > 0.);

    constexpr size_t m               = 2;
    const auto       sk              = dtk * w.inverse();
    const auto       numerator       = m * STD pow(sk.determinant(), 2. / m);
    const auto       denominator     = sk.squared_euclidean_norm();
    const auto       current_quality = numerator / denominator;

    new_quality += current_quality;
  }

  new_quality /= 4.;

  MI_CHECK(new_quality > 0. && new_quality <= 1.);

  return new_quality;
}

// Качество является валидным, если quality ∈ (0.0; 1.0].
// Качество равное 0.0 означает, что элемент не прошел проверку на валидность.
template<class Mesh, class Element>
MI_NODISCARD bool is_valid_quality(const Mesh& mesh, const Element& element) {
  const double quality = MI quality(element, mesh);

  return quality > 0. && quality <= 1.;
}
}  // namespace mi
