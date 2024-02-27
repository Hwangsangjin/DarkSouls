// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/DarkSoulsGameMode.h"

ADarkSoulsGameMode::ADarkSoulsGameMode()
{
	// �⺻ �� Ŭ������ �÷��̾�� ����
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClass(TEXT("/Script/Engine.Blueprint'/Game/AA_HSJ/Blueprints/Actor/BP_Player.BP_Player_C'"));
	if (PlayerPawnClass.Class)
	{
		DefaultPawnClass = PlayerPawnClass.Class;
	}
}
