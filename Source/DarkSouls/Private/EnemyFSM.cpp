// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "Character/PlayerCharacter.h"
#include "BossEnemy.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/CapsuleComponent.h>
#include "EnemyAnim.h"

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	

	// ...
	
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	Me = Cast<ABossEnemy>(GetOwner());

	EnemyAnim = Cast<UEnemyAnim>(Me->GetMesh()->GetAnimInstance());

	// ...
	
}

// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	switch (State)
	{
	
	case EEnemyState::Idle:		TickIdle();			break;
	case EEnemyState::Move:		TickMove();			break;
	case EEnemyState::Attack:	TickAttack();		break;
	case EEnemyState::Damage:	TickDamge();		break;
	case EEnemyState::Die:		TickDie();			break;

	}




}

void UEnemyFSM::TickIdle()
{
	// 플레이어를 찾고 싶다.
	Target = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	// 만약 찾았다면
	// 플레이어와의 거리를 구한다.
	FVector dir = Target->GetActorLocation() - Me->GetActorLocation();
	if (Target && dir.Size() < 2000.0f)
	{
		// 이동 상태로 전이하고 싶다.
		SetState(EEnemyState::Move);
	}
}

void UEnemyFSM::TickMove()
{
	// 목적지를 향해서 이동하고싶다
	FVector dir = Target->GetActorLocation() - Me->GetActorLocation();
	
	Me->AddMovementInput(dir.GetSafeNormal());
	// 만약 공격 가능거리라면
	if (dir.Size() < AttackDistance)
	{

		// 공격 상태로 전이하고 싶다.
		SetState(EEnemyState::Attack);
		CurrentTime = AttackTime;
	}
}

void UEnemyFSM::TickAttack()
{
	
	CurrentTime += GetWorld()->GetDeltaSeconds();

	if (CurrentTime > AttackTime)
	{
		
		CurrentTime = 0;
	
		float dist = FVector::Dist(Target->GetActorLocation(), Me->GetActorLocation());

		if (dist > AttackDistance)
		{
	
			
			SetState(EEnemyState::Move);
		}
		else
		{

		UE_LOG(LogTemp,Warning,TEXT("Attack!"));
		}
	
	
	}


}

void UEnemyFSM::TickDamge()
{

	CurrentTime += GetWorld()->GetDeltaSeconds();

	if (CurrentTime > DamageTime)
	{
		SetState(EEnemyState::Move);

	}

}

void UEnemyFSM::TickDie()
{

	if (false == EnemyAnim->IsDieDone)
	{
		return;

	}



	//CurrentTime += GetWorld()->GetDeltaSeconds();
	//if (CurrentTime > DieTime)
	//{

	//	Me->Destroy();
	//}
	//else
	//{
	//	//p = p0 + vt;
	//	//FVector P0 = Me->GetActorLocation();
	//	//FVector dir = FVector::DownVector;
	//	//FVector velocity = dir * 200;
	//	//FVector vt = velocity * GetWorld()->GetDeltaSeconds();
	//	//Me->SetActorLocation(P0+ vt);

	//	FVector NewLoc = FMath::Lerp(Me->GetActorLocation(), DieEndLoc, GetWorld()->GetDeltaSeconds() * 2);
	//	Me->SetActorLocation(NewLoc);						
	//																				
	//}								
									
}						

void UEnemyFSM::OnTakeDamage(int32 damage)
{
	HP -= damage;

	if (HP > 0) 
	{
		SetState(EEnemyState::Damage);
		PlayMontageDamage();

	}
	else 
	{
		
		SetState(EEnemyState::Die);
		PlayMontageDie();
		Me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DieEndLoc = Me->GetActorLocation() + FVector::DownVector * 300;
	}
}

void UEnemyFSM::SetState(EEnemyState next)
{
	check(EnemyAnim);
	State = next;
	EnemyAnim->State = next;
	CurrentTime = 0;

}

void UEnemyFSM::PlayMontageDamage()
{
	Me->PlayAnimMontage(EnemyActionMontage,1,TEXT("Damage"));
}

void UEnemyFSM::PlayMontageDie()
{
	Me->PlayAnimMontage(EnemyActionMontage, 1, TEXT("Die"));

}

