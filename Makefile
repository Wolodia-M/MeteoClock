# Build and upload to board
upload:
	pio run -t upload

# Only build
build:
	pio run

# Clean
clean:
	pio run -t clean

# Export compile_commands.json
ccj:
	pio run -t compiledb
