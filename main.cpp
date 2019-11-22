#include <cstdio>
#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cmath>
#include <cstdlib> 
#include <chrono>

namespace NConst {
	const short BUF_SIZE = 2048+1; // максимальная длина строки (по заданию) +1 - для признака окончания строки
}

struct TMyStructure { // структура элемента сортируемых данных 
	double Key;		// ключ 
	int StrId;		// вместо строки при сортировке используем идентификатор (для экономии времени на копировании строк)
};

// ------------вектор с гитхаба (от преподавателя) без комментариев-----------
template <typename T>
class vector {
public:
	using value_type = T;
	using iterator = value_type * ;
	using const_iterator = const value_type*;

	vector() :
		already_used_(0), storage_size_(0), storage_(nullptr)
	{
	}

	vector(int size, const value_type& default_value = value_type()) :
		vector()
	{
		assert(size >= 0);

		if (size == 0)
			return;

		already_used_ = size;
		storage_size_ = size;
		storage_ = new value_type[size];

		std::fill(storage_, storage_ + already_used_, default_value);
	}

	int size() const
	{
		return already_used_;
	}

	bool empty() const
	{
		return size() == 0;
	}

	iterator begin() const
	{
		return storage_;
	}

	iterator end() const
	{
		if (storage_)
			return storage_ + already_used_;

		return nullptr;
	}

	friend void swap(vector& lhs, vector& rhs)
	{
		using std::swap;

		swap(lhs.already_used_, rhs.already_used_);
		swap(lhs.storage_size_, rhs.storage_size_);
		swap(lhs.storage_, rhs.storage_);
	}

	vector& operator=(vector other)
	{
		swap(*this, other);
		return *this;
	}

	vector(const vector& other) :
		vector()
	{
		vector next(other.storage_size_);
		next.already_used_ = other.already_used_;

		if (other.storage_)
			std::copy(other.storage_, other.storage_ + other.storage_size_,
				next.storage_);

		swap(*this, next);
	}

	~vector()
	{
		delete[] storage_;

		storage_size_ = 0;
		already_used_ = 0;
		storage_ = nullptr;
	}

	void push_back(const value_type& value)
	{
		if (already_used_ < storage_size_) {
			storage_[already_used_] = value;
			++already_used_;
			return;
		}

		int next_size = 1;
		if (storage_size_)
			next_size = storage_size_ * 2;

		vector next(next_size);
		next.already_used_ = already_used_;

		if (storage_)
			std::copy(storage_, storage_ + storage_size_, next.storage_);

		next.push_back(value);
		swap(*this, next);
	}

	const value_type& at(int index) const
	{
		if (index < 0 || index > already_used_)
			throw std::out_of_range("You are doing this wrong!");

		return storage_[index];
	}

	value_type& at(int index)
	{
		const value_type& elem = const_cast<const vector*>(this)->at(index);
		return const_cast<value_type&>(elem);
	}

	const value_type& operator[](int index) const
	{
		return at(index);
	}

	value_type& operator[](int index)
	{
		return at(index);
	}

private:
	int already_used_;
	int storage_size_;
	value_type* storage_;
};
// ----------------- конец вектора ------------------------------


// сортировка внутри корзины
void inSort(vector<TMyStructure> &a) {
	if (a.size() <= 1) {
		return;
	}
	for (int i = 1; i < a.size(); i++)
		for (int j = i; j > 0 && a[j - 1].Key > a[j].Key; j--) 
		{
			std::swap(a[j - 1].Key, a[j].Key);
			std::swap(a[j - 1].StrId, a[j].StrId);
		}
}

// корзинная сортировка
void BacketSort(vector<TMyStructure> &mas) {
	if (mas.size() <= 1) { // сортировать имеет смысл только 2 и более элементов
		return;
	}

	const double MIN_VALUE = -100; // минимум возможных сортируемых значений
	const double MAX_VALUE = 100; // максимум возможных сортируемых значений
	const int BUCKETS = 272;     // количество корзин

	 
	vector<TMyStructure> b[BUCKETS]; // массив из векторов (каждый вектор - это корзина)

	 
		for (int i = 0; i < mas.size(); i++){ 
			int bi = (BUCKETS-1) * ((mas[i].Key + (abs)(MIN_VALUE)) / (MAX_VALUE - MIN_VALUE)); 
			b[bi].push_back(mas[i]); // заполняем корзины данными из исходного вектора
		}
	 
		for (int i = 0; i < BUCKETS; i++) {	// содержимое каждой корзины 
			inSort(b[i]);					// отсортируем
		}
		
		int index = 0;
		for (int i = 0; i < BUCKETS; i++) 		  // последовательно переносим
			for (int j = 0; j < b[i].size(); j++)//  содержимое отсортированных  векторов (корзин)
				mas[index++] = b[i][j]; 		//   в первоначальный массив
}
// конец корзинной сортировки

//* генераторы случайных ключей и строк (для тестирования)
double getRandomNumber(int min, int max)
{
	static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
	return static_cast<double>(rand() * fraction * (max - min + 1) + min);
}

char* getRandomString(char* s, const int len) {
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	for (int i = 0; i < len; ++i) {
		s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
	}

	s[len] = '\0';
	return s;
}
//*/




int main()
{
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	vector<TMyStructure> arr; //введенные ключи и идентификаторы строк будут храниться в векторе arr
	char str[NConst::BUF_SIZE];
	vector<char *> strings;// введенные строки будут храниться в векторе strings
	TMyStructure s;
	int strId = 0;

	/*while (std::cin >> s.Key >> str) {	//ввод данных, ключ помещается в структуру
		strings.push_back(strdup(str)); //строка добавляется  в конец вектора strings
		s.StrId = strId;				//порядковый номер строки в векторе strings помещается в структуру
		arr.push_back(s);//заполненная структура помещается в конец вектора arr
		++strId;
	}*/
	int count;
	std::cin>>count;
	for(int i = 0;i<count;i++){
		s.Key = getRandomNumber(-99, 99);
		getRandomString(str, 1024);
		strings.push_back(strdup(str));
		s.StrId = strId;
		arr.push_back(s);
		++strId;

	}
	auto t1 = std::chrono::high_resolution_clock::now();
	BacketSort(arr);	// сортируем arr корзинной сортировкой
	auto t2 = std::chrono::high_resolution_clock::now();
	std::cout << std::setprecision(6) << std::fixed;
	for (size_t i = 0; i < arr.size(); i++) {// последовательно выводим элементы отсортированного вектора
		printf("%.6f\t%s\n", arr[i].Key, strings[arr[i].StrId]); // ключ и строку по StrId
	}
	for (int i = 0; i < strings.size(); ++i) {
		free(strings[i]); // удаляем строки из памяти
	}
	auto res = std::chrono::duration_cast<std::chrono::nanoseconds>(t2-t1);
	std::cout<<"res = "<< res.count() / 1e9 <<'\n';
	return 0;
}