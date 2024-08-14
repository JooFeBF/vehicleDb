#include <stdio.h>
#include <ctype.h>
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

typedef struct vehicle vehicle;

void getStringInput(const char *prompt, char *buffer, size_t bufferSize, const char *errorMsg) {
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, bufferSize, stdin) == NULL) {
            printf("Error reading input\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }

        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        if (strlen(buffer) == 0) {
            printf("%s\n", errorMsg);
        } else if (len >= bufferSize - 1) {
            printf("Input is too long\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        } else {
            return;
        }
    }
}


void getValidatedInput(const char *prompt, int *value, const char *errorMsg, int minValue) {
    char buffer[20];
    while (1) {
        getStringInput(prompt, buffer, sizeof(buffer), errorMsg);
        if (sscanf(buffer, "%d", value) == 1 && *value >= minValue) {
            return;
        }
        printf("%s\n", errorMsg);
    }
}

void getValidatedCharInput(const char *prompt, char *value, const char *errorMsg, const char *validOptions) {
    char buffer[10];
    while (1) {
        getStringInput(prompt, buffer, sizeof(buffer), errorMsg);
        if (sscanf(buffer, "%c", value) == 1 && strchr(validOptions, *value)) {
            return;
        }
        printf("%s\n", errorMsg);
    }
}

vehicle createVehicle() {
    vehicle vehicleToAdd = {0};

    getStringInput("Enter plate: ", vehicleToAdd.plate, sizeof(vehicleToAdd.plate), "Plate cannot be empty");

    getStringInput("Enter brand: ", vehicleToAdd.brand, sizeof(vehicleToAdd.brand), "Brand cannot be empty");

    getStringInput("Enter model: ", vehicleToAdd.model, sizeof(vehicleToAdd.model), "Model cannot be empty");

    getValidatedInput("Enter year: ", &vehicleToAdd.year, "Year must be a positive number", 0);

    getStringInput("Enter color: ", vehicleToAdd.color, sizeof(vehicleToAdd.color), "Color cannot be empty");

    getValidatedCharInput("Enter type (P or C): ", &vehicleToAdd.type, "Type must be P or C", "PC");

    getValidatedInput("Enter value: ", &vehicleToAdd.value, "Value must be a positive number", 0);

    vehicleToAdd.state = 'A';

    return vehicleToAdd;
}



int writeVehicles(const char *filename, vehicle *vehiclesToAdd, int total) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    if (fwrite(&total, sizeof(int), 1, file) != 1) {
        printf("Error writing number of vehicles to file\n");
        fclose(file);
        return 1;
    }

    if (fwrite(vehiclesToAdd, sizeof(vehicle), total, file) != total) {
        printf("Error writing vehicles to file\n");
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

    if (fread(total, sizeof(int), 1, file) != 1) {
        printf("Error reading number of vehicles from file\n");
        fclose(file);
        return NULL;
    }

    vehicle *vehicles = (vehicle *)malloc(sizeof(vehicle) * (*total));
    if (vehicles == NULL) {
        printf("Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    if (fread(vehicles, sizeof(vehicle), *total, file) != *total) {
        printf("Error reading vehicles from file\n");
        free(vehicles);
        fclose(file);
        return NULL;
    }

    fclose(file);
    return vehicles;
}

int findDeletedVehicle(vehicle *vehicles, int total) {
    for (int i = 0; i < total; i++) {
        if (vehicles[i].state == 'E') {
            return i;
        }
    }
    return -1;
}

void addVehicle(const char *filename) {
    vehicle vehicleToAdd = createVehicle();

    vehicle *vehicles;
    int total;

    vehicles = readVehicles(filename, &total);

    if (vehicles == NULL) {
        vehicles = (vehicle *)malloc(sizeof(vehicle));
        if (vehicles == NULL) {
            printf("Memory allocation failed\n");
            return;
        }
        total = 1;
    } else {
        total++;
        int deletedIndex = findDeletedVehicle(vehicles, total);
        if (deletedIndex != -1) {
            vehicles[deletedIndex] = vehicleToAdd;
            if (writeVehicles(filename, vehicles, total) != 0) {
                printf("Error writing to file\n");
            }
            free(vehicles);
            return;
        }
        else {
            vehicles = (vehicle *)realloc(vehicles, sizeof(vehicle) * total);
            if (vehicles == NULL) {
                printf("Memory allocation failed\n");
                return;
            }
        }
    }

    vehicles[total - 1] = vehicleToAdd;

    if (writeVehicles(filename, vehicles, total) != 0) {
        printf("Error writing to file\n");
    }

    free(vehicles);
}

void printVehicles (const char *filename) {
  vehicle *vehicles;
  int total = 0;

  vehicles = readVehicles(filename, &total);

  for(int i = 0; i < total; i++) {
    if (vehicles[i].state  == 'E') {
      continue;
    }
    printf("Vehicle %d\n", i + 1);
    printf("Plate: %s\n", vehicles[i].plate);
    printf("Brand: %s\n", vehicles[i].brand);
    printf("Model: %s\n", vehicles[i].model);
    printf("Year: %i\n", vehicles[i].year);
    printf("Color: %s\n", vehicles[i].color);
    printf("Type: %c\n", vehicles[i].type);
    printf("Value: %d\n", vehicles[i].value);
    printf("State: %c\n", vehicles[i].state);
    printf("\n");
  }
}

char *toLower(const char *str) {
    char *lower = (char *)malloc(strlen(str) + 1);
    if (lower == NULL) {
        return NULL;
    }

    for (int i = 0; str[i]; i++) {
        lower[i] = tolower(str[i]);
    }
    lower[strlen(str)] = '\0';
    return lower;
}

void printVehiclesByModel (const char *filename, const char *model) {
  vehicle *vehicles;
  int total = 0;

  vehicles = readVehicles(filename, &total);
  size_t len = strlen(model);
  char *lowerModel = toLower(model);

  for(int i = 0; i < total; i++) {
    if (vehicles[i].state  == 'E') {
      continue;
    }
    char *lower = toLower(vehicles[i].model);
    if (strncmp(lower, lowerModel, len) == 0) {
      printf("Vehicle %d\n", i + 1);
      printf("Plate: %s\n", vehicles[i].plate);
      printf("Brand: %s\n", vehicles[i].brand);
      printf("Model: %s\n", vehicles[i].model);
      printf("Year: %i\n", vehicles[i].year);
      printf("Color: %s\n", vehicles[i].color);
      printf("Type: %c\n", vehicles[i].type);
      printf("Value: %d\n", vehicles[i].value);
      printf("State: %c\n", vehicles[i].state);
      printf("\n");
    }
    free(lower);
  }
  free(lowerModel);
}

void printVehiclesByBrand (const char *filename, const char *brand) {
  vehicle *vehicles;
  int total = 0;

  vehicles = readVehicles(filename, &total);
  size_t len = strlen(brand);
  char *lowerBrand = toLower(brand);

  for(int i = 0; i < total; i++) {
    if (vehicles[i].state  == 'E') {
      continue;
    }
    char *lower = toLower(vehicles[i].brand);
    if (strncmp(lower, lowerBrand, len) == 0) {
      printf("Vehicle %d\n", i + 1);
      printf("Plate: %s\n", vehicles[i].plate);
      printf("Brand: %s\n", vehicles[i].brand);
      printf("Model: %s\n", vehicles[i].model);
      printf("Year: %i\n", vehicles[i].year);
      printf("Color: %s\n", vehicles[i].color);
      printf("Type: %c\n", vehicles[i].type);
      printf("Value: %d\n", vehicles[i].value);
      printf("State: %c\n", vehicles[i].state);
      printf("\n");
    }
    free(lower);
  }
  free(lowerBrand);
}

void printVehiclesByType (const char *filename, const char type) {
  vehicle *vehicles;
  int total = 0;

  vehicles = readVehicles(filename, &total);

  for(int i = 0; i < total; i++) {
    if (vehicles[i].state  == 'E') {
      continue;
    }
    if (vehicles[i].type == type) {
      printf("Vehicle %d\n", i + 1);
      printf("Plate: %s\n", vehicles[i].plate);
      printf("Brand: %s\n", vehicles[i].brand);
      printf("Model: %s\n", vehicles[i].model);
      printf("Year: %i\n", vehicles[i].year);
      printf("Color: %s\n", vehicles[i].color);
      printf("Type: %c\n", vehicles[i].type);
      printf("Value: %d\n", vehicles[i].value);
      printf("State: %c\n", vehicles[i].state);
      printf("\n");
    }
  }
}

void printVehiclesByPriceRange (const char *filename, int min, int max) {
  if(min > max) {
    printf("Invalid price range\n");
    return;
  }
  vehicle *vehicles;
  int total = 0;

  vehicles = readVehicles(filename, &total);

  for(int i = 0; i < total; i++) {
    if (vehicles[i].state  == 'E') {
      continue;
    }
    if (vehicles[i].value >= min && vehicles[i].value <= max) {
      printf("Vehicle %d\n", i + 1);
      printf("Plate: %s\n", vehicles[i].plate);
      printf("Brand: %s\n", vehicles[i].brand);
      printf("Model: %s\n", vehicles[i].model);
      printf("Year: %i\n", vehicles[i].year);
      printf("Color: %s\n", vehicles[i].color);
      printf("Type: %c\n", vehicles[i].type);
      printf("Value: %d\n", vehicles[i].value);
      printf("State: %c\n", vehicles[i].state);
      printf("\n");
    }
  }
  free(vehicles);
}

void modifyVehicleValue (const char *filename, const char *plate, int newValue) {
  vehicle *vehicles;
  int total = 0;

  vehicles = readVehicles(filename, &total);

  for(int i = 0; i < total; i++) {
    if (vehicles[i].state  == 'E') {
      continue;
    }
    if (strcmp(vehicles[i].plate, plate) == 0) {
      vehicles[i].value = newValue;
      if (writeVehicles(filename, vehicles, total) != 0) {
          printf("Error writing to file\n");
      } else {
        printf("Vehicle value updated\n");
      }
      return;
    }
  }
  free(vehicles);
  printf("Vehicle not found\n");
}

void modifyVehicleType (const char *filename, const char *plate, char newType) {
  vehicle *vehicles;
  int total = 0;

  vehicles = readVehicles(filename, &total);

  for(int i = 0; i < total; i++) {
    if (vehicles[i].state  == 'E') {
      continue;
    }
    if (strcmp(vehicles[i].plate, plate) == 0) {
      vehicles[i].type = newType;
      if (writeVehicles(filename, vehicles, total) != 0) {
          printf("Error writing to file\n");
      } else {
          printf("Vehicle type updated\n");
      }
      return;
    }
  }
  free(vehicles);
  printf("Vehicle not found\n");
}

void deleteVehicle (const char *filename, const char *plate) {
  vehicle *vehicles;
  int total = 0;

  vehicles = readVehicles(filename, &total);

  for(int i = 0; i < total; i++) {
    if (vehicles[i].state  == 'E') {
      continue;
    }
    if (strcmp(vehicles[i].plate, plate) == 0) {
      vehicles[i].state = 'E';
      if (writeVehicles(filename, vehicles, total) != 0) {
          printf("Error writing to file\n");
      } else {
        printf("Vehicle deleted\n");
      }
      return;
    }
  }
  free(vehicles);
  printf("Vehicle not found\n");
}


int getOption() {
    char optionChar[5];
    int option;
    while (1) {
        printf("Enter an option: ");
        if (fgets(optionChar, sizeof(optionChar), stdin) != NULL) {
            size_t len = strlen(optionChar);
            if (len > 0 && optionChar[len - 1] == '\n') {
                optionChar[len - 1] = '\0';
            }
            if (sscanf(optionChar, "%d", &option) == 1) {
                if (option >= 1 && option <= 10) {
                    return option;
                } else {
                    printf("Invalid option. Please choose a number between 1 and 10\n");
                }
            } else {
                printf("Option must be a number.\n");
            }
        } else {
            printf("Error reading input. Please try again.\n");
        }
    }
}

int main() {
    int option;
    while (1) {
        printf("\n");
        printf("Welcome to the vehicle registration system\n");
        printf("\n");
        printf("1. Add vehicle\n");
        printf("2. List vehicles\n");
        printf("3. Print vehicles by brand\n");
        printf("4. Print vehicles by model\n");
        printf("5. Print vehicles by type\n");
        printf("6. Print vehicles by price range\n");
        printf("7. Modify vehicle value\n");
        printf("8. Modify vehicle type\n");
        printf("9. Delete vehicle\n");
        printf("10. Exit\n");
        printf("\n");
        option = getOption();

        switch (option) {
            case 1:
                addVehicle("vehicles.dat");
                break;
            case 2:
                printVehicles("vehicles.dat");
                break;
            case 3:
                {
                    char brand[20];
                    getStringInput("Enter brand: ", brand, sizeof(brand), "Brand cannot be empty");
                    printVehiclesByBrand("vehicles.dat", brand);
                }
                break;
            case 4:
                {
                    char model[20];
                    getStringInput("Enter model: ", model, sizeof(model), "Model cannot be empty");
                    printVehiclesByModel("vehicles.dat", model);
                }
                break;
            case 5:
                {
                    char type;
                    getValidatedCharInput("Enter type (P or C): ", &type, "Type must be P or C", "PC");
                    printVehiclesByType("vehicles.dat", type);
                }
                break;
            case 6:
                {
                    int min, max;
                    getValidatedInput("Enter min price: ", &min, "Price must be a positive number", 0);
                    getValidatedInput("Enter max price: ", &max, "Price must be a positive number", 0);
                    printVehiclesByPriceRange("vehicles.dat", min, max);
                }
                break;
            case 7:
                {
                    char plate[10];
                    int newValue;
                    getStringInput("Enter plate: ", plate, sizeof(plate), "Plate cannot be empty");
                    getValidatedInput("Enter new value: ", &newValue, "Value must be a positive number", 0);
                    modifyVehicleValue("vehicles.dat", plate, newValue);
                }
                break;
            case 8:
                {
                    char plate[10];
                    char newType;
                    getStringInput("Enter plate: ", plate, sizeof(plate), "Plate cannot be empty");
                    getValidatedCharInput("Enter new type (P or C): ", &newType, "Type must be P or C", "PC");
                    modifyVehicleType("vehicles.dat", plate, newType);
                }
                break;
            case 9:
                {
                    char plate[10];
                    getStringInput("Enter plate: ", plate, sizeof(plate), "Plate cannot be empty");
                    deleteVehicle("vehicles.dat", plate);
                }
                break;
            default:
                {
                  printf("Goodbye\n");
                  return 0;
                }
                break;
        }
    }

    return 0;
}
