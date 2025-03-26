#include "JudgeLibrary.h"
#include "GuessResultStruct.h"

FGuessResultStruct& UJudgeLibrary::JudgeGuessNumber(int32 CorrectNumber, int32 GuessNumber)
{
	/* 
		서버가 생성한 랜덤한 숫자와 사용자가 입력한 숫자를 비교하여 결과를 반환합니다.
		- 자리수와 값이 같으면 스트라이크(S) 숫자를 늘립니다.
		- 자리수는 다르고 값이 같으면 볼(B) 숫자를 늘립니다.
		- 예 1. 서버 생성 숫자 386, 사용자 답변 127 ⇒ OUT
		- 예 2. 서버 생성 숫자 386, 사용자 답변 167 ⇒ 0S1B
		- 예 3. 서버 생성 숫자 386, 사용자 답변 367 ⇒ 1S1B
		- 예 4. 서버 생성 숫자 386, 사용자 답변 396 ⇒ 2S0B
		- 예 4. 서버 생성 숫자 386, 사용자 답변 386 ⇒ 3S0B ⇒ Win
	 */

	//무조건 세자리 숫자만 들어옴
	//그냥 new? 과연 이대로 괜찮은가
	FGuessResultStruct* GuessResultStruct = new FGuessResultStruct();
	GuessResultStruct->StrikeCount = 0;
	GuessResultStruct->BallCount = 0;
	
	//자리수와 값이 같은 경우
	FString fCorrectNumber = FString::FromInt(CorrectNumber);
	FString fGuessNumber = FString::FromInt(GuessNumber);

	//여기 더 수정해야함
	for (int i = 0; i < fCorrectNumber.Len(); i++)
	{
		for (int j=0; j < fGuessNumber.Len(); j++)
		{
			if (fCorrectNumber[i] == fGuessNumber[j]) //숫자가 똑같은데
			{
				if (i == j) //자리수가 같아
				{
					GuessResultStruct->StrikeCount += 1;
				}
				else //자리수는 달라
				{
					GuessResultStruct->BallCount += 1;
				}
			}	
		}
	}
	//다 돌았는데 처음과 숫자가 똑같아? ->OUT
	if (GuessResultStruct->StrikeCount ==0 && GuessResultStruct->BallCount ==0)
	{
		GuessResultStruct->OutCount += 1;
	}
	
	GuessResultStruct->ResultString = FString::Printf(TEXT("%d"), GuessResultStruct->StrikeCount)+ "S"
									+ FString::Printf(TEXT("%d"), GuessResultStruct->BallCount) + "B"
									+ FString::Printf(TEXT("%d"), GuessResultStruct->OutCount) + "OUT";
	return *GuessResultStruct;
}

