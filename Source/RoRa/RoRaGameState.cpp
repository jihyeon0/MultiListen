#include "RoRaGameState.h"
#include "GuessResultStruct.h"
#include "Net/UnrealNetwork.h"

void ARoRaGameState::SetRandomNumber(int32 Number)
{
	RandomNumber = Number;
}

void ARoRaGameState::ResetGame()
{
	RandomNumber = 0;
	IsGameStart = false;
	IsAllReady = false;
}

void ARoRaGameState::SetNumberOfParticipants(int32 Number)
{
	Participants = Number;
}

int32 ARoRaGameState::GetNumberOfParticipants()
{
	return Participants;
}

bool ARoRaGameState::GetGameStart()
{
	return IsGameStart;
}

void ARoRaGameState::SetGameStart(bool Result)
{
	IsGameStart = Result;
}

void ARoRaGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME( ARoRaGameState, RandomNumber );
	DOREPLIFETIME( ARoRaGameState, IsGameStart );
	DOREPLIFETIME( ARoRaGameState, IsAllReady );
}
