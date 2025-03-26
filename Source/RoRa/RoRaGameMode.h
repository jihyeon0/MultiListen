#pragma once

#include "CoreMinimal.h"
#include "RoRaPlayerState.h"
#include "GameFramework/GameModeBase.h"
#include "RoRaGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCheckReady,ARoRaPlayerController*, Resultptr);

class ARoRaPlayerController;

UCLASS(minimalapi)
class ARoRaGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARoRaGameMode();

	virtual void BeginPlay() override;
	
	UFUNCTION() 
	void NormalChat(const FString& NewString);

	UFUNCTION() 
	void GuessNumber(int32 NewGuessNumber, ARoRaPlayerController* Ownerptr);

	//흠 string...
	UFUNCTION()
	void SendGuessNumberToController(const FGuessResultStruct& ResultStruct, ARoRaPlayerController* Ownerptr);

	UFUNCTION()
	void SendNormalChatToController(const FString& NewString);

	UFUNCTION()
	void NotifyIncorrectAnswerAddPlayerOut(ARoRaPlayerController* Ownerptr);
	
	UFUNCTION()
	void SendResultChatToController(ARoRaPlayerState* Winner);
	
	UFUNCTION()
	void EndGameWithWinner(ARoRaPlayerState* Winner);

	UFUNCTION()
	void EndGameWithDraw();
	
	UFUNCTION()
	bool IsEndGame();

	UFUNCTION()
	void ResetGame();

	UFUNCTION()
	void ReadyForGameStart(ARoRaPlayerController* Requestptr);
	
	UFUNCTION()
	void CheckAllReady(ARoRaPlayerController* Requestptr);
	
	UFUNCTION()
	void GameStart();

	UFUNCTION()
	void MatchCancel();
	
	UFUNCTION()
	bool IsCurrentTurn(ARoRaPlayerController* PlayerController);

	UFUNCTION()
	void UpdateNextTurn();

	UFUNCTION()
	void SetNumberOfParticipants(int32 Number);

	UFUNCTION()
	void ItsYourTurn();

	//게임모드가 가지고 있을 필요가 있나?
	UPROPERTY()
	int32 RandomNumber = 0;

	UPROPERTY()
	int MaxTries = 3;

	UPROPERTY()
	int32 CurrentOrderArrayIndex = 0;
	
	UPROPERTY()
	TArray<int32> RandomOrderArray;

	UPROPERTY()
	TArray<ARoRaPlayerController*> ParticipantsArray;
	
	UPROPERTY()
	ARoRaPlayerState* CurrentTurnPlayerState = nullptr;

	UPROPERTY()
	FTimerHandle MatchCancelTimerHandle;
};






