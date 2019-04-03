#pragma once
#include <cstddef> 
#include <stdexcept>

class Matrix
{
	//helper class for [][] - represent the row
	class MatrixRow
	{
	private:
		size_t size;
		int *data;

		friend  Matrix;
		//private constructor to block creation of this object outside the matrix class )
		MatrixRow(size_t size, int *data): data(data), size(size) 
		{

		}
	public:
		int& operator[](size_t c)
		{
			if (c >= size)
				throw std::out_of_range("");
			return data[c];
		}
		const int& operator[](size_t c) const
		{
			if (c >= size)
				throw std::out_of_range("");
			return data[c];
		}
	};

	private:
		size_t rows;
		size_t cols;
		int **data;

	public:
		Matrix(size_t rows, size_t cols): rows(rows), cols(cols) 
		{
			if(rows>0 && cols>0)
			{
				data = new int*[rows];
				for (size_t i = 0; i < rows; ++i) 
				{
					data[i] = new int[cols]();
				}
			}
		}

		size_t getRows() const
		{ 
			return rows; 
		}
		size_t getColumns() const
		{
			return cols; 
		}

		MatrixRow operator[](size_t r)
		{
			if (r >= rows)
			{
				throw std::out_of_range("");
			}
			return MatrixRow(cols, data[r]);
		}

		const MatrixRow operator[](size_t r) const
		{
			if (r >= rows)
			{
				throw std::out_of_range("");
			}
			return MatrixRow(cols, data[r]);
		}

		bool operator==(const Matrix &m) const
		{
			if(m.cols == cols && m.rows == rows)
			{
				bool flag(true);
				for(size_t i=0;i<rows;++i)
				{
					for(size_t j=0;j<cols;++j)
					{
						if(m.data[i][j]!=data[i][j])
						{
							flag=false;
							break;
						}
					}
				}
				return flag;
			}
			else
				return false;

		}

		bool operator!=(const Matrix& m) const 
		{ 
			return !(m == *this); 
		}

		Matrix& operator*=(int a)
		{
			for (size_t i = 0; i < rows; ++i)
			{
				for (size_t j = 0; j < cols; ++j)
				{
					data[i][j] *= a;
				}
			}
			return *this;
		}

		Matrix& operator+=(const Matrix& m)
		{
			if(m.cols != cols && m.rows != rows)
				throw std::out_of_range("");
			for (size_t i = 0; i < rows; ++i)
			{
				for (size_t j = 0; j < cols; ++j)
				{
					data[i][j] += m.data[i][j];
				}
			}
			return *this;
		}

		~Matrix() 
		{
			if(rows>0 && cols>0)
			{
				for (size_t i = 0; i < rows; i++) 
				{
					delete[] data[i];
				}
				delete[] data;
			}
		}
	

};