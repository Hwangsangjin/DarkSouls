// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Character/CharacterState.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULS_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPlayerAnimInstance();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	void PlayRollMontage();
	void PlayParryMontage();
	void PlayAttackMontage();
	void PlayChargeMontage();
	void PlayDrinkMontage();

	UFUNCTION()
	void AnimNotify_AttackStart();
	UFUNCTION()
	void AnimNotify_AttackEnd();
	
	UFUNCTION()
	void AnimNotify_ChargeAttack();

	UFUNCTION()
	void AnimNotify_ChargeAttacked();

	UFUNCTION()
	void AnimNotify_DrinkHp();

	UFUNCTION()
	void AnimNotify_ParryStart();
	UFUNCTION()
	void AnimNotify_ParryEnd();

	UFUNCTION()
	void AnimNotify_BloodOn();

	UFUNCTION()
	void AnimNotify_BloodOff();
private:
	//UPROPERTY(BlueprintReadOnly, Category = "State", Meta = (AllowPrivateAccess = true));
	ECharacterState CharacterState;

	UPROPERTY(BlueprintReadOnly, Category = "Owner", Meta = (AllowPrivateAccess = true))
	TObjectPtr<class APlayerCharacter> PlayerCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UCharacterMovementComponent> CharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", Meta = (AllowPrivateAccess = true))
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", Meta = (AllowPrivateAccess = true))
	float Direction;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", Meta = (AllowPrivateAccess = true))
	bool bIsFalling;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage", Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAnimMontage> RollMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage", Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAnimMontage> ParryMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage", Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage", Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAnimMontage> ChargeMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage", Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAnimMontage> DrinkMontage;

};
