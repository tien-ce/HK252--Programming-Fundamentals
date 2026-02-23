# Water 7 Project - Test Environment

This repository contains the logic for the Water 7 arc simulation and automated testing scripts.

---

### 1. Generate Input Files

Use default input files or create new and add it into run.sh script or run `./main "path to your input file"` after using run.sh script.

### 2. Run Tests

Use the run.sh script to compile and execute all test cases.

- Normal Mode:

```
./run.sh
```

- Debug Mode (Enables #ifdef DEBUG logs):

```
./run.sh -d
```

If you use the script with debug mode, the main program will run with your own file by command `./main "path to your input file"` in debug mode coresspondingly.

### 3. Check the log

If you build and run program with run.sh scripts, check your result in result.log file.
