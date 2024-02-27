// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"
#include "Character/EnemyCharacter.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Animation/PlayerAnimInstance.h"
#include "Animation/AnimMontage.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include <Components/BoxComponent.h>
#include <../../../../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h>

void APlayerCharacter::EnableSwordCollision()
{
	SwordCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void APlayerCharacter::DisableSwordCollision()
{
	SwordCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// New Code
	SwordCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordCollision"));
	SwordCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Blood = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Blood"));

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 컨트롤러 회전 사용 해제
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 캐릭터가 움직이는 방향으로 회전
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;

	// 스프링암 생성
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 250.0f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->bEnableCameraRotationLag = true;

	// 카메라 생성
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);

	// 메시 설정
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> TempSkeletalMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Resource/AshenOne/Meshes/SKM_AshenOne.SKM_AshenOne'"));
	if (TempSkeletalMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempSkeletalMesh.Object);
	}

	// 소켓 설정
	if (GetMesh()->DoesSocketExist(TEXT("HipSocket")))
	{
		HipSheath = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HipSheath"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> TempHipSheath(TEXT("/Script/Engine.StaticMesh'/Game/Resource/AshenOne/Meshes/SM_BroadswordSheath.SM_BroadswordSheath'"));
		if (TempHipSheath.Succeeded())
		{
			HipSheath->SetStaticMesh(TempHipSheath.Object);
			HipSheath->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			HipSheath->SetupAttachment(GetMesh(), TEXT("HipSocket"));
		}

		HipSword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HipSword"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> TempHipSword(TEXT("/Script/Engine.StaticMesh'/Game/Resource/AshenOne/Meshes/SM_BroadswordRetopo.SM_BroadswordRetopo'"));
		if (TempHipSword.Succeeded())
		{
			HipSword->SetStaticMesh(TempHipSword.Object);
			HipSword->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			HipSword->SetupAttachment(HipSheath);
			HipSword->SetHiddenInGame(true);
		}
	}

	if (GetMesh()->DoesSocketExist(TEXT("BackSocket")))
	{
		BackShield = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BackShield"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> TempBackShield(TEXT("/Script/Engine.StaticMesh'/Game/Resource/AshenOne/Meshes/SM_GrassCrestShield.SM_GrassCrestShield'"));
		if (TempBackShield.Succeeded())
		{
			BackShield->SetStaticMesh(TempBackShield.Object);
			BackShield->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			BackShield->SetupAttachment(GetMesh(), TEXT("BackSocket"));
			BackShield->SetHiddenInGame(true);
		}
	}

	if (GetMesh()->DoesSocketExist(TEXT("LeftHandSocket")))
	{
		LeftShield = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftShield"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> TempLeftShield(TEXT("/Script/Engine.StaticMesh'/Game/Resource/AshenOne/Meshes/SM_GrassCrestShield.SM_GrassCrestShield'"));
		if (TempLeftShield.Succeeded())
		{
			LeftShield->SetStaticMesh(TempLeftShield.Object);
			LeftShield->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			LeftShield->SetupAttachment(GetMesh(), TEXT("LeftHandSocket"));
		}
	}

	if (GetMesh()->DoesSocketExist(TEXT("RightHandSocket")))
	{
		RightSword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightSword"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> TempRightSword(TEXT("/Script/Engine.StaticMesh'/Game/Resource/AshenOne/Meshes/SM_BroadswordRetopo.SM_BroadswordRetopo'"));
		if (TempRightSword.Succeeded())
		{
			RightSword->SetStaticMesh(TempRightSword.Object);
			RightSword->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			RightSword->SetupAttachment(GetMesh(), TEXT("RightHandSocket"));

			RightSwordCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RightSwordCollision"));
			RightSwordCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			RightSwordCollision->SetRelativeLocation(FVector(-45.0, 0, 0));
			RightSwordCollision->SetRelativeRotation(FRotator(90, 0, 0));
			RightSwordCollision->SetupAttachment(RightSword);
		}
	}

	if (GetMesh()->DoesSocketExist(TEXT("RightHandEstusSocket")))
	{
		RightEstus = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightEstus"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> TempRightEstus(TEXT("/Script/Engine.StaticMesh'/Game/Resource/AshenOne/Meshes/SM_Estus.SM_Estus'"));
		if (TempRightEstus.Succeeded())
		{
			RightEstus->SetStaticMesh(TempRightEstus.Object);
			RightEstus->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			RightEstus->SetupAttachment(GetMesh(), TEXT("RightHandEstusSocket"));
			RightEstus->SetHiddenInGame(true);
		}
	}

	// 입력 설정
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> TempIMC(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/AA_HSJ/Blueprints/Actor/Input/IMC_Player.IMC_Player'"));
	if (TempIMC.Succeeded())
	{
		InputMappingContext = TempIMC.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> TempMove(TEXT("/Script/EnhancedInput.InputAction'/Game/AA_HSJ/Blueprints/Actor/Input/Actions/IA_Move.IA_Move'"));
	if (TempMove.Succeeded())
	{
		MoveAction = TempMove.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> TempLook(TEXT("/Script/EnhancedInput.InputAction'/Game/AA_HSJ/Blueprints/Actor/Input/Actions/IA_Look.IA_Look'"));
	if (TempLook.Succeeded())
	{
		LookAction = TempLook.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> TempLock(TEXT("/Script/EnhancedInput.InputAction'/Game/AA_HSJ/Blueprints/Actor/Input/Actions/IA_Lock.IA_Lock'"));
	if (TempLock.Succeeded())
	{
		LockAction = TempLock.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> TempRun(TEXT("/Script/EnhancedInput.InputAction'/Game/AA_HSJ/Blueprints/Actor/Input/Actions/IA_Run.IA_Run'"));
	if (TempRun.Succeeded())
	{
		RunAction = TempRun.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> TempRoll(TEXT("/Script/EnhancedInput.InputAction'/Game/AA_HSJ/Blueprints/Actor/Input/Actions/IA_Roll.IA_Roll'"));
	if (TempRoll.Succeeded())
	{
		RollAction = TempRoll.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> TempJump(TEXT("/Script/EnhancedInput.InputAction'/Game/AA_HSJ/Blueprints/Actor/Input/Actions/IA_Jump.IA_Jump'"));
	if (TempJump.Succeeded())
	{
		JumpAction = TempJump.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> TempParry(TEXT("/Script/EnhancedInput.InputAction'/Game/AA_HSJ/Blueprints/Actor/Input/Actions/IA_Parry.IA_Parry'"));
	if (TempParry.Succeeded())
	{
		ParryAction = TempParry.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> TempAttack(TEXT("/Script/EnhancedInput.InputAction'/Game/AA_HSJ/Blueprints/Actor/Input/Actions/IA_Attack.IA_Attack'"));
	if (TempAttack.Succeeded())
	{
		AttackAction = TempAttack.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> TempCharge(TEXT("/Script/EnhancedInput.InputAction'/Game/AA_HSJ/Blueprints/Actor/Input/Actions/IA_Charge.IA_Charge'"));
	if (TempCharge.Succeeded())
	{
		ChargeAction = TempCharge.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> TempDrink(TEXT("/Script/EnhancedInput.InputAction'/Game/AA_HSJ/Blueprints/Actor/Input/Actions/IA_Drink.IA_Drink'"));
	if (TempDrink.Succeeded())
	{
		DrinkAction = TempDrink.Object;
	}

}

// Allow actors to initialize themselves on the C++ side after all of their components have been initialized, only called during gameplay
void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 델리게이트
	AnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (IsValid(AnimInstance))
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &APlayerCharacter::OnRollMontageEnded);
		AnimInstance->OnMontageEnded.AddDynamic(this, &APlayerCharacter::OnParryMontageEnded);
		AnimInstance->OnMontageEnded.AddDynamic(this, &APlayerCharacter::OnAttackMontageEnded);
		AnimInstance->OnMontageEnded.AddDynamic(this, &APlayerCharacter::OnChargeMontageEnded);
		AnimInstance->OnMontageEnded.AddDynamic(this, &APlayerCharacter::OnDrinkMontageEnded);
	}
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 향상된 입력 매핑 컨텍스트 추가
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}


	// New Code
	SwordCollision->AttachToComponent(RightSword, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	SwordCollision->SetRelativeLocation(FVector(-50, 0, 0));
	SwordCollision->SetBoxExtent(FVector(30, 8, 8));

	/*Relative Location : (X=-49.000000,Y=0.000000,Z=0.000000)
Box Scale : (X=30.000000,Y=8.000000,Z=8.000000)*/

	Blood->AttachToComponent(SwordCollision, FAttachmentTransformRules::SnapToTargetNotIncludingScale);


}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetCharacterMovement()->Velocity == FVector::ZeroVector)
	{
		bIsRunning = false;
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 향상된 입력 액션 바인딩
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		EnhancedInputComponent->BindAction(LockAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Lock);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Run);
		EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Roll);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Jump);
		EnhancedInputComponent->BindAction(ParryAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Parry);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Attack);
		EnhancedInputComponent->BindAction(ChargeAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Charge);
		EnhancedInputComponent->BindAction(DrinkAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Drink);
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;

	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (IsValid(Controller))
	{
		if (bIsLocking)
		{
			auto Enemy = UGameplayStatics::GetActorOfClass(GetWorld(), AEnemyCharacter::StaticClass());
			if (IsValid(Enemy))
			{
				const FVector Start = GetActorLocation();
				const FVector Target = Enemy->GetActorLocation();
				FRotator LookAtRotation; // = UKismetMathLibrary::FindLookAtRotation(Start, Target);

				FVector XAxis = Target - Start;
				const FVector NewX = XAxis.GetSafeNormal();

				const FVector UpVector = (FMath::Abs(NewX.Z) < (1.0f - UE_KINDA_SMALL_NUMBER)) ? FVector(0, 0, 1.0f) : FVector(1.0f, 0, 0);

				const FVector NewY = FVector::CrossProduct(UpVector, NewX).GetSafeNormal();
				const FVector NewZ = FVector::CrossProduct(NewX, NewY);

				LookAtRotation = FMatrix(NewX, NewY, NewZ, FVector::ZeroVector).Rotator();

				Controller->SetControlRotation(LookAtRotation);
				const FRotator Rotation = Controller->GetControlRotation();
				const FRotator YawRotation(0, Rotation.Yaw, 0);

				const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
				const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

				SetActorRotation(FRotator(0, LookAtRotation.Yaw, 0));
				AddMovementInput(ForwardDirection, MovementVector.Y);
				AddMovementInput(RightDirection, MovementVector.X);
			}
		}
		else
		{
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			AddMovementInput(ForwardDirection, MovementVector.Y);
			AddMovementInput(RightDirection, MovementVector.X);
		}
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (IsValid(Controller))
	{
		AddControllerPitchInput(LookAxisVector.Y);
		AddControllerYawInput(LookAxisVector.X);
	}
}

void APlayerCharacter::Lock()
{
	bIsLocking = !bIsLocking;

	if (bIsLocking)
	{
		bIsLocking = true;
		CameraBoom->TargetArmLength = 200.0f;
		CameraBoom->bUsePawnControlRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	else
	{
		bIsLocking = false;
		CameraBoom->TargetArmLength = 250.0f;
		CameraBoom->bUsePawnControlRotation = true;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}

void APlayerCharacter::Run()
{
	bIsRunning = true;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

void APlayerCharacter::Roll()
{
	if (GetCharacterMovement()->IsFalling() ||
		bIsRunning ||
		bIsRolling ||
		bIsParrying ||
		bIsAttacking ||
		bIsCharging ||
		bIsDrinking)
	{
		return;
	}

	if (GetCharacterMovement()->Velocity != FVector::ZeroVector)
	{
		if (bIsLocking)
		{
			GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Overlap);
		}

		bIsRolling = true;
		AnimInstance->PlayRollMontage();
	}
}

void APlayerCharacter::Jump()
{
	if (bIsRunning)
	{
		Super::Jump();
		bIsRunning = false;
	}
}

void APlayerCharacter::Parry()
{
	if (GetCharacterMovement()->IsFalling() ||
		bIsRolling ||
		bIsParrying ||
		bIsAttacking ||
		bIsCharging ||
		bIsDrinking)
	{
		return;
	}

	if (GetCharacterMovement()->Velocity == FVector::ZeroVector)
	{
		bIsParrying = true;
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		AnimInstance->PlayParryMontage();
	}
}

void APlayerCharacter::Attack()
{
	if (GetCharacterMovement()->IsFalling() ||
		bIsRolling ||
		bIsParrying ||
		bIsAttacking ||
		bIsCharging ||
		bIsDrinking)
	{
		return;
	}

	bIsAttacking = true;
	LaunchCharacter(GetActorForwardVector() * 500.0f, false, false);
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	AnimInstance->PlayAttackMontage();
}

void APlayerCharacter::Charge()
{
	if (GetCharacterMovement()->IsFalling() ||
		bIsRunning ||
		bIsRolling ||
		bIsRolling ||
		bIsParrying ||
		bIsAttacking ||
		bIsCharging ||
		bIsDrinking)
	{
		return;
	}

	bIsCharging = true;
	AnimInstance->PlayChargeMontage();
	LaunchCharacter(GetActorForwardVector() * 2500.0f, false, false);
}

void APlayerCharacter::Drink()
{
	if (GetCharacterMovement()->IsFalling() ||
		bIsRunning ||
		bIsRolling ||
		bIsParrying ||
		bIsAttacking ||
		bIsCharging ||
		bIsDrinking)
	{
		return;
	}

	bIsDrinking = true;
	RightSword->SetHiddenInGame(true);
	RightEstus->SetHiddenInGame(false);
	AnimInstance->PlayDrinkMontage();
}

void APlayerCharacter::OnRollMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (bIsLocking)
	{
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Block);
	}

	bIsRolling = false;
}

void APlayerCharacter::OnParryMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bIsParrying = false;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void APlayerCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bIsAttacking = false;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void APlayerCharacter::OnChargeMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bIsCharging = false;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void APlayerCharacter::OnDrinkMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bIsDrinking = false;
	RightEstus->SetHiddenInGame(true);
	RightSword->SetHiddenInGame(false);
}

