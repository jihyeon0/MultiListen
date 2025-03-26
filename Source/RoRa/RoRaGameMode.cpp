// Copyright Epic Games, Inc. All Rights Reserved.

#include "RoRaGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "EngineUtils.h"
#include "Kismet/KismetSystemLibrary.h" //디버깅용
#include "RoRaPlayerController.h"
#include "JudgeLibrary.h"
#include "RandomLibrary.h"
#include "RoRaGameState.h"
#include "GuessResultStruct.h"
#include "RoRaCharacter.h"
#include "RoRaPlayerState.h"

ARoRaGameMode::ARoRaGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	GameStateClass = ARoRaGameState::StaticClass(); //사용할 Gamestate 지정
}

void ARoRaGameMode::BeginPlay()
{
	Super::BeginPlay();

	/*
	 생각해봐 게임모드의 BeginPlay에서 컨트롤러 -> HUD를 부르면 이 타이밍에 HUD가 있을까
	 난 아니라고 봐 GameStart(), NotifyPlayerTurnOrder(); 를 어디로 옮겨야겟어 -> 진짜 모르겠음
	 플레이어가 생성될 때마다 호출되도록 만든다 -> 랜덤 번호가 부여되지 않음
	 모든 플레이어가 생성되고 호출한다 -> 그게 언젠데 ->모르겠지만 tick에서 한번호출하고 tick 설정끄는건 아닌거같아 ->턴 알려주지마
	아무래도 처음에 몇명으로 시작할지 입력받고 인원 수 정한다음에 해야 조건검사하면서 다 생성됐는지 알텐데 그걸 안 했네 이런~~ 
	*/
}

void ARoRaGameMode::NormalChat(const FString& NewString)
{
	//여기서 뭐 더 해줄거 있나?
	SendNormalChatToController(NewString);
}

void ARoRaGameMode::GuessNumber(int32 NewGuessNumber, ARoRaPlayerController* Ownerptr)
{
	//현재 순서인지 판단
	if (!IsCurrentTurn(Ownerptr))
	{
		return;
	}
	
	//플레이어 입력에 대한 판단
	FGuessResultStruct ResultStruct = UJudgeLibrary::JudgeGuessNumber(RandomNumber, NewGuessNumber); 

	//현재 컨트롤러가 연결된 캐릭터의 playerstate에 접근하여 정보 갱신
	ARoRaCharacter* RoRaCharacter = Cast<ARoRaCharacter>(Ownerptr->GetCharacter());
	if(RoRaCharacter)
	{
		ARoRaPlayerState* PlayerState = Cast<ARoRaPlayerState>(RoRaCharacter->GetPlayerState());
		if (PlayerState)
		{
			PlayerState->UpdateGuessResultStruct(ResultStruct);
			PlayerState->SetCurrentTry();
			
			SendGuessNumberToController(ResultStruct, Ownerptr);
		}
		
		if (IsEndGame())
		{
			//게임리셋
			ResetGame();
			GameStart();
		}
		else
		{
			// 게임이 끝나지 않았으면 playerstate를 다음 턴으로 갱신
			UpdateNextTurn();
		}
	}
}

void ARoRaGameMode::SendGuessNumberToController(const FGuessResultStruct& ResultStruct, ARoRaPlayerController* Ownerptr)
{
	//호출한다 클라에서 실행되는 플레이어컨트롤러의 함수(컨트롤러 ->HUD 전파)

	//오너십 설정  -> 오너의 플레이어컨트롤러에게만 전달
	if (GetWorld())
	{
		/*이렇게 하니까 서버에서만 뜸, 클라에서 호출돼도 무조건 호스트?가 불리나
		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())*/
		
		
		//입력을 보낸 대상에게만 전달
		for (TActorIterator<ARoRaCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			ARoRaCharacter* RoRaCharacter = *Iter;
			if (RoRaCharacter && RoRaCharacter->GetOwner() == Ownerptr)
			{
				if (ARoRaPlayerController* RoRaPlayerController = Cast<ARoRaPlayerController>(RoRaCharacter->GetController()))
				{
					RoRaPlayerController->ClientSendGuessResult(ResultStruct);
				}
			}
		}
	}
}

