// Fill out your copyright notice in the Description page of Project Settings.


#include "RoRaPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GuessResultStruct.h"
#include "Blueprint/UserWidget.h"
#include "RoRaGameMode.h"
#include "RoRaHUD.h"
#include "PlayerCountSelectWidget.h"
#include "Kismet/KismetSystemLibrary.h"

ARoRaPlayerController::ARoRaPlayerController()
	:RoRaInputMappingContext(nullptr), JumpAction(nullptr),MoveAction(nullptr),LookAction(nullptr)
{
	//여기가 아니지요 생각이란걸하란말이지요 이것은 컨트롤러가 컨트롤러 자신을 소유하라는 말이지요 -> 플레이어 캐릭터에서 해야지요 
	//SetOwner(this); 
}

void ARoRaPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	UKismetSystemLibrary::PrintString(this, " Create RoRaPlayerController");

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (RoRaInputMappingContext)
			{
				Subsystem->AddMappingContext(RoRaInputMappingContext,0);
			}
		}
	}

	//클라에서만 ui 생성, 서버에서 생성 방지
	if (IsLocalController())  // -> 이 조건이 계속 헷갈림
	{
		if (UserWidgetClass) //얘가 이미 BP_RoRaHUD
		{
			UUserWidget* HUDWidget = CreateWidget<UUserWidget>(this,UserWidgetClass);
			
			if (HUDWidget)
			{
				RoRaHUD = Cast<URoRaHUD>(HUDWidget);
				if (RoRaHUD)
				{
					RoRaHUD->AddToViewport();
					//HUD는 UI만 만들고, 실제로 로직을 처리할 애는 PlayerController
					//델리게이트 시그니처랑 바인딩할 함수의 시그니처 일치 시켜야함
					RoRaHUD->OnGuessMsg.AddDynamic(this, &ARoRaPlayerController::ServerSendMsg);
				}
			}
		}
	}
}
void ARoRaPlayerController::ShowPlayerCountWidget()
{
	if (!PlayerCountWidget && PlayerCountWidgetClass)
	{
		PlayerCountWidget = CreateWidget<UPlayerCountSelectWidget>(this, PlayerCountWidgetClass);
	}

	if (PlayerCountWidget)
	{
		PlayerCountWidget->AddToViewport();
	}
}

void ARoRaPlayerController::HidePlayerCountWidget()
{
	if (PlayerCountWidget)
	{
		PlayerCountWidget->RemoveFromParent();
		PlayerCountWidget = nullptr;
	}
}
void ARoRaPlayerController::ServerSetNumberOfParticipants_Implementation(int32 PlayerCount)
{
	HidePlayerCountWidget();
	
	//얘네 이만큼 참여한대요
	if (ARoRaGameMode* RoRaGameMode = Cast<ARoRaGameMode>( GetWorld()->GetAuthGameMode()))
	{
		RoRaGameMode->SetNumberOfParticipants(PlayerCount);
	}
}

void ARoRaPlayerController::ClientRequestNumberOfParticipants_Implementation()
{
	//위젯 추가 : 메세지, 인원, 확인
	ShowPlayerCountWidget();
}

void ARoRaPlayerController::ClientSendErrorMsg_Implementation(const FString& ErrorMsg)
{
	if (RoRaHUD)
	 {
	 	RoRaHUD->SetChatVerticalBox(ErrorMsg);
	 }
}

void ARoRaPlayerController::ClientSendGuessResult_Implementation(const FGuessResultStruct& ResultStruct)
{
	if (RoRaHUD)
	{
		//여기 나중에 돌려봐서 이상하면 수정
		//여기서 정상 답변은 string으로 처리하고 오류에 대한 답변은 변수값만 변경해서 보내고잇어서 string은 비어잇음
		RoRaHUD->SetChatVerticalBox(ResultStruct.ResultString);
	}
}

void ARoRaPlayerController::ClienttSendNormalChat_Implementation(const FString& Result)
{
	//게임 모드로부터 받은 메세지를 출력 -> 위젯에 텍스트블록으로 넘겨주도록 수정해야함
	if (RoRaHUD)
	{
		RoRaHUD->SetChatVerticalBox(Result);
	}
}

