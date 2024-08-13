#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct vehicle {
  char plate[10];
  char brand[20];
  char model[20];
  int year;
  char color[20];
  char type;
  int value;
  char state;
};

vehicle createVehicle() {
  vehicle vehicleToAdd = {0};

  printf("Enter plate: ");
  fgets (vehicleToAdd.plate, 10, stdin);
  while (vehicleToAdd.plate[0] == '\n') {
    printf("Plate cannot be empty\n");
    printf("Enter plate: ");
    fgets (vehicleToAdd.plate, 10, stdin);
  }
  while (vehicleToAdd.plate[strlen(vehicleToAdd.plate) - 1] != '\n') {
    printf("Plate is too long\n");
    printf("Enter plate: ");
    fgets (vehicleToAdd.plate, 10, stdin);
  }

  printf("Enter brand: ");
  fgets (vehicleToAdd.brand, 20, stdin);
  while (vehicleToAdd.brand[0] == '\n') {
    printf("Brand cannot be empty\n");
    printf("Enter brand: ");
    fgets (vehicleToAdd.brand, 20, stdin);
  }
  while (vehicleToAdd.brand[strlen(vehicleToAdd.brand) - 1] != '\n') {
    printf("Brand is too long\n");
    printf("Enter brand: ");
    fgets (vehicleToAdd.brand, 20, stdin);
  }

  printf("Enter model: ");
  fgets (vehicleToAdd.model, 20, stdin);
  while (vehicleToAdd.model[0] == '\n') {
    printf("Model cannot be empty\n");
    printf("Enter model: ");
    fgets (vehicleToAdd.model, 20, stdin);
  }
  while (vehicleToAdd.model[strlen(vehicleToAdd.model - 1)] != '\n') {
    printf("Model is too long\n");
    printf("Enter model: ");
    fgets (vehicleToAdd.model, 20, stdin);
  }

  printf("Enter year: ");
  scanf("%d", &vehicleToAdd.year);
  while (vehicleToAdd.year < 0) {
    printf("Year cannot be negative\n");
    printf("Enter year: ");
    scanf("%d", &vehicleToAdd.year);
  }

  printf("Enter color: ");
  fgets (vehicleToAdd.color, 20, stdin);
  while (vehicleToAdd.color[0] == '\n') {
    printf("Color cannot be empty\n");
    printf("Enter color: ");
    fgets (vehicleToAdd.color, 20, stdin);
  }
  while (vehicleToAdd.color[strlen(vehicleToAdd.color) - 1] != '\n') {
    printf("Color is too long\n");
    printf("Enter color: ");
    fgets (vehicleToAdd.color, 20, stdin);
  }

  printf("Enter type: ");
  scanf("%c", &vehicleToAdd.type);
  if (vehicleToAdd.type != 'P' && vehicleToAdd.type != 'C') {
    printf("Type must be P or C\n");
    printf("Enter type: ");
    scanf("%c", &vehicleToAdd.type);
  }

  printf("Enter value: ");
  scanf("%d", &vehicleToAdd.value);
  while (vehicleToAdd.value < 0) {
    printf("Value cannot be negative\n");
    printf("Enter value: ");
    scanf("%d", &vehicleToAdd.value);
  }

  printf("Enter state: ");
  scanf("%c", &vehicleToAdd.state);
  if (vehicleToAdd.state != 'A' && vehicleToAdd.state != 'E') {
    printf("State must be A or E\n");
    printf("Enter state: ");
    scanf("%c", &vehicleToAdd.state);
  }

  return vehicleToAdd;
}

int writeVehicles(const char *filename, vehicle *vehicleToAdd, int total) {
  FILE *file = fopen(filename, "wb");
  if (file == NULL) {
    printf("Error opening file\n");
    return 1;
  }

  if(fwrite(&total, sizeof(int), 1, file) != 1) {
    printf("Error writing to file\n");
    fclose(file);
    return 1;
  }

  if(fwrite(vehicleToAdd, sizeof(vehicle), total, file) != total) {
    printf("Error writing to file\n");
    fclose(file);
    return 1;
  }

  fclose(file);
  return 0;
}

vehicle *readVehicles(const char *filename, int *total) {
  FILE *file = fopen(filename, "rb");
  if (file == NULL) {
    printf("Error opening file\n");
    return NULL;
  }

  if(fread(total, sizeof(int), 1, file) != 1) {
    printf("Error reading from file\n");
    fclose(file);
    return NULL;
  }

  vehicle *vehicles = (vehicle *)malloc(sizeof(vehicle) * (*total));
  return vehicles;
}

void addVehicle(const char *filename) {
  vehicle vehicleToAdd = createVehicle();

  vehicle *vehicles;
  int total;

  vehicles = readVehicles("vehicles.dat", &total);

  if (vehicles == NULL) {
    vehicles = (vehicle *)malloc(sizeof(vehicle));
    total = 1;

    writeVehicles("vehicles.dat", vehicles, total);
  } else {
    total++;
    vehicles = (vehicle *)realloc(vehicles, sizeof(vehicle) * total);
    writeVehicles("vehicles.dat", vehicles, total);
  }

  free(vehicles);
}

void printVehicles (const char *filename) {
  vehicle *vehicles;
  int total = 0;

  vehicles = readVehicles("vehicles.dat", &total);

  for(int i = 0; i < total; i++) {
    if (vehicles[i].state  == 'E') {
      continue;
    }
    printf("Plate: %s\n", vehicles[i].plate);
    printf("Brand: %s\n", vehicles[i].brand);
    printf("Model: %s\n", vehicles[i].model);
    printf("Year: %d\n", vehicles[i].year);
    printf("Color: %s\n", vehicles[i].color);
    printf("Type: %c\n", vehicles[i].type);
    printf("Value: %d\n", vehicles[i].value);
    printf("State: %c\n", vehicles[i].state);
    printf("\n");
  }
}

int main () {
  printf("\n");
  printf("Welcome to the vehicle registration system\n");
  printf("\n");
  printf("1. Add vehicle\n");
  printf("2. List vehicles\n");
  printf("3. Exit\n");
  printf("\n");
  printf("Enter an option: ");

  int option;
  scanf("%d", &option);

  switch (option) {
    case 1:
      addVehicle("vehicles.dat");
      main();
      break;
    case 2:
      printVehicles("vehicles.dat");
      main();
      break;
    case 3:
      break;
    default:
      printf("Invalid option\n");
      break;
  }
}
