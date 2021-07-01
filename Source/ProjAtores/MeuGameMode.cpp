// Fill out your copyright notice in the Description page of Project Settings.


#include "MeuGameMode.h"

#include "Granada.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"

// StartPlay é uma função que você pode chamar dentro de GameModes. Desta forma será chamada antes de BeginPlay em atores.
void AMeuGameMode::StartPlay()
{
	Super::StartPlay();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("I'm in Game Mode \n Grenades launched!"));
}


// Game mode é uma classe que cotém as regras e configurações do nosso jogo
// Cada level pode ter seu próprio game mode
// Para isso em World Settgins você pode sobrescrever o game mode padrão pelo seu game mode
// Caso deseje substituir o game mode pra todos os levels use Edit->Project Settings
void AMeuGameMode::BeginPlay()
{
	Super::BeginPlay();
	// Declara variável do nome LocalGranadas do tipo array dinâmico capas de armazenar ponteiros do tipo AActor.
	TArray<AActor*> LocalGranadas;

	// A função vai percorre o mundo em busca de TargetPoints e cada Objeto deste tipo encontrado
	// Irá adicionar no array dinâmico LocalGranadas o endereço de memória de cada Target Point
	// Nesse caso o nosso serão coletados 3 endereços de memória e armazenados neste TArray LocalGranadas.
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), LocalGranadas);
	UE_LOG(LogTemp, Warning, TEXT("Num de Target Point Encontrados %d"), LocalGranadas.Num());

	if(LocalGranadas.Num() != 0)
	{
		// FActorSpawnParamets são configurações que você pode criar adicionalmente para quando
		// criar um ator através da fun~ção SpawnActor
		// Contudo na maioria das vezes será apenas necessário os valores padrão.
		// Assim criamos uma variável prams e inicamos ela com valores padrão atra´ves do resulta da chamada
		//   doc construtor de FActorSpawnParamets.
		FActorSpawnParameters params = FActorSpawnParameters();
		
		// Vai Iterar(Percorrer o TArray LocalGranadas e a cada passada pelos índices de 0 a 6
		// pois são 7 elementos o comando for vai colocando dentro da variável It o endereço de memória
		//   de cada posição do Array dinâmico LocalGranadas.
		// Assim It é um iterador algo como um cursor que vai está apontando para o array
		// Então It contém um endereço de memória e desta forma ele é um PONTEIRO
		// Para encontrar e verificar o conteúdo que It está apontando fazemos (*it).GetActorLocation()
		// Para simplificar usamos a notação flecha It->GetActorLocation();
		// Podemos ler assim: It vá lá no endereço de memória que você está apontando e me dica a localização deste local.
		
		for (auto It : LocalGranadas)
		{
			GetWorld()->SpawnActor<AGranada>(It->GetActorLocation(), It->GetActorRotation(), params);
			GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Red, TEXT("Ator Gerado"));
			UE_LOG(LogTemp, Warning, TEXT("Transform %s"), *It->GetActorLocation().ToString());
		}
	}
	
}
