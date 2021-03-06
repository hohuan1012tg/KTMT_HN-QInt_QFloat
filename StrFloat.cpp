﻿#include "StrFloat.h"

//Cân bằng độ dài từng phần của a và b: trước và sau dấu phẩy.
void balanceLength(string &a, string &b)
{
	int i;

	//Tínhh số chữ số trước dấu phẩy của a và b. //Calculate figure of numbers before dot in string a and b.
	int bfa = 0, bfb = 0;
	for (i = 0; i < a.length() && a[i] != '.'; i++)
		bfa++;
	for (i = 0; i < b.length() && b[i] != '.'; i++)
		bfb++;

	//Thêm "0" vào đầu của a và b sao cho số lượng số bằng nhau.
	string tmp = "0";
	for (bfa = bfa; bfa < bfb; bfa++) a.insert(0, tmp);
	for (bfb = bfb; bfb < bfa; bfb++) b.insert(0, tmp);

	//Tínhh số chữ số sau dấu phẩy của a và b. //Calculate figure of numbers after dot in string a and b.
	bfa = bfb = 0;
	for (i = a.length() - 1; i >= 0 && a[i] != '.'; i--)
		bfa++;
	for (i = b.length() - 1; i >= 0 && b[i] != '.'; i--)
		bfb++;

	//Thêm "0" vào đầu của a và b sao cho số lượng số bằng nhau. Trường hợp nếu a và b không có số sau dấu phẩy thì bfa=bfb -> vẫn đúng. //Cần <=AFTER_DOT?
	for (bfa = bfa; bfa < bfb && bfa <= AFTER_DOT; bfa++) a.push_back('0');
	for (bfb = bfb; bfb < bfa && bfb <= AFTER_DOT; bfb++) b.push_back('0');
}

//-----------------------Class-------------------------------------------
StrFloat::StrFloat()
{
	m_float = "";
	m_negative = false;
}

//Chuyển từ sâu p về StrFloat.
StrFloat::StrFloat(string p)
{
	m_float = p;
	normalize(); //Chuẩn hóa m_float.
}

StrFloat::~StrFloat()
{
}

//Kiểm tra số âm. True: âm; False: dương.
bool StrFloat::isNegative()
{
	return (m_negative);
}

//Nếu số âm thì loại bỏ dấu và gán m_negative=true. Loại bỏ số "0" ở đầu. Chuẩn hóa số 0. // If number is negative, eliminate sign, m_negative=true. Normalize the number 0.
void StrFloat::normalize()
{
	//Kiểm tra số âm. //Check negative
	if (m_float[0] == '-') {
		m_negative = true;
		m_float.erase(0, 1); //Loại bỏ dấu.
	}
	else m_negative = false;

	//Giữ số lượng số sau dấu phẩy bằng AFTER_DOT = 35. //Keep the number after dot equal to AFTER_DOT
	int afd = 0; //Lưu số lượng số sau dấu phẩy.
	//Đếm số lượng số sau dấu phẩy -> afd. //Count how many numbers after dot -> afd
	int i = m_float.length() - 1;
	while (i > -1 && m_float[i] != '.') {
		i--;
		afd++;
	}
	if (i == -1) { //Nếu như i==-1 nghĩa là số không tồn tại số sau dấu phẩy. Thêm số '0' để đủ AFTER_DOT số sau dấu phẩy. //The number doesn't have . and after dot, so adding '0' to fill this.
		m_float.push_back('.'); //Thêm '.' 
		for (int j = 1; j <= AFTER_DOT; j++) m_float.push_back('0'); //Thêm 0.
	}
	else
		if (afd < AFTER_DOT) //Thêm '0'. // Add '0' to fill it
			for (int j = afd + 1; j <= AFTER_DOT; j++) m_float.push_back('0');
		else
			while (afd > AFTER_DOT) { //Xóa bớt số để cho đủ AFTER_ DOT. //Eliminate the numbers afer AFTER_DOT
				afd--;
				m_float.erase(m_float.length() - 1, 1);
			}

	//Xóa số 0 ở đâu cho đến khi gặp "0.0...." //Erase the number 0 at header until it is !'0' or a '0'.
	while (m_float[0] == '0' && m_float[1] != '.') m_float.erase(0, 1);

	//Kiểm tra trường hợp "-0". //Check the situation "-0"
	for (i = 0; i < m_float.length(); i++)
		if (m_float[i] != '0' && m_float[i] != '.') return;
	//Nếu là trường hợp "-0" thì bỏ dấu.
	m_negative = false;
}

