// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "mesh_Pawn.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UMaterial;

UCLASS()
class TICTACTOE_API Amesh_Pawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	Amesh_Pawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables");
	TArray <UStaticMeshComponent*> Objects;

	//Other components attached to main mesh.
	UPROPERTY(EditAnywhere);
	USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere);

	UCameraComponent* Camera;

	//Color property.
	UPROPERTY(EditAnywhere);
	UMaterialInterface* Red;

	UPROPERTY(EditAnywhere);
	UMaterialInterface* Blue;

	UPROPERTY(EditAnywhere);
	UMaterialInterface* White;


	//Voids for key-pressed cases.
	void OnePressed();
	void TwoPressed();
	void ThreePressed();
	void FourPressed();
	void FivePressed();
	void SixPressed();
	void SevenPressed();
	void EightPressed();
	void NinePressed();

	//Turn management.
	int TurnCounter;

	TArray<bool> ObjectStatus;

	void TurnManager(int ObjectIndex);
};
