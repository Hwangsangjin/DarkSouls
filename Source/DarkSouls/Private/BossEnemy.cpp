// Fill out your copyright notice in the Description page of Project Settings.


#include "BossEnemy.h"
#include "EnemyFSM.h"
#include "EnemyAnim.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h>

// Sets default values
ABossEnemy::ABossEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EnemyFSM = CreateDefaultSubobject<UEnemyFSM>(TEXT("EnemyFSM"));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/AA_GDH/Boss/Animation/GDH_Boss.GDH_Boss'"));

	if (tempMesh.Succeeded())
	{

		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0,0,-90),FRotator(0,-90,0));
	}

	ConstructorHelpers::FClassFinder<UEnemyAnim> tempAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/AA_GDH/Boss/ABP_BossAnim.ABP_BossAnim_C'"));
	if (tempAnim.Succeeded())
	{

		GetMesh()->SetAnimClass(tempAnim.Class);


	}
	GetCharacterMovement()->bOrientRotationToMovement = true;

}

// Called when the game starts or when spawned
void ABossEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABossEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABossEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

