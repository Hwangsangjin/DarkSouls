// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle UMETA(DisplayName= "IDLE"),
	Move UMETA(DisplayName = "MOVE"),
	Attack UMETA(DisplayName = "ATTACK"),
	Damage UMETA(DisplayName = "DAMAGE"),
	Die UMETA(DisplayName = "DIE"),

};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOULS_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	EEnemyState State;


public:
	UPROPERTY()
	class ABossEnemy* Me;
	UPROPERTY()
	class APlayerCharacter* Target;
	
	float AttackDistance = 250.0f;


public:
	void TickIdle();
	void TickMove();
	void TickAttack();
	void TickDamge();
	void TickDie();
	UFUNCTION(BlueprintCallable)
	void OnTakeDamage(int32 damage);
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float HP = 8;
	
	float CurrentTime;
	UPROPERTY(EditAnywhere)
	float DamageTime = 2;
	UPROPERTY(EditAnywhere)
	float DieTime = 2;
	UPROPERTY(EditAnywhere)
	float AttackTime = 4.4;
	void SetState(EEnemyState next);

	FVector DieEndLoc;

	UPROPERTY(EditAnywhere)
	//TSubclassOf<class UEnemyAnim> EnemyAnim;
	class UEnemyAnim* EnemyAnim;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* EnemyActionMontage;

	void PlayMontageDamage();
	void PlayMontageDie();


};
