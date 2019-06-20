#pragma once
#include<iostream>
#include<string>
using namespace std;
const int defaultSize = 5;
void Assert(bool val, string s)
{
	if (!val)
	{
		cout << "Assertion Failed:" << s << endl;
		exit(-1);
	}
};

template<typename E>
void printList(List<E> &L)
{
	int currpos = L.currPos();
	L.moveToStart();
	cout << "<";
	for (int i = 0; i<currpos; i++)
	{
		cout << L.getValue() << ",";
		L.next();
	}
	cout << "|";
	for (int i = currpos; i<L.length(); i++)
	{
		cout << L.getValue() << ",";
		L.next();
	}
	cout << ">" << endl;
	L.moveToPos(currpos);
};