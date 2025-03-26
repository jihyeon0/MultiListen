// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "JudgeLibrary.generated.h"

/**
 * S/B 판정 전용 라이브러리
 * 변수 저장 하지 말고 , 입력 → 출력 구조로 만들기
 */
UCLASS()
class RORA_API UJudgeLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary")
	static  FGuessResultStruct& JudgeGuessNumber(int32 CorrectNumber, int32 GuessNumber);
};
