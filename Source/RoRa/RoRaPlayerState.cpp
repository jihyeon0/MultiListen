// Fill out your copyright notice in the Description page of Project Settings.


#include "RoRaPlayerState.h"

#include "Net/UnrealNetwork.h"

int32 ARoRaPlayerState::GetCurrentTry()
{
	return CurrentTry;
}

void ARoRaPlayerState::SetCurrentTry(int32 count)
{
	CurrentTry += count;
}

void ARoRaPlayerState::ResetGame()
{
	CurrentTry = 0;
	GuessResultStruct.Reset(); //요소를 삭제하지만 TArray자체의 메모리 주소는 바뀌지 않음
}

void ARoRaPlayerState::UpdateGuessResultStruct(const FGuessResultStruct& GuessResult)
{
	//매개변수로 넘겨줄 때 불필요한 메모리 복사 막으려고 &로 가져옴
	//TArray<FGuessResultStruct> GuessResultStruct 에 넣어줄때는 깊은 복사로 새로운 메모리에 같은 데이터로 넣기
	GuessResultStruct.Add(GuessResult);
}

FGuessResultStruct& ARoRaPlayerState::NotifyIncorrectAnswerAddPlayerOut()
{
	//마지막 GuessResultStruct의 요소를 꺼내서 복사 후 OUT 카운트만 늘리고 요소 추가 시키기
	if (GuessResultStruct.IsEmpty())
	{
		SetCurrentTry();
		FGuessResultStruct NewStruct;
		NewStruct.OutCount = 1;
		GuessResultStruct.Add(NewStruct);
		return GuessResultStruct.Last();
	}
	
	SetCurrentTry();
	FGuessResultStruct NewStruct = GuessResultStruct.Last();
	NewStruct.OutCount += 1;
	GuessResultStruct.Add(NewStruct);

	return GuessResultStruct.Last();
}

void ARoRaPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME( ARoRaPlayerState, CurrentTry );
	DOREPLIFETIME( ARoRaPlayerState, GuessResultStruct );
}

int32 ARoRaPlayerState::GetResultStrike()
{
	if (!GuessResultStruct.IsEmpty())
	{
		return GuessResultStruct.Last().StrikeCount; 
	}
	return -1;
}

int32 ARoRaPlayerState::GetResultOut()
{
	if (!GuessResultStruct.IsEmpty())
	{
		return GuessResultStruct.Last().OutCount;
	}
	return -1;
}