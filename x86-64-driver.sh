#!/usr/bin/env bash

# Constants
readonly LIB="functions"
readonly LIB_DIR="./lib"
readonly EXTENSION=".calc"
readonly PROLOGUE_FILE="prologue.s"
readonly EPILOGUE_FILE="epilogue.s"

# Function to handle errors
error() {
  echo "Error: $1"
  exit 1
}

# Function to display usage
usage() {
  echo "Usage: $0 <testprogs/filename.calc>"
  exit 1
}

# Function to validate input file
validate_input() {
  local input_file="$1"

  # Check if input file has the correct extension
  [[ ! "$input_file" =~ .*"$EXTENSION" ]] && error "Invalid file, extension must be '$EXTENSION'"

  # Check if input file is readable
  [[ ! -r "$input_file" ]] && error "Input file '$input_file' not found or not readable"
}

# Function to generate assembly file
generate_assembly() {
  local input_file="$1"
  local base_name="$2"
  local assembly_file="$3"

  # Write prologue to assembly file
  cat "$PROLOGUE_FILE" > "$assembly_file"

  # Append input file to assembly file
  ./bin/calc3i < "$input_file" >> "$assembly_file"

  # Append epilogue to assembly file
  cat "$EPILOGUE_FILE" >> "$assembly_file"
}

# Function to compile assembly file
compile_assembly() {
  local assembly_file="$1"
  local base_name="$2"

  # Compile assembly file
  gcc -no-pie "$assembly_file" -L "$LIB_DIR" -l "$LIB" -o "$base_name" -z noexecstack
}

# Check if there is exactly one argument
[[ "$#" -ne 1 ]] && usage

# Get input file
input_file="$1"

# Validate input file
validate_input "$input_file"

# Get base name of file
base_name=$(basename "$input_file" "$EXTENSION")

# Set assembly file name
assembly_file="$base_name.s"

# Generate assembly file
generate_assembly "$input_file" "$base_name" "$assembly_file"

# Notify user of assembly file location
echo "Assembly code written to file: '$assembly_file'"

# Compile assembly file
compile_assembly "$assembly_file" "$base_name"
