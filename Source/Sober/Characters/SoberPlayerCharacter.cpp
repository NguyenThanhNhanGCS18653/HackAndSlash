#include "SoberPlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"

#include "Sober/UI/HUD/SoberHUD.h"
#include "Sober/Core/SoberAttributeSet.h"
#include "Sober/Core/SoberGameplayTags.h"
#include "Sober/Core/Data/SoberInputConfig.h"
#include "Sober/Core/SoberAbilitySystemComponent.h"

ASoberPlayerCharacter::ASoberPlayerCharacter(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 400.f;
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    bUseControllerRotationYaw = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);
}

void ASoberPlayerCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    if (APlayerController* PC = Cast<APlayerController>(NewController))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            if (DefaultMappingContext)
            {
                Subsystem->AddMappingContext(DefaultMappingContext, MappingContextPriority);
            }
        }
    }

    InitializeOverlay();
}

void ASoberPlayerCharacter::OnRep_PlayerState()
{
    Super::OnRep_PlayerState();

    InitializeOverlay();
}

void ASoberPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

    if (!InputConfig)
    {
        UE_LOG(LogTemp, Error, TEXT("[%s] InputConfig chưa được gán trong Blueprint!"), *GetName());
        return;
    }
    FSoberGameplayTags GameplayTag = FSoberGameplayTags::Get();
    // ---- Bind Native Input (Move/Look/Jump) ----
    if (UInputAction* MoveAction = InputConfig->FindNativeInputActionForTag(GameplayTag.Input_Move))
    {
        EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASoberPlayerCharacter::Input_Move);
    }
    if (UInputAction* LookAction = InputConfig->FindNativeInputActionForTag(GameplayTag.Input_Look))
    {
        EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASoberPlayerCharacter::Input_Look);
    }
    if (UInputAction* JumpAction = InputConfig->FindNativeInputActionForTag(GameplayTag.Input_Jump))
    {
        EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
        EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
    }

    // ---- Bind Ability Input — loop generic, không hardcode từng ability ----
    for (const FYourGameInputAction& AbilityInput : InputConfig->AbilityInputActions)
    {
        if (!AbilityInput.InputAction || !AbilityInput.InputTag.IsValid())
        {
            continue;
        }

        EnhancedInput->BindAction(AbilityInput.InputAction, ETriggerEvent::Started,
            this, &ASoberPlayerCharacter::Input_AbilityInputTagPressed, AbilityInput.InputTag);

        EnhancedInput->BindAction(AbilityInput.InputAction, ETriggerEvent::Completed,
            this, &ASoberPlayerCharacter::Input_AbilityInputTagReleased, AbilityInput.InputTag);
    }
}

void ASoberPlayerCharacter::Input_Move(const FInputActionValue& Value)
{
    const FVector2D MoveVector = Value.Get<FVector2D>();
    if (!Controller) return;

    const FRotator ControlRotation = Controller->GetControlRotation();
    const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    AddMovementInput(ForwardDirection, MoveVector.Y);
    AddMovementInput(RightDirection, MoveVector.X);
}

void ASoberPlayerCharacter::Input_Look(const FInputActionValue& Value)
{
    const FVector2D LookVector = Value.Get<FVector2D>();
    if (!Controller) return;

    AddControllerYawInput(LookVector.X);
    AddControllerPitchInput(LookVector.Y);
}

void ASoberPlayerCharacter::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
    if (USoberAbilitySystemComponent* ASC = Cast<USoberAbilitySystemComponent>(GetAbilitySystemComponent()))
    {
        ASC->AbilityInputTagPressed(InputTag);
    }
}

void ASoberPlayerCharacter::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
    if (USoberAbilitySystemComponent* ASC = Cast<USoberAbilitySystemComponent>(GetAbilitySystemComponent()))
    {
        ASC->AbilityInputTagReleased(InputTag);
    }
}

void ASoberPlayerCharacter::InitializeOverlay()
{
    APlayerController* PC = Cast<APlayerController>(GetController());
    if (!PC || !PC->IsLocalController()) return;

    if (ASoberHUD* HUD = Cast<ASoberHUD>(PC->GetHUD()))
    {
        HUD->InitOverlay(PC, this, GetAbilitySystemComponent(), Cast<UAttributeSet>(AttributeSet));
    }
}
