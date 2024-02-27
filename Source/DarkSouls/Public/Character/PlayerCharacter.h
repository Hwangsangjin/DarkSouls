// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Character/CharacterState.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
class UBoxComponent;
UCLASS()
class DARKSOULS_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// New Code
	void EnableSwordCollision();
	void DisableSwordCollision();

	// Sets default values for this character's properties
	APlayerCharacter();

	// Allow actors to initialize themselves on the C++ side after all of their components have been initialized, only called during gameplay
	virtual void PostInitializeComponents() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE void SetCharacterState(ECharacterState NewCharacterState) { CharacterState = NewCharacterState; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Action Functions
	void Move(const struct FInputActionValue& Value);
	void Look(const struct FInputActionValue& Value);
	void Lock();
	void Run();
	void Roll();
	void Jump();
	void Parry();
	void Attack();
	void Charge();
	void Drink();

	// Delegate
	UFUNCTION()
	void OnRollMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnParryMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnChargeMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnDrinkMontageEnded(UAnimMontage* Montage, bool bInterrupted);

public:
	// New Code
	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* SwordCollision;


	// SpringArm
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", Meta = (AllowPrivateAccess = true))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	// Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UCameraComponent> FollowCamera;

	// Mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UStaticMeshComponent> HipSheath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UStaticMeshComponent> HipSword;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UStaticMeshComponent> BackShield;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UStaticMeshComponent> LeftShield;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UStaticMeshComponent> RightSword;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UStaticMeshComponent> RightEstus;

	// Collision
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UBoxComponent> LeftShieldCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UCapsuleComponent> RightSwordCollision;

	// Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInputAction> LockAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInputAction> RunAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInputAction> RollAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInputAction> ParryAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInputAction> ChargeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInputAction> DrinkAction;

	// AnimInstance
	UPROPERTY()
	TObjectPtr<class UPlayerAnimInstance> AnimInstance;

	// State
	//UPROPERTY(BlueprintReadOnly, Category = "State", Meta = (AllowPrivateAccess = true));
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsLocking;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsRunning;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsRolling;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsParrying;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsCharging;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsDrinking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bGDHIsAttack;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bGDHIsParry;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Hp = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bGDHBloodOn;

	//NEW CODE
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UNiagaraComponent* Blood;
};
