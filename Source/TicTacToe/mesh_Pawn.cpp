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
	SpringArm->SetRelativeRotation(FRotator(-90.f, -90.f, 0.f));

	//Initializing the camera and attaching it to brick 4.
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	////////////////////////////////////////////////////////////////////////////////////////
	
	//Star position.
	float OffsetY = -200.f;
	float OffsetX = -200.f;

	//Loop for y-Offset
	for (int i = NULL; i < Objects.Num(); i++)
	{

		Objects[i]->SetRelativeLocation(FVector(OffsetX, OffsetY, 0.f));

		//Loop for x-Offset; 3 
		for (int j = NULL; j < 2; j++)
		{
			//Updating index ,i, so Objects still know the amount of iterations per line.
			OffsetX += 200.f;
			i++;
			Objects[i]->SetRelativeLocation(FVector(OffsetX, OffsetY, 0.f));

			//Updating Offset, so the Offset is correctly moved for the three iterations per y-value.
		}

		OffsetX = -200.f;
		OffsetY += 200.f;
		
	}

	////////////////////////////////////////////////////////////////////////////////////////
	
	//Setting start integer fro TurnCounter.
	TurnCounter = 0;

	//Initializing booleans. 
	ObjectStatus.Init(NULL, 9);
	redCounter.Init(NULL, 9);
	blueCounter.Init(NULL, 9);

	////////////////////////////////////////////////////////////////////////////////////////
	
	//Autopossesing player so the camera is fixed.
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned.
void Amesh_Pawn::BeginPlay()
{

	if (Won != true && Draw != true)
	{
		Super::BeginPlay();

		GEngine->AddOnScreenDebugMessage(0,10.f, FColor::Emerald, "Welcome to TicTacToe");
		GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Blue, "Press the mouse-left click, then Blue will start");

		for (int i = NULL; i < 9; i++)
		{
			Objects[i]->SetMaterial(0, White);
		}
	}

}

// Called every frame.
void Amesh_Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Setting start integer fro TurnCounter
	WinCondition();
	DrawCondition();
}

// Called to bind "key" pressed to voids for usage later.
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
	PlayerInputComponent->BindAction("r", IE_Pressed, this, &Amesh_Pawn::RestartGame);
}

//For respective key pressed. turn manager will increase by one. 
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

void Amesh_Pawn::RestartGame()
{
		Super::BeginPlay();
		GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Emerald, "You restarted");
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Blue, "Blue`s turn");

		//Setting start integer fro TurnCounter.
		TurnCounter = 0;

		//Initializing booleans. 
		ObjectStatus.Init(NULL, 9);
		redCounter.Init(NULL, 9);
		blueCounter.Init(NULL, 9);


		GEngine->AddOnScreenDebugMessage(0, 10.f, FColor::Emerald, "Welcome to TicTacToe");
		GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Blue, "Press the mouse-left click, then Blue will start");

		for (int i = NULL; i < 9; i++)
		{
			Objects[i]->SetMaterial(0, White);
		}

}

//Managing turns between player with given conditions.
void Amesh_Pawn::TurnManager(int ObjectIndex)
{
	if (Won != true && Draw != true)
	{
		//Checking for the corresponding turn.
		if (ObjectStatus[ObjectIndex] == true)
		{
			UE_LOG(LogTemp, Warning, TEXT("That Brick is already taken ^_^"));
			GEngine->AddOnScreenDebugMessage(0, 3.f, FColor::Green, "That Brick is already taken ^_^");
			return;
		}

		if (TurnCounter % 2 == 0)
		{
			Objects[ObjectIndex]->SetMaterial(0, Blue);
			blueCounter[ObjectIndex] = true;
			GEngine->AddOnScreenDebugMessage(0, 3.f, FColor::Blue, "Blue`s turn.");
			GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Emerald, "Red`s next.");

		}

		else if (TurnCounter % 2 == 1)
		{
			Objects[ObjectIndex]->SetMaterial(0, Red);
			redCounter[ObjectIndex] = true;
			GEngine->AddOnScreenDebugMessage(0, 3.f, FColor::Red, "Red Turn");
			GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Emerald, "Blue`s next.");
		}

		//Cycling turns so the code updates if given criteria is met.
		ObjectStatus[ObjectIndex] = true;
		TurnCounter++;
	}
}

void Amesh_Pawn::WinCondition()
{
	//Checking for all the possible winning combinations for both colors.
		if (
			blueCounter[0] && blueCounter[1] && blueCounter[2] ||
			blueCounter[0] && blueCounter[3] && blueCounter[6] ||
			blueCounter[0] && blueCounter[1] && blueCounter[2] ||
			blueCounter[2] && blueCounter[5] && blueCounter[8] ||
			blueCounter[0] && blueCounter[4] && blueCounter[8] ||
			blueCounter[2] && blueCounter[4] && blueCounter[6] == true
			)
		{
			Won = true;
			UE_LOG(LogTemp, Warning, TEXT("Blue won ^_^"));
			GEngine->AddOnScreenDebugMessage(0, 3.f, FColor::Blue, "Blue won ^_^");

		}

		else if (
			redCounter[0] && redCounter[1] && redCounter[2] ||
			redCounter[0] && redCounter[3] && redCounter[6] ||
			redCounter[0] && redCounter[1] && redCounter[2] ||
			redCounter[2] && redCounter[5] && redCounter[8] ||
			redCounter[0] && redCounter[4] && redCounter[8] ||
			redCounter[2] && redCounter[4] && redCounter[6] == true
			)
		{
			Won = true;
			UE_LOG(LogTemp, Warning, TEXT("Red won ^_^"));
			GEngine->AddOnScreenDebugMessage(0, 3.f, FColor::Red, "Red won ^_^");
		}
}

void Amesh_Pawn::DrawCondition()
{
	//Creating a condition for draw.
	if (Won != true && TurnCounter == 8)
	{
		Draw = true;
		UE_LOG(LogTemp, Warning, TEXT("Draw ^_^"));
		GEngine->AddOnScreenDebugMessage(0, 3.f, FColor::Cyan, "It`s a draw ^_^");
	}
}


