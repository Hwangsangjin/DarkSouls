// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/DarkSoulsGameMode.h"

ADarkSoulsGameMode::ADarkSoulsGameMode()
{
	// 기본 폰 클래스를 플레이어로 설정
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClass(TEXT("/Script/Engine.Blueprint'/Game/AA_HSJ/Blueprints/Actor/BP_Player.BP_Player_C'"));
	if (PlayerPawnClass.Class)
	{
		DefaultPawnClass = PlayerPawnClass.Class;
	}
}
