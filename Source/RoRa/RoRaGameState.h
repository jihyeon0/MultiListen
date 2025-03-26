// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RoRaGameState.generated.h"

UCLASS()
class RORA_API ARoRaGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION()
	void SetRandomNumber(int32 Number);

	UFUNCTION()
	void ResetGame();
	
	UFUNCTION()
	void SetNumberOfParticipants(int32 Number);

	UFUNCTION()
	int32 GetNumberOfParticipants();

	UFUNCTION()
	bool GetGameStart();

	UFUNCTION()
	void SetGameStart(bool Result);
	
protected:
	//매 tick 클라의 gamestate에 복제갱신, 리플리케이티드 구현방식은 UnReliable 이지만 매 tick 실행되므로 괜찮아 딩딩딩딩딩
	//랜덤넘버는 끝날 때 한번만 공유해주면 되는데 등록해둘 필요가 있는가 
	UPROPERTY(Replicated,EditAnywhere)
	int32 RandomNumber = 0;

	UPROPERTY(Replicated,EditAnywhere)
	int32 Participants = 0;

	UPROPERTY(Replicated,EditAnywhere)
	bool IsGameStart = false;

	UPROPERTY(Replicated,EditAnywhere)
	bool IsAllReady = false;
};
