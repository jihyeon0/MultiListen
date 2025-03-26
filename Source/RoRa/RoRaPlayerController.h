// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RoRaPlayerController.generated.h"

/**
 * 
 */
class UInputMappingContext;
class UInputAction;
class URoRaHUD;
class UPlayerCountSelectWidget;
struct FGuessResultStruct;

UCLASS()
class RORA_API ARoRaPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	ARoRaPlayerController();

public:
	virtual void BeginPlay() override;
	void ShowPlayerCountWidget();
	void HidePlayerCountWidget();

	UFUNCTION(Client, Reliable)
	void ClienttSendNormalChat(const FString& Result);

	UFUNCTION(Client, Reliable)
	void ClientSendGuessResult(const FGuessResultStruct& ResultStruct);

	UFUNCTION(Server, Reliable)
	void ServerSendMsg(const FString& NewString, ARoRaPlayerController* Ownerptr);

	UFUNCTION(Server, Reliable)
	void ServerSetNumberOfParticipants(int32 PlayerCount);
	
	UFUNCTION(Client, Reliable)
	void ClientSendErrorMsg(const FString& ErrorMsg);

	UFUNCTION(Client, Reliable)
	void ClientRequestNumberOfParticipants();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* RoRaInputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> UserWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PlayerCountWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	URoRaHUD* RoRaHUD = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UPlayerCountSelectWidget* PlayerCountWidget = nullptr;
};