void ARoRaGameMode::SendNormalChatToController(const FString& NewString)
{
	//클라에서 실행되는 플레이어컨트롤러의 함수(컨트롤러 ->HUD 전파) 호출
	for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
	{
		ARoRaPlayerController* RoRaPlayerController = Cast<ARoRaPlayerController>(*Iter);
		if (RoRaPlayerController)
		{
			RoRaPlayerController->ClienttSendNormalChat(NewString);
		}
	}
}

void ARoRaGameMode::NotifyIncorrectAnswerAddPlayerOut(ARoRaPlayerController* Ownerptr)
{
	//현재 순서인지 판단
	if (!IsCurrentTurn(Ownerptr))
	{
		return;
	}
	
	FGuessResultStruct ResultStruct;
	
		for (TActorIterator<ARoRaCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			ARoRaCharacter* RoRaCharacter = *Iter;
			if (RoRaCharacter && RoRaCharacter->GetOwner() == Ownerptr)
			{
				ARoRaPlayerState* PlayerState = Cast<ARoRaPlayerState>(RoRaCharacter->GetPlayerState());
				if (PlayerState)
				{
					ResultStruct = PlayerState->NotifyIncorrectAnswerAddPlayerOut();
				}
				if (ARoRaPlayerController* RoRaPlayerController = Cast<ARoRaPlayerController>(RoRaCharacter->GetController()))
				{
					if (RoRaPlayerController)
					{
						ResultStruct.ResultString = FString::Printf(TEXT("%d"), ResultStruct.StrikeCount)+ "S"
									   + FString::Printf(TEXT("%d"), ResultStruct.BallCount) + "B"
									   + FString::Printf(TEXT("%d"), ResultStruct.OutCount) + "OUT";
						RoRaPlayerController->ClientSendGuessResult(ResultStruct);
					}
				}
			}
		}

	if (IsEndGame())
	{
		//게임리셋
		ResetGame();
		GameStart();
	}
	else
	{
		// 게임이 끝나지 않았으면 playerstate를 다음 턴으로 갱신
		UpdateNextTurn();
	}
}

void ARoRaGameMode::SendResultChatToController(ARoRaPlayerState* Winner)
{
	ARoRaPlayerController* WinnerController = Cast<ARoRaPlayerController>(Winner->GetPlayerController());

	if (WinnerController)
	{
		for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
		{
			ARoRaPlayerController* RoRaPlayerController = Cast<ARoRaPlayerController>(*Iter);
		
			if (RoRaPlayerController == WinnerController)
			{
				FGuessResultStruct Result;
				Result.ResultString = FString::Printf(TEXT("축하합니다. 1위"));
				RoRaPlayerController->ClientSendGuessResult(Result);
			}
			else
			{
				FGuessResultStruct Result;
				Result.ResultString = FString::Printf(TEXT("저런...패배하셨습니다."));
				RoRaPlayerController->ClientSendGuessResult(Result);
			}
		
		}
	}
}


bool ARoRaGameMode::IsEndGame()
{
	//플레이어state 순회하면서 게임종료인지 판단
	ARoRaPlayerState* Winner = nullptr;
	TArray<ARoRaPlayerState*> AlivePlayers;
	ARoRaGameState* RoRaGameState = GetGameState<ARoRaGameState>();
	
	if (RoRaGameState)
	{
		 TArray<APlayerState*> PlayerStates = RoRaGameState->PlayerArray;
		if (!PlayerStates.IsEmpty())
		{
			for (APlayerState* State : PlayerStates)
			{
				ARoRaPlayerState* RoRaPlayerState = Cast<ARoRaPlayerState>(State);
				if (RoRaPlayerState)
				{
					//즉시 종료 -> 1위 나옴
					if (RoRaPlayerState->GetResultStrike() == MaxTries)
					{
						Winner = RoRaPlayerState;
						break;
					}

					//1위가 나오지 않고, 기회가 남았는지 확인
					if (RoRaPlayerState->GetCurrentTry() < MaxTries)
					{
						AlivePlayers.Add(RoRaPlayerState);
					}
				}
			}

			//3S로 즉시 종료 , 1위 결정된 후 처리
			if (Winner)
			{
				EndGameWithWinner(Winner);
				return true;
			}

			//기회를 다 쓰고, 1위가 나오지 않고, 살아남은 사람 없음 -> 무승부
			if (AlivePlayers.Num() == 0)
			{
				EndGameWithDraw();
				return true;
			}
		}
	}
	return false;
}