void ARoRaPlayerController::ServerSendMsg_Implementation(const FString& NewString, ARoRaPlayerController* Ownerptr)
{
	//게임모드까지 갈 필요없이 여기서 0.게임시작인지, 1.일반 채팅인지 2.게임에 대한 대답인지 3.잘못된 메세지인지 판단 ->HUD는 위젯 띄우는 역할만 하기 위해서 고려X

	if(NewString[0] == TEXT('/')) //게임에 대한 문자열인 경우
	{
		/*오류 조건 -> 이걸 오류타입으로 분류해서 메세지 띄우면 좋을 듯
		*세글자가 아닌 경우
		*공백이 있는 경우
		 *숫자가 아닌 문자
		 *중복되는 숫자
		*/
		
		// 문자열에서 "/" 제거
		FString RemoveSlashString = NewString.Replace(TEXT("/"), TEXT(""));
		//문자열에서 공백 제거
		FString RemoveGapString = RemoveSlashString.Replace(TEXT(" "), TEXT(""));

		if (RemoveGapString == FString::Printf(TEXT("게임시작")))
		{
			if (ARoRaGameMode* RoRaGameMode = Cast<ARoRaGameMode>( GetWorld()->GetAuthGameMode()))
			{
				RoRaGameMode->ReadyForGameStart(Ownerptr);
			}
			return;
		}
		
		if(RemoveGapString.Len() != 3) 
		{
			FString ErrorMsg = TEXT("정답에 대한 정상적인 대답이 아닙니다. 이유 : 문자열 길이 오류");
			//하이고 여기는 이미 서버입니다... 죽도록 위젯 함수 호출해봣자 모릅니다~. 클라RPC를 쏘세요
			//컨트롤러 헤더에 클라rpc 함수 만들고 호출
			ClientSendErrorMsg(ErrorMsg);
			//GameMode의 잘못된 입력으로 인한 out에 대해 처리하는 함수 호출
			if (ARoRaGameMode* RoRaGameMode = Cast<ARoRaGameMode>( GetWorld()->GetAuthGameMode()))
			{
				RoRaGameMode->NotifyIncorrectAnswerAddPlayerOut(this);
			}
			return;
		}
		
		for (int i = 0; i<RemoveGapString.Len(); i++)
		{
			if (!FChar::IsDigit(RemoveGapString[i])) //문자 하나가 숫자인지 확인
			{
				FString ErrorMsg = TEXT("정답에 대한 정상적인 대답이 아닙니다. 이유 : 숫자가 아닌 문자 포함");
				ClientSendErrorMsg(ErrorMsg);
				//GameMode의 잘못된 입력으로 인한 out에 대해 처리하는 함수 호출
				if (ARoRaGameMode* RoRaGameMode = Cast<ARoRaGameMode>( GetWorld()->GetAuthGameMode()))
				{
					RoRaGameMode->NotifyIncorrectAnswerAddPlayerOut(this);
				}
				return;
			}
		}

		TSet<TCHAR> FindDuplication;
		for (TCHAR StringChar : RemoveGapString)
		{
			if (FindDuplication.Contains(StringChar)) // 이미 있는 문자면 true
			{
				FString ErrorMsg = TEXT("정답에 대한 정상적인 대답이 아닙니다. 이유 : 숫자 중복");
				ClientSendErrorMsg(ErrorMsg);
				//GameMode의 잘못된 입력으로 인한 out에 대해 처리하는 함수 호출
				if (ARoRaGameMode* RoRaGameMode = Cast<ARoRaGameMode>( GetWorld()->GetAuthGameMode()))
				{
					RoRaGameMode->NotifyIncorrectAnswerAddPlayerOut(this);
				}
				
				return;
			}
			FindDuplication.Add(StringChar);
		}
		
		int NewGuessNumber = 0;
		LexTryParseString(NewGuessNumber, *RemoveGapString); //FString -> int
		
		if (GetWorld())
		{
			if (ARoRaGameMode* RoRaGameMode = Cast<ARoRaGameMode>( GetWorld()->GetAuthGameMode()))
			{
				RoRaGameMode->GuessNumber(NewGuessNumber, Ownerptr);
			}
		}
	}
	else //일반채팅에 대한 문자열인 경우
	{
		if (GetWorld())
		{
			if (ARoRaGameMode* RoRaGameMode = Cast<ARoRaGameMode>( GetWorld()->GetAuthGameMode()))
			{
				RoRaGameMode->NormalChat(NewString);
			}
		}
	}
}