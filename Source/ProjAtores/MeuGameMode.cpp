// Fill out your copyright notice in the Description page of Project Settings.


#include "MeuGameMode.h"

#include "Granada.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"

// StartPlay � uma fun��o que voc� pode chamar dentro de GameModes. Desta forma ser� chamada antes de BeginPlay em atores.
void AMeuGameMode::StartPlay()
{
	Super::StartPlay();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("I'm in Game Mode \n Grenades launched!"));
}


// Game mode � uma classe que cot�m as regras e configura��es do nosso jogo
// Cada level pode ter seu pr�prio game mode
// Para isso em World Settgins voc� pode sobrescrever o game mode padr�o pelo seu game mode
// Caso deseje substituir o game mode pra todos os levels use Edit->Project Settings
void AMeuGameMode::BeginPlay()
{
	Super::BeginPlay();
	// Declara vari�vel do nome LocalGranadas do tipo array din�mico capas de armazenar ponteiros do tipo AActor.
	TArray<AActor*> LocalGranadas;

	// A fun��o vai percorre o mundo em busca de TargetPoints e cada Objeto deste tipo encontrado
	// Ir� adicionar no array din�mico LocalGranadas o endere�o de mem�ria de cada Target Point
	// Nesse caso o nosso ser�o coletados 3 endere�os de mem�ria e armazenados neste TArray LocalGranadas.
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), LocalGranadas);
	UE_LOG(LogTemp, Warning, TEXT("Num de Target Point Encontrados %d"), LocalGranadas.Num());

	if(LocalGranadas.Num() != 0)
	{
		// FActorSpawnParamets s�o configura��es que voc� pode criar adicionalmente para quando
		// criar um ator atrav�s da fun~��o SpawnActor
		// Contudo na maioria das vezes ser� apenas necess�rio os valores padr�o.
		// Assim criamos uma vari�vel prams e inicamos ela com valores padr�o atra�ves do resulta da chamada
		//   doc construtor de FActorSpawnParamets.
		FActorSpawnParameters params = FActorSpawnParameters();
		
		// Vai Iterar(Percorrer o TArray LocalGranadas e a cada passada pelos �ndices de 0 a 6
		// pois s�o 7 elementos o comando for vai colocando dentro da vari�vel It o endere�o de mem�ria
		//   de cada posi��o do Array din�mico LocalGranadas.
		// Assim It � um iterador algo como um cursor que vai est� apontando para o array
		// Ent�o It cont�m um endere�o de mem�ria e desta forma ele � um PONTEIRO
		// Para encontrar e verificar o conte�do que It est� apontando fazemos (*it).GetActorLocation()
		// Para simplificar usamos a nota��o flecha It->GetActorLocation();
		// Podemos ler assim: It v� l� no endere�o de mem�ria que voc� est� apontando e me dica a localiza��o deste local.
		
		for (auto It : LocalGranadas)
		{
			GetWorld()->SpawnActor<AGranada>(It->GetActorLocation(), It->GetActorRotation(), params);
			GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Red, TEXT("Ator Gerado"));
			UE_LOG(LogTemp, Warning, TEXT("Transform %s"), *It->GetActorLocation().ToString());
		}
	}
	
}
