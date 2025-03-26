// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomLibrary.h"
#include "Algo/RandomShuffle.h"

int32 URandomLibrary::GenerateRandomNumber()
{
	// 1~9까지 중복없는 3자리 숫자 생성
	TArray<int32> Numbers = {1,2,3,4,5,6,7,8,9};
	Algo::RandomShuffle(Numbers);
	int32 RandomNumber = Numbers[0] *100 + Numbers[1] *10 + Numbers[2];
	return RandomNumber;
}

TArray<int32> URandomLibrary::GenerateRandomOrder(int32 PlayerArrayNum)
{
	//플레이어들의 랜덤 순서 생성
	TArray<int32> Numbers;
	for (int32 i = 0; i < PlayerArrayNum; i++)
	{
		Numbers.Add(i);
	}
	Algo::RandomShuffle(Numbers);
	
	return Numbers;
}