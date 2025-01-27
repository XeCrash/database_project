# *Employee Database - Alpha v0.5* 

[![GCC Build](https://github.com/XeCrash/database_project/actions/workflows/c-build.yml/badge.svg?branch=master)](https://github.com/XeCrash/database_project/actions/workflows/c-build.yml)

## About

* Early build, I plan on updating this often, the database will store employees using a file on the disk which isn't added as of this version (0.5)

## Requirments

* Must have [GCC](https://gcc.gnu.org/) installed to compile  
  * For Linux: `sudo apt install gcc`  
  * For Windows: [Tutorial To Install gcc on Windows](https://phoenixnap.com/kb/install-gcc-windows)

## How to build the project from the terminal

### On Linux (from database_project directory)

* `gcc ./app.c -o bin/debug/database && ./bin/debug/database`

### On Windows (from database_project directory)

* `start build`  
or
* `start build-run`  
or with gcc commands
* `gcc -g app.c -o bin/debug/database`

## Flow Chart of Employee Database (WIP)

```mermaid
graph TD
    subgraph "main() Function"
        A[Start] --> B{Print Menu};
        B --> C{Get User Choice};
        C -- 0 --> J[Exit];
        C -- 1 --> D[Add Employee];
        C -- 2 --> E{Print all Employees};
        C -- 3 --> F[Search Employee];
        C -- 4 --> G[Delete Employee];
        C -- Default --> H[Invalid Choice];
        D --> DA{Get Employee Data via User Input};
        DA --> DB{Validate Input Data};
        DB -- Valid --> D1[Add to Database];
        D1 --> I[Clear Screen];
        DB -- Invalid --> D2[Print Invalid Input & Clear Char Buffer];
        D2 --> I[Clear Screen];
        E --> EA{Employee Count Validation};
        EA -- Validation: == 0 --> EN[Print No Employees];
        EN --> I;
        EA -- Validation: > 0 --> K[Display All Employees];
        F --> FS{Get Search ID};
        FS --> F1{Search in Database};
        F1 -- Found --> F2[Display Employee];
        F1 -- Not Found --> F3[Print Not Found];
        F2 --> I;
        F3 --> I;
        G --> GD{Get Delete ID};
        GD --> G1{Search in Database};
        G1 -- Found --> G2[Delete Employee];
        G2 --> I;
        G1 -- Not Found --> G3[Print Not Found];
        G3 --> I;
        H --> I;
        I --> B;
        K --> I;
        J[Exit] --> L[Free Memory];
        L --> M[End];
    end
```