void ARoRaGameMode::EndGameWithWinner(ARoRaPlayerState* Winner)
{
	//게임종료 전체 메시지 전송
	FString EndGameMsg = FString::Printf(TEXT("게임 종료"))
						+ FString::Printf(TEXT(" 정답은 %d 입니다."), RandomNumber); 
	SendNormalChatToController(EndGameMsg);
	
	//1위 플레이어 , 나머지 플레이어 구분 해서 결과 메세지 전송
	SendResultChatToController(Winner);
}

void ARoRaGameMode::EndGameWithDraw()
{
	//게임종료 전체 메시지 전송
	FString EndGameMsg = FString::Printf(TEXT("게임 종료"))
						+ FString::Printf(TEXT(" 정답은 %d 입니다."), RandomNumber);
	SendNormalChatToController(EndGameMsg);

	//추가게임 진행 메세지 전송
	FString NewGameMsg = FString::Printf(TEXT("연장 게임을 시작합니다.")); 
	SendNormalChatToController(NewGameMsg);
}

void ARoRaGameMode::ResetGame()
{
	RandomNumber = 0;
	CurrentOrderArrayIndex = 0;
	RandomOrderArray.Reset();
	
	//gamestate reset
	GetGameState<ARoRaGameState>()->ResetGame();

	//playerstate reset
	ARoRaGameState* RoRaGameState = GetGameState<ARoRaGameState>();
	if (RoRaGameState)
	{
		TArray<APlayerState*> PlayerStates = RoRaGameState->PlayerArray;
		if (!PlayerStates.IsEmpty())
		{
			for (APlayerState* State : PlayerStates)
			{
				ARoRaPlayerState* RoRaPlayerState = Cast<ARoRaPlayerState>(State);
				if (RoRaPlayerState)
				{
					RoRaPlayerState->ResetGame();
				}
			}
		}
	}
}

void ARoRaGameMode::ReadyForGameStart(ARoRaPlayerController* Requestptr)
{
	//우리 게임 시작할거야 -> 방장이 보낸 요청이 맞는지 확인
	if (GameState)
	{
		ARoRaPlayerState* State = Cast<ARoRaPlayerState>(GameState->PlayerArray[0]);
		if (State)
		{
			ARoRaPlayerController* Controller = Cast<ARoRaPlayerController>(State->GetPlayerController());
			if (Controller && Controller == Requestptr)
			{
				//몇 명 참여할거야?
				Requestptr->ClientRequestNumberOfParticipants();
			}
		}
	}
}

void ARoRaGameMode::CheckAllReady(ARoRaPlayerController* Requestptr)
{
	/*
	ParticipantsArray.AddUnique(Requestptr);

	if (ParticipantsArray.Num() == GetGameState<ARoRaGameState>()->GetNumberOfParticipants())
	{
		GetGameState<ARoRaGameState>()->SetGameStart(true);
		GameStart();

		//전체 레디하면 CheckAllReady를 부르는 타이머 해제 

		//매치 취소 타이머 해제
	}
	*/

}

