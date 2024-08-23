#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VEHICLES 50

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

struct vehicle createVehicle() {
    struct vehicle vehicleToAdd = {0};

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

void addVehicle (struct vehicle *vehicles) {
  struct vehicle vehicleToAdd = createVehicle();

  struct vehicle *vehiclePtr = vehicles;

  int vehicleNumber = 0;
  while(1){
    if(strcmp((*vehiclePtr).plate, "")  == 0 || (*vehiclePtr).state == 'E'){
      *vehiclePtr = vehicleToAdd;
      break;
    }
    vehicleNumber++;
    if(vehicleNumber > MAX_VEHICLES) {
      printf("Max vehicles excedeed\n");
      break;
    };
    vehiclePtr += vehicleNumber;
  }
}

void printVehicles (struct vehicle *vehicles) {
  struct vehicle *vehiclePtr = vehicles;

  int vehicleNumber = 0;

  while(1) {
    if(strcmp((*vehiclePtr).plate, "") == 0 || vehicleNumber > MAX_VEHICLES) break;
    if ((*vehiclePtr).state == 'E') {
      continue;
    }
    printf("Vehicle %d\n", vehicleNumber + 1);
    printf("Plate: %s\n", (*vehiclePtr).plate);
    printf("Brand: %s\n", (*vehiclePtr).brand);
    printf("Model: %s\n", (*vehiclePtr).model);
    printf("Year: %i\n", (*vehiclePtr).year);
    printf("Color: %s\n", (*vehiclePtr).color);
    printf("Type: %c\n", (*vehiclePtr).type);
    printf("Value: %d\n", (*vehiclePtr).value);
    printf("State: %c\n", (*vehiclePtr).state);
    printf("\n");
    vehicleNumber++;
    if(vehicleNumber > MAX_VEHICLES) break;
    vehiclePtr += vehicleNumber;
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

void printVehiclesByModel (struct vehicle *vehicles, const char *model) {
  size_t len = strlen(model);
  char *lowerModel = toLower(model);

  struct vehicle *vehiclePtr = vehicles;

  int vehicleNumber = 0;

  while(1){
    if(strcmp((*vehiclePtr).plate, "") == 0 || vehicleNumber > MAX_VEHICLES) break;
    if ((*vehiclePtr).state  == 'E') {
      continue;
    }
    char *lower = toLower((*vehiclePtr).model);
    if (strncmp(lower, lowerModel, len) == 0) {
      printf("Vehicle %d\n", vehicleNumber + 1);
      printf("Plate: %s\n", (*vehiclePtr).plate);
      printf("Brand: %s\n", (*vehiclePtr).brand);
      printf("Model: %s\n", (*vehiclePtr).model);
      printf("Year: %i\n", (*vehiclePtr).year);
      printf("Color: %s\n", (*vehiclePtr).color);
      printf("Type: %c\n", (*vehiclePtr).type);
      printf("Value: %d\n", (*vehiclePtr).value);
      printf("State: %c\n", (*vehiclePtr).state);
      printf("\n");
    }
    vehicleNumber++;
    vehiclePtr += vehicleNumber;
  }
}

void printVehiclesByBrand(struct vehicle *vehicles, const char *brand) {
    struct vehicle *vehiclePtr = vehicles;
    int vehicleNumber = 0;

    size_t len = strlen(brand);
    char *lowerBrand = toLower(brand);

    while (1) {
        if(strcmp((*vehiclePtr).plate, "") == 0 || vehicleNumber > MAX_VEHICLES) break;
        if ((*vehiclePtr).state == 'E') {
            continue;
        }

        char *lower = toLower((*vehiclePtr).brand);
        if (strncmp(lower, lowerBrand, len) == 0) {
            printf("Vehicle %d\n", vehicleNumber + 1);
            printf("Plate: %s\n", (*vehiclePtr).plate);
            printf("Brand: %s\n", (*vehiclePtr).brand);
            printf("Model: %s\n", (*vehiclePtr).model);
            printf("Year: %i\n", (*vehiclePtr).year);
            printf("Color: %s\n", (*vehiclePtr).color);
            printf("Type: %c\n", (*vehiclePtr).type);
            printf("Value: %d\n", (*vehiclePtr).value);
            printf("State: %c\n", (*vehiclePtr).state);
            printf("\n");
        }
        vehicleNumber++;
        vehiclePtr += vehicleNumber;
        free(lower);
    }
    free(lowerBrand);
}

void printVehiclesByType (struct vehicle *vehicles, char type) {
    struct vehicle *vehiclePtr = vehicles;
    int vehicleNumber = 0;

    while (1) {
        if(strcmp((*vehiclePtr).plate, "") == 0 || vehicleNumber > MAX_VEHICLES) break;
        if ((*vehiclePtr).state == 'E') {
            continue;
        }

        if ((*vehiclePtr).type == type) {
            printf("Vehicle %d\n", vehicleNumber + 1);
            printf("Plate: %s\n", (*vehiclePtr).plate);
            printf("Brand: %s\n", (*vehiclePtr).brand);
            printf("Model: %s\n", (*vehiclePtr).model);
            printf("Year: %i\n", (*vehiclePtr).year);
            printf("Color: %s\n", (*vehiclePtr).color);
            printf("Type: %c\n", (*vehiclePtr).type);
            printf("Value: %d\n", (*vehiclePtr).value);
            printf("State: %c\n", (*vehiclePtr).state);
            printf("\n");
        }
        vehicleNumber++;
        vehiclePtr += vehicleNumber;
    }
}

void printVehiclesByPriceRange (struct vehicle *vehicles, int min, int max) {
    struct vehicle *vehiclePtr = vehicles;
    int vehicleNumber = 0;

    if (min > max) {
        printf("Invalid price range\n");
        return;
    }

    while (1) {
        if(strcmp((*vehiclePtr).plate, "") == 0 || vehicleNumber > MAX_VEHICLES) break;
        if ((*vehiclePtr).state == 'E') {
            continue;
        }

        if ((*vehiclePtr).value >= min && (*vehiclePtr).value <= max) {
            printf("Vehicle %d\n", vehicleNumber + 1);
            printf("Plate: %s\n", (*vehiclePtr).plate);
            printf("Brand: %s\n", (*vehiclePtr).brand);
            printf("Model: %s\n", (*vehiclePtr).model);
            printf("Year: %i\n", (*vehiclePtr).year);
            printf("Color: %s\n", (*vehiclePtr).color);
            printf("Type: %c\n", (*vehiclePtr).type);
            printf("Value: %d\n", (*vehiclePtr).value);
            printf("State: %c\n", (*vehiclePtr).state);
            printf("\n");
        }
        vehicleNumber++;
        vehiclePtr += vehicleNumber;
    }
}

void modifyVehicleValue (struct vehicle *vehicles, const char *plate, int newValue) {
    struct vehicle *vehiclePtr = vehicles;
    int vehicleNumber = 0;

    while (1) {
        if(strcmp((*vehiclePtr).plate, "") == 0 || vehicleNumber > MAX_VEHICLES) break;
        if ((*vehiclePtr).state == 'E') {
            continue;
        }

        if (strcmp((*vehiclePtr).plate, plate) == 0) {
            (*vehiclePtr).value = newValue;
            printf("Vehicle value updated\n");
            return;
        }
        vehicleNumber++;
        vehiclePtr += vehicleNumber;
    }
}

void modifyVehicleType (struct vehicle *vehicles, const char *plate, char newType) {
    struct vehicle *vehiclePtr = vehicles;
    int vehicleNumber = 0;

    while (1) {
        if(strcmp((*vehiclePtr).plate, "") == 0 || vehicleNumber > MAX_VEHICLES) break;
        if ((*vehiclePtr).state == 'E') {
            continue;
        }

        if (strcmp((*vehiclePtr).plate, plate) == 0) {
            (*vehiclePtr).type = newType;
            printf("Vehicle type updated\n");
            return;
        }
        vehicleNumber++;
        vehiclePtr += vehicleNumber;
    }
}

void deleteVehicle (struct vehicle *vehicles, const char *plate) {
    struct vehicle *vehiclePtr = vehicles;
    int vehicleNumber = 0;

    while (1) {
        if(strcmp((*vehiclePtr).plate, "") == 0 || vehicleNumber > MAX_VEHICLES) break;
        if (strcmp((*vehiclePtr).plate, plate) == 0) {
            (*vehiclePtr).state = 'E';
            printf("Vehicle deleted\n");
            return;
        }
        vehicleNumber++;
        vehiclePtr += vehicleNumber;
    }
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
    struct vehicle vehicles[50] = {0};
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
                addVehicle(vehicles);
                break;
            case 2:
                printVehicles(vehicles);
                break;
            case 3:
                {
                    char brand[20];
                    getStringInput("Enter brand: ", brand, sizeof(brand), "Brand cannot be empty");
                    printVehiclesByBrand(vehicles, brand);
                }
                break;
            case 4:
                {
                    char model[20];
                    getStringInput("Enter model: ", model, sizeof(model), "Model cannot be empty");
                    printVehiclesByModel(vehicles, model);
                }
                break;
            case 5:
                {
                    char type;
                    getValidatedCharInput("Enter type (P or C): ", &type, "Type must be P or C", "PC");
                    printVehiclesByType(vehicles, type);
                }
                break;
            case 6:
                {
                    int min, max;
                    getValidatedInput("Enter min price: ", &min, "Price must be a positive number", 0);
                    getValidatedInput("Enter max price: ", &max, "Price must be a positive number", 0);
                    printVehiclesByPriceRange(vehicles, min, max);
                }
                break;
            case 7:
                {
                    char plate[10];
                    int newValue;
                    getStringInput("Enter plate: ", plate, sizeof(plate), "Plate cannot be empty");
                    getValidatedInput("Enter new value: ", &newValue, "Value must be a positive number", 0);
                    modifyVehicleValue(vehicles, plate, newValue);
                }
                break;
            case 8:
                {
                    char plate[10];
                    char newType;
                    getStringInput("Enter plate: ", plate, sizeof(plate), "Plate cannot be empty");
                    getValidatedCharInput("Enter new type (P or C): ", &newType, "Type must be P or C", "PC");
                    modifyVehicleType(vehicles, plate, newType);
                }
                break;
            case 9:
                {
                    char plate[10];
                    getStringInput("Enter plate: ", plate, sizeof(plate), "Plate cannot be empty");
                    deleteVehicle(vehicles, plate);
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
