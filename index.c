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

void addVehicle (const char *filename) {
  struct vehicle vehicleToAdd = createVehicle();

  FILE *file;

  if ((file = fopen(filename, "ab+")) == NULL) {
    printf("Error opening file\n");
    return;
  }

  int total = 0;

  if (fread(&total, sizeof(int), 1, file) != 1){
    total++;

    if(fwrite(&total, sizeof(int), 1, file) != 1){
      printf("Coudln't write on file\n");
      fclose(file);
      return;
    }

    if(fwrite(&vehicleToAdd, sizeof(struct vehicle), 1, file) != 1){
      printf("Couldn't write on file\n");
      fclose(file);
      return;
    }

    printf("Vehicle added succesfully\n");
    fclose(file);
    return;
  }

  total++;

  if(fwrite(&total, sizeof(int), 1, file) != 1){
    printf("Coudln't write on file\n");
    fclose(file);
    return;
  }

  if(fwrite(&vehicleToAdd, sizeof(struct vehicle), 1, file) != 1){
    printf("Couldn't write on file\n");
    fclose(file);
    return;
  }

  fclose(file);
  printf("Vehicle added succesfully\n");
}

void printVehicles (const char *filename) {
  FILE *file;
  if ((file = fopen(filename, "rb")) == NULL) {
    printf("Error opening file\n");
    fclose(file);
    return;
  }
  int total = 0;

  if (fread(&total, sizeof(int), 1, file) != 1) {
    printf("Error reading from file\n");
    fclose(file);
  }
  struct vehicle currentVehicle;
  for(int i = 0; i < total; i++) {
    if (fread(&currentVehicle, sizeof(struct vehicle), 1, file) != 1) {
      printf("Error reading from file");
      fclose(file);
      return;
    }
    if (currentVehicle.state == 'E') {
      continue;
    }
    printf("Vehicle %d\n", i + 1);
    printf("Plate: %s\n", currentVehicle.plate);
    printf("Brand: %s\n", currentVehicle.brand);
    printf("Model: %s\n", currentVehicle.model);
    printf("Year: %i\n", currentVehicle.year);
    printf("Color: %s\n", currentVehicle.color);
    printf("Type: %c\n", currentVehicle.type);
    printf("Value: %d\n", currentVehicle.value);
    printf("State: %c\n", currentVehicle.state);
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
  int total = 0;
  FILE *file;


  if ((file = fopen(filename, "rb")) == NULL) {
    printf("Error opening file\n");
    fclose(file);
    return;
  }

  size_t len = strlen(model);
  char *lowerModel = toLower(model);

  if(fread(&total, sizeof(int), 1, file) != 1) printf("Error reading from file \n");

  struct vehicle currentVehicle;

  for(int i =0; i < total; i++){
    if(fread(&currentVehicle, sizeof(struct vehicle), 1, file) != 1) {
      printf("Error reading from file\n");
      fclose(file);
      return;
    }

    if (currentVehicle.state  == 'E') {
      continue;
    }
    char *lower = toLower(currentVehicle.model);
    if (strncmp(lower, lowerModel, len) == 0) {
      printf("Vehicle %d\n", i + 1);
      printf("Plate: %s\n", currentVehicle.plate);
      printf("Brand: %s\n", currentVehicle.brand);
      printf("Model: %s\n", currentVehicle.model);
      printf("Year: %i\n", currentVehicle.year);
      printf("Color: %s\n", currentVehicle.color);
      printf("Type: %c\n", currentVehicle.type);
      printf("Value: %d\n", currentVehicle.value);
      printf("State: %c\n", currentVehicle.state);
      printf("\n");

  }
  }
  fclose(file);
}

void printVehiclesByBrand(const char *filename, const char *brand) {
    FILE *file;
    if ((file = fopen(filename, "rb")) == NULL) {
        printf("Error opening file\n");
        return;
    }

    int total = 0;
    if (fread(&total, sizeof(int), 1, file) != 1) {
        printf("Error reading from file\n");
        return;
    }

    struct vehicle currentVehicle;
    size_t len = strlen(brand);
    char *lowerBrand = toLower(brand);

    for (int i = 0; i < total; i++) {
        if (fread(&currentVehicle, sizeof(struct vehicle), 1, file) != 1) {
            printf("Error reading from file\n");
            fclose(file);
            return;
        }

        if (currentVehicle.state == 'E') {
            continue;
        }

        char *lower = toLower(currentVehicle.brand);
        if (strncmp(lower, lowerBrand, len) == 0) {
            printf("Vehicle %d\n", i + 1);
            printf("Plate: %s\n", currentVehicle.plate);
            printf("Brand: %s\n", currentVehicle.brand);
            printf("Model: %s\n", currentVehicle.model);
            printf("Year: %i\n", currentVehicle.year);
            printf("Color: %s\n", currentVehicle.color);
            printf("Type: %c\n", currentVehicle.type);
            printf("Value: %d\n", currentVehicle.value);
            printf("State: %c\n", currentVehicle.state);
            printf("\n");
        }
        free(lower);
    }
    free(lowerBrand);
    fclose(file);
}

void printVehiclesByType (const char *filename, const char type) {
  int total = 0;

  FILE *file;

  if ((file = fopen(filename, "rb")) == NULL) {
    printf("Error opening file\n");
    fclose(file);
    return;
  }

  if ((fread(&total, sizeof(int), 1, file)) != 1) {
    printf("Error reading from file\n");
    fclose(file);
    return;
  }

  struct vehicle currentVehicle;

  for (int i = 0; i < total; i++) {
    if ((fread(&currentVehicle, sizeof(struct vehicle), 1, file)) != 1) {
      printf("Error reading from file\n");
      fclose(file);
      return;
    }

    if (currentVehicle.state == 'E') {
      continue;
    }

    if (currentVehicle.type == type) {
      printf("Vehicle %d\n", i + 1);
      printf("Plate: %s\n", currentVehicle.plate);
      printf("Brand: %s\n", currentVehicle.brand);
      printf("Model: %s\n", currentVehicle.model);
      printf("Year: %i\n", currentVehicle.year);
      printf("Color: %s\n", currentVehicle.color);
      printf("Type: %c\n", currentVehicle.type);
      printf("Value: %d\n", currentVehicle.value);
      printf("State: %c\n", currentVehicle.state);
      printf("\n");
    }
  }
  fclose(file);
}

void printVehiclesByPriceRange (const char *filename, int min, int max) {
    if(min > max) {
        printf("Invalid price range\n");
        return;
    }
    FILE *file;
    if ((file = fopen(filename, "rb")) == NULL) {
        printf("Error opening file\n");
        fclose(file);
        return;
    }

    int total = 0;
    if (fread(&total, sizeof(int), 1, file) != 1) {
        printf("Error reading from file\n");
        fclose(file);
        return;
    }

    struct vehicle currentVehicle;

    for (int i = 0; i < total; i++) {
        if (fread(&currentVehicle, sizeof(struct vehicle), 1, file) != 1) {
            printf("Error reading from file\n");
            fclose(file);
            return;
        }

        if (currentVehicle.state == 'E') {
            continue;
        }

        if (currentVehicle.value >= min && currentVehicle.value <= max) {
            printf("Vehicle %d\n", i + 1);
            printf("Plate: %s\n", currentVehicle.plate);
            printf("Brand: %s\n", currentVehicle.brand);
            printf("Model: %s\n", currentVehicle.model);
            printf("Year: %i\n", currentVehicle.year);
            printf("Color: %s\n", currentVehicle.color);
            printf("Type: %c\n", currentVehicle.type);
            printf("Value: %d\n", currentVehicle.value);
            printf("State: %c\n", currentVehicle.state);
            printf("\n");
        }
    }
    fclose(file);
  }

void modifyVehicleValue (const char *filename, const char *plate, int newValue) {
    FILE *file;
    if ((file = fopen(filename, "rb+")) == NULL) {
        printf("Error opening file\n");
        fclose(file);
        return;
    }

    int total = 0;

    if (fread(&total, sizeof(int), 1, file) != 1) {
        printf("Error reading from file\n");
        fclose(file);
        return;
    }

    struct vehicle currentVehicle;

    for (int i = 0; i < total; i++) {
        if (fread(&currentVehicle, sizeof(struct vehicle), 1, file) != 1) {
            printf("Error reading from file\n");
            break;
        }

        if (currentVehicle.state == 'E') {
            continue;
        }

        if (strcmp(currentVehicle.plate, plate) == 0) {
            currentVehicle.value = newValue;

            fseek(file, sizeof(int) + i * sizeof(struct vehicle), SEEK_SET);

            if (fwrite(&currentVehicle, sizeof(struct vehicle), 1, file) != 1) {
                printf("Error writing to file\n");
            } else {
                printf("Vehicle value updated\n");
            }

            fclose(file);
            return;
        }
    }
  }

void modifyVehicleType (const char *filename, const char *plate, char newType) {
  int total = 0;
  FILE *file;

  if ((file = fopen(filename, "rb+")) == NULL) {
    printf("Error opening file\n");
    fclose(file);
    return;
  }

  if (fread(&total, sizeof(int), 1, file) != 1) {
    printf("Error reading from file\n");
    fclose(file);
    return;
  }

  struct vehicle currentVehicle;

  for (int i = 0; i < total; i++){
    if (fread(&currentVehicle, sizeof(struct vehicle), 1, file) != 1) {
      printf("Error reading from file\n");
      fclose(file);
      return;
    }
    if (currentVehicle.state == 'E') {
      continue;
    }
    if (strcmp(currentVehicle.plate, plate) == 0) {
      currentVehicle.type = newType;

      fseek(file, sizeof(int) + i * sizeof(struct vehicle), SEEK_SET);

      if (fwrite(&currentVehicle, sizeof(struct vehicle), 1, file) != 1) {
        printf("Error writing to file\n");
      } else {
        printf("Vehicle type updated\n");
      }

      fclose(file);
      return;
    }
  }
}

void deleteVehicle (const char *filename, const char *plate) {
  FILE *file;
  if ((file = fopen(filename, "rb+")) == NULL) {
    printf("Error opening file\n");
    fclose(file);
    return;
  }

  int total = 0;

  if (fread(&total, sizeof(int), 1, file) != 1) {
    printf("Error reading from file\n");
    fclose(file);
    return;
  }

  struct vehicle currentVehicle;

  for (int i = 0; i < total; i++) {
    if (fread(&currentVehicle, sizeof(struct vehicle), 1, file) != 1) {
      printf("Error reading from file\n");
      break;
    }

    if (currentVehicle.state == 'E') {
      continue;
    }

    if (strcmp(currentVehicle.plate, plate) == 0) {
      currentVehicle.state = 'E';

      fseek(file, sizeof(int) + i * sizeof(struct vehicle), SEEK_SET);

      if (fwrite(&currentVehicle, sizeof(struct vehicle), 1, file) != 1) {
        printf("Error writing to file\n");
      } else {
        printf("Vehicle deleted\n");
      }

      fclose(file);
      return;
    }
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
