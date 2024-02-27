// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PlayerAnimInstance.h"
#include "Character/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> TempRoll(TEXT("/Script/Engine.AnimMontage'/Game/AA_HSJ/Blueprints/Animation/AM_Roll.AM_Roll'"));
	if (TempRoll.Succeeded())
	{
		RollMontage = TempRoll.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> TempParry(TEXT("/Script/Engine.AnimMontage'/Game/AA_HSJ/Blueprints/Animation/AM_Parry.AM_Parry'"));
	if (TempParry.Succeeded())
	{
		ParryMontage = TempParry.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> TempAttack(TEXT("/Script/Engine.AnimMontage'/Game/AA_HSJ/Blueprints/Animation/AM_Attack.AM_Attack'"));
	if (TempAttack.Succeeded())
	{
		AttackMontage = TempAttack.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> TempCharge(TEXT("/Script/Engine.AnimMontage'/Game/AA_GDH/Animation/GDH_AM_Charge.GDH_AM_Charge'"));
	if (TempCharge.Succeeded())
	{
		ChargeMontage = TempCharge.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> TempDrink(TEXT("/Script/Engine.AnimMontage'/Game/AA_GDH/Animation/GDH_AM_Drink.GDH_AM_Drink'"));
	if (TempDrink.Succeeded())
	{
		DrinkMontage = TempDrink.Object;
	}
}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (PlayerCharacter)
	{
		CharacterMovement = PlayerCharacter->GetCharacterMovement();
	}
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (CharacterMovement)
	{
		CharacterState = PlayerCharacter->GetCharacterState();
		bIsFalling = CharacterMovement->IsFalling();

		const FVector Velocity = PlayerCharacter->GetVelocity();
		const FVector ForwardVector = PlayerCharacter->GetActorForwardVector();
		Speed = FVector::DotProduct(ForwardVector, Velocity);

		const FVector RightVector = PlayerCharacter->GetActorRightVector();
		Direction = FVector::DotProduct(RightVector, Velocity);
	}
}

void UPlayerAnimInstance::PlayRollMontage()
{
	Montage_Play(RollMontage);
}

void UPlayerAnimInstance::PlayParryMontage()
{
	Montage_Play(ParryMontage);
}

void UPlayerAnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage);
}

void UPlayerAnimInstance::PlayChargeMontage()
{
	Montage_Play(ChargeMontage);
}

void UPlayerAnimInstance::PlayDrinkMontage()
{
	Montage_Play(DrinkMontage);
}

void UPlayerAnimInstance::AnimNotify_AttackStart()
{

	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (Player)
	{
		// New Code
		Player->EnableSwordCollision();

		Player->bGDHIsAttack = true;
	}

	UE_LOG(LogTemp, Warning, TEXT("AnimNotify_AttackStart"));
	PlayerCharacter->RightSwordCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void UPlayerAnimInstance::AnimNotify_AttackEnd()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (Player)
	{
		// New Code
		Player->DisableSwordCollision();

		Player->bGDHIsAttack = false;
	}

	UE_LOG(LogTemp, Warning, TEXT("AnimNotify_AttackEnd"));
	PlayerCharacter->RightSwordCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UPlayerAnimInstance::AnimNotify_ChargeAttack()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (Player)
	{
		Player->bGDHIsAttack = true;
	}

	UE_LOG(LogTemp, Warning, TEXT("AnimNotify_ChargeAttack"));
}

void UPlayerAnimInstance::AnimNotify_ChargeAttacked()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (Player)
	{
		Player->bGDHIsAttack = false;
	}

	UE_LOG(LogTemp, Warning, TEXT("AnimNotify_ChargeAttacked"));
}

void UPlayerAnimInstance::AnimNotify_DrinkHp()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (Player)
	{
		if (Player->Hp <= 9) 
		{
			Player->Hp += 4;
			if (Player->Hp >= 11)
			{
				Player->Hp = 10;
			}
		}
	
	}

	UE_LOG(LogTemp, Warning, TEXT("AnimNotify_DrinkHp : %f"), Player->Hp);
}

void UPlayerAnimInstance::AnimNotify_ParryStart()
{
	UE_LOG(LogTemp, Warning, TEXT("AnimNotify_ParryStart"));
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (Player)
	{
		Player->bGDHIsParry = true;
	}
}

void UPlayerAnimInstance::AnimNotify_ParryEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("AnimNotify_ParryEnd"));
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (Player)
	{
		Player->bGDHIsParry = false;
	}
}

void UPlayerAnimInstance::AnimNotify_BloodOn()
{
	UE_LOG(LogTemp, Warning, TEXT("AnimNotify_BloodOn"));
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (Player)
	{
		Player->bGDHBloodOn = true;
	}

}

void UPlayerAnimInstance::AnimNotify_BloodOff()
{
	UE_LOG(LogTemp, Warning, TEXT("AnimNotify_BloodOff"));
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (Player)
	{
		Player->bGDHBloodOn = false;
	}

}