void ARoRaGameMode::GameStart()
{
	//전체레디체크를 대신하는 임시코드-> 순회하면서 전부 넣어버림 -> 방장이 입력한 수와 실제 접속한 플레이어 수가 일치해야함
	for (int i=0; i<GameState->PlayerArray.Num(); i++)
	{
		ParticipantsArray.AddUnique(Cast<ARoRaPlayerController>(GameState->PlayerArray[i]->GetPlayerController()));
	}
	
	//게임 진행되는 동안은 게임모드만 알고 있기, 판단을 게임모드에서 하기 때문에 클라가 정답을 알 필요없음
	RandomNumber = URandomLibrary::GenerateRandomNumber();

	//참여한 플레이어들의 랜덤 순서 결정
	RandomOrderArray = URandomLibrary::GenerateRandomOrder(ParticipantsArray.Num());
	CurrentTurnPlayerState = ParticipantsArray[RandomOrderArray[CurrentOrderArrayIndex]]->GetPlayerState<ARoRaPlayerState>();
	
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("디버깅을 위한 RandomNumber: %d"), RandomNumber), true, true);
}

void ARoRaGameMode::MatchCancel()
{
}

bool ARoRaGameMode::IsCurrentTurn(ARoRaPlayerController* PlayerController)
{
	//Cast의 지옥에 빠져버린거야
	ARoRaPlayerState* RequestPlayerState = PlayerController->GetPlayerState<ARoRaPlayerState>();
	if (RequestPlayerState == nullptr || CurrentTurnPlayerState != RequestPlayerState )
	{
		FGuessResultStruct ResultStruct;
		ResultStruct.ResultString = FString::Printf(TEXT("틀렸어. 당신 차례가 아니야. 기다려"));
		PlayerController->ClientSendGuessResult(ResultStruct);
		return false;
	}
	return true;
}

void ARoRaGameMode::UpdateNextTurn()
{
	CurrentOrderArrayIndex++;
	CurrentOrderArrayIndex = CurrentOrderArrayIndex % ParticipantsArray.Num();
	CurrentTurnPlayerState = ParticipantsArray[RandomOrderArray[CurrentOrderArrayIndex]]->GetPlayerState<ARoRaPlayerState>();

	ItsYourTurn();
}


void ARoRaGameMode::SetNumberOfParticipants(int32 Number)
{
	if (GameState)
	{
		ARoRaPlayerState* State = Cast<ARoRaPlayerState>(GameState->PlayerArray[0]);
		if (State)
		{
			//참여할 인원이 정해짐
			GetGameState<ARoRaGameState>()->SetNumberOfParticipants(Number);

			/* 전체 레디 체크 버전 코드
			레디체크하는 타이머 설정 -> 인자 받는거 있어서 타이머 설정 못함 -> 타이머델리게이트 , 델리게이트 실패
			매치 취소 타이머는 알아서 돌고 있다 && 플레이어가 각자 확인 누를 때마다 호출? ->그럴싸함 
			//전체에게 레디하라고 메세지 전송
			FString Msg = FString::Printf(TEXT("레디 박으세요"));
			SendNormalChatToController(Msg);
			*/


			GameStart();
			FString StartMsg = FString::Printf(TEXT("시작합니다."));
			SendNormalChatToController(StartMsg);
			ItsYourTurn();
		}
	}
}

void ARoRaGameMode::ItsYourTurn()
{
	FGuessResultStruct Struct;
	Struct.ResultString = FString::Printf(TEXT("당신 차례입니다."));
	Cast<ARoRaPlayerController>(CurrentTurnPlayerState->GetPlayerController())->ClientSendGuessResult(Struct);
}

/* 돌려돌려 돌림판
	GameState 는 PlayerArray 로 PlayerState 들을 가져올 수 있음
	PlayerState 는 GetPlayerController() 로 Controller 가져올 수 있음
	Controller 는 GetCharacter() 로 오너십 설정된 캐릭터를 가져올 수 있음, GetPlayerState<ARoRaPlayerState>() 로 PlayerState를 가져올 수 있음
	Character 는 GetPlayerState() 로 자신의 PlayerState를 가져올 수 있음
*/
