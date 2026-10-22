#pragma once
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <vector>

// Matriz 2D genérica guardada en un solo vector contiguo.
// Se accede con at(x, y): x = columna, y = fila.
// OJO: no uses Grid<bool> (vector<bool> es especial); usa Grid<uint8_t>.
template <typename T>
class Grid {
public:
    Grid() = default;
    Grid(int width, int height, const T& fillValue = T{})
        : m_w(width), m_h(height), m_data(static_cast<size_t>(width) * height, fillValue) {}

    int  width()  const { return m_w; }
    int  height() const { return m_h; }
    bool empty()  const { return m_data.empty(); }
    bool inBounds(int x, int y) const { return x >= 0 && y >= 0 && x < m_w && y < m_h; }

    T& at(int x, int y) {
        assert(inBounds(x, y));
        return m_data[static_cast<size_t>(y) * m_w + x];
    }
    const T& at(int x, int y) const {
        assert(inBounds(x, y));
        return m_data[static_cast<size_t>(y) * m_w + x];
    }

    void fill(const T& value) { std::fill(m_data.begin(), m_data.end(), value); }
    const std::vector<T>& data() const { return m_data; }

private:
    int m_w = 0;
    int m_h = 0;
    std::vector<T> m_data;
};
