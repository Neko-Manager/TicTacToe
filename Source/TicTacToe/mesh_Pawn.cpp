// Fill out your copyright notice in the Description page of Project Settings.


#include "mesh_Pawn.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include <UObject/ConstructorHelpers.h>
#include "Containers/Array.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
Amesh_Pawn::Amesh_Pawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Objects.Init(NULL, 9);

	for (int i = NULL; i < 9; i++)
	{
		FString Number = FString::FromInt(i);
		Objects[i] = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("Object_") + FString::FromInt(i)), false);
	}


	/*Objects[0] = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Object_1"));
	Objects[1] = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Object_2"));
	Objects[2] = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Object_3"));
	Objects[3] = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Object_4"));
	Objects[4] = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Object_5"));
	Objects[5] = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Object_6"));
	Objects[6] = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Object_7"));
	Objects[7] = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Object_8"));
	Objects[8] = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Object_9"));*/

	////////////////////////////////////////////////////////////////////////////////////////

	//Object 4 ("Middle one as root")
	RootComponent = Objects[4];

	//Making the Pawn spawns with runned code direct in file ("spawns with all nine bricks")
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshComponentAsset(TEXT("/Script/CoreUObject.Class'/Script/TicTacToe.mesh_Pawn'"));

	////////////////////////////////////////////////////////////////////////////////////////

	//If else statement with limit as 9 with counted elements 0->9 0 included and9 not included--> 0,1,2,3,4,5,6,7,8;
	for (int i = NULL; i < 9; i++)
	{
		//In this case element 4 is the middle one since we start at 0 and end at 8.
		if (i != 4)
		{
			Objects[i]->SetupAttachment(GetRootComponent());
		}
		Objects[i]->SetStaticMesh(MeshComponentAsset.Object);
	}

	////////////////////////////////////////////////////////////////////////////////////////

	//Initializing colors
	Red = CreateDefaultSubobject<UMaterial>(TEXT("RedMaterial"));
	Blue = CreateDefaultSubobject<UMaterial>(TEXT("BlueMaterial"));
	White = CreateDefaultSubobject<UMaterial>(TEXT("WhiteMaterial"));

	////////////////////////////////////////////////////////////////////////////////////////

	//Attached springarm to brick 4.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(Objects[4]);
	SpringArm->TargetArmLength = 500.f;
	SpringArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));

	//Initializing the camera and attaching it to brick 4.
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	//Arranging the Objects in a 3 x 3 formation.
	//Objects[0]->SetRelativeLocation(FVector(200.f, -200.f, 0.f));
	//Objects[1]->SetRelativeLocation(FVector(200.f, 0.f, 0.f));
	//Objects[2]->SetRelativeLocation(FVector(200.f, 200.f, 0.f));

	//Objects[3]->SetRelativeLocation(FVector(0.f, -200.f, 0.f));
	//Objects[4]->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	//Objects[5]->SetRelativeLocation(FVector(0.f, 200.f, 0.f));

	//Objects[6]->SetRelativeLocation(FVector(-200.f, -200.f, 0.f));
	//Objects[7]->SetRelativeLocation(FVector(-200.f, 0.f, 0.f));
	//Objects[8]->SetRelativeLocation(FVector(-200.f, 200.f, 0.f));


	//Star position.
	float OffsetY = -200.f;
	float OffsetX = -200.f;

	//Loop for y-Offset
	for (int i = NULL; i < 3; i++)
	{

		Objects[i]->SetRelativeLocation(FVector(OffsetX, OffsetY, 0.f));

		//Loop for x-Offset; 3 
		for (int j = NULL; j < 3; j++)
		{
			//Updating index ,i, so Objects still know the amount of iterations per line.
			i++;
			Objects[i]->SetRelativeLocation(FVector(OffsetX, OffsetY, 0.f));

			//Updating Offset, so the Offset is correctly moved for the three iterations per y-value.
			OffsetX += 200.f;
		}

		OffsetX = -200.f;
		OffsetY -= 200.f;
		
	}
	


	//Setting start integer fro TurnCounter
	TurnCounter = 0;
	ObjectStatus.Init(NULL, 9);

	//Autopossesing player so the camera is fixed.
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void Amesh_Pawn::BeginPlay()
{
	Super::BeginPlay();

	for (int i = NULL; i < 9; i++)
	{
		Objects[i]->SetMaterial(0, White);
	}
}

// Called every frame
void Amesh_Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void Amesh_Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("1", IE_Pressed, this, &Amesh_Pawn::OnePressed);
	PlayerInputComponent->BindAction("2", IE_Pressed, this, &Amesh_Pawn::TwoPressed);
	PlayerInputComponent->BindAction("3", IE_Pressed, this, &Amesh_Pawn::ThreePressed);
	PlayerInputComponent->BindAction("4", IE_Pressed, this, &Amesh_Pawn::FourPressed);
	PlayerInputComponent->BindAction("5", IE_Pressed, this, &Amesh_Pawn::FivePressed);
	PlayerInputComponent->BindAction("6", IE_Pressed, this, &Amesh_Pawn::SixPressed);
	PlayerInputComponent->BindAction("7", IE_Pressed, this, &Amesh_Pawn::SevenPressed);
	PlayerInputComponent->BindAction("8", IE_Pressed, this, &Amesh_Pawn::EightPressed);
	PlayerInputComponent->BindAction("9", IE_Pressed, this, &Amesh_Pawn::NinePressed);
}

void Amesh_Pawn::OnePressed()
{
	TurnManager(0);
}

void Amesh_Pawn::TwoPressed()
{
	TurnManager(1);
}

void Amesh_Pawn::ThreePressed()
{
	TurnManager(2);
}

void Amesh_Pawn::FourPressed()
{
	TurnManager(3);
}

void Amesh_Pawn::FivePressed()
{
	TurnManager(4);
}

void Amesh_Pawn::SixPressed()
{
	TurnManager(5);
}

void Amesh_Pawn::SevenPressed()
{
	TurnManager(6);
}

void Amesh_Pawn::EightPressed()
{
	TurnManager(7);
}

void Amesh_Pawn::NinePressed()
{
	TurnManager(8);
}

void Amesh_Pawn::TurnManager(int brickIndex)
{
	if (ObjectStatus[brickIndex] == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("That Brick is already taken ^_^"));
		return;
	}

	if (TurnCounter % 2 == 0)
	{
		Objects[brickIndex]->SetMaterial(0, Blue);
	}

	else if (TurnCounter % 2 == 1)
	{
		Objects[brickIndex]->SetMaterial(0, Red);
	}

	//Cycling turns so the code updates if given criteria is met.
	ObjectStatus[brickIndex] = true;
	TurnCounter++;

}