//Chỉ cộng 2 số dương. //Only do with two positive numbers
StrFloat StrFloat::operator+(const StrFloat & p)
{
	StrFloat tmp; //Lưu kết quả trả về.
	string a = m_float, b = p.m_float;
	//Cân bằng số lượng số giữa a và b ở 2 phần: trước và sau dấu phẩy. //Balance the length of number after and before dot between a and b;
	balanceLength(a, b);

	//Thực hiện phép cộng
	int i, j = 0;
	for (i = a.length() - 1; i >= 0; i--)
		if (a[i] != '.') //Nếu kí tự khác "." thì thực hiện cộng.
		{
			int sp = a[i] + b[i] - '0' - '0' + j;
			if (sp > 9) j = 1;
			else j = 0;
			sp %= 10;
			tmp.m_float.push_back(sp + '0');
		}
		else tmp.m_float.push_back('.');

		while (j > 0)
		{
			tmp.m_float.push_back(j % 10 + '0');
			j = j / 10;
		}
		reverse(tmp.m_float.begin(), tmp.m_float.end());//Đảo ngược chuỗi cho đúng thứ tự. //Reverse the arrangement of the number 
		tmp.normalize(); //Chuẩn hóa kết quả.
		return tmp;
}

StrFloat & StrFloat::operator=(StrFloat & p)
{
	m_float = p.m_float;
	m_negative = p.m_negative;
	return (*this);
}

//Chia xâu m_float cho số chia p.
StrFloat & StrFloat::operator/(int p)
{
	if (p == 0) return (*this);
	if (p < 0) { //Kiểm tra số chia có âm hay không. Nếu có thì thay đổi dấu bị chia.
		m_negative = !m_negative;
		p = -p;
	}

	bool haveDot = false; //Kiểm tra xem có gặp dấu "." chưa.
	string tmp = ""; //Lưu kết quả của phép chia.

	int j = 0, afd = 0; //afd: số lượng số sau dấu phẩy.
	for (int k = 0; k < m_float.length(); k++) //Thực hiện phép chia từ trái sang phải.
		if (m_float[k] == '.') //Nếu gặp dấu "."
		{
			haveDot = true;
			if (tmp == "") tmp.push_back('0'); //Trường hợp phần nguyên không chia được cho số chia thì thêm "0" vào trước dấu ".".
			tmp.push_back('.');
		}
		else
		{
			j = j * 10 + m_float[k] - '0'; //Lấy số thứ k trong chuỗi.
			tmp.push_back((j / p) + '0'); //Thêm kết quả vào sau chuỗi tmp.
			j %= p;
			afd += haveDot;
		}

	//Nếu số bị chia nguyên và không chia được cho số chia thì thêm "0.". Nếu còn thừa số dư và vẫn chưa có phần thập phân thì thêm "."
	if (j > 0 && !haveDot)
	{
		if (tmp.length() == 0) tmp.push_back('0'); //Kiểm tra trường hợp số nguyên < số chia.
		tmp.push_back('.');
	}

	//Chia cho tới khi phần thập phân có đủ AFTER_DOT số hoặc số dư = 0.
	while (j > 0 && afd < AFTER_DOT) {
		j *= 10;
		tmp.push_back(j / p + '0');
		j %= p;
		afd++;
	}

	//Gán kết quả
	m_float = tmp;
	this->normalize();
	return (*this);
}

//Nhân xâu m_float cho số nhân p
StrFloat & StrFloat::operator*(int p)
{
	//Kiểm tra số nhân là âm thì đổi dấu của kết quả.
	if (p < 0) {
		p = -p;
		m_negative = !m_negative;
	}

	StrFloat res; //Lưu kết quả nhân
	int j = 0, s = 0;
	for (int i = m_float.length() - 1; i >= 0; i--)
		if (m_float[i] != '.')
		{
			j = (m_float[i] - '0')*p + s;
			s = j / 10;
			res.m_float.push_back(j % 10 + '0');
		}
		else res.m_float.push_back('.');

		//Nếu số dư còn thì thêm vào kết quả.
		while (s > 0) {
			res.m_float.push_back(s % 10 + '0');
			s = s / 10;
		}
		reverse(res.m_float.begin(), res.m_float.end());//Đảo ngược xâu kết quả để cho kết quả đúng.
		res.normalize(); //Chuẩn hóa kết quả.
		(*this) = res;
		return (*this);
}

StrFloat StrFloat::operator/=(int p)
{
	(*this) = (*this) / p;
	return (*this);
}

StrFloat StrFloat::operator+=(const StrFloat & p)
{
	(*this) = (*this) + p;
	return (*this);
}

//Tăng lên 1 đơn vị ở phần nguyên.
StrFloat StrFloat::operator++(int x)
{
	StrFloat p("1.0");
	p.normalize();
	(*this) += p;
	return (*this);
}

//Nhập xâu vào m_float rồi chuẩn hóa.
istream & operator>>(istream & is, StrFloat & p)
{
	is >> p.m_float;
	p.normalize();
	return is;
}

//Xuất xâu lưu trong m_float.
ostream & operator<<(ostream & os, const StrFloat & p)
{
	//if (p.m_negative) os << "-";
	os << p.m_float;
	return os;
}
