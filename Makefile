all: deps build

deps:
	# esp32 depends on Python + pyserial :(
	python3 -m venv venv
	./venv/bin/pip install --upgrade pip
	./venv/bin/pip install pyserial

	# Update package index
	arduino-cli core update-index
	# Install feather esp32 core
	arduino-cli core install esp32:esp32
	# Other library dependencies
	arduino-cli lib install Button2@1.6.1
	arduino-cli lib install Ramp@0.5.0
	arduino-cli lib install ESP32Servo@0.9.0

build:
	# TODO: This is hard coded to build for featheresp32
	# Might want to parameterize this in the future if additional
	# boards are supported.
	PATH="$$PWD/venv/bin:$$PATH" arduino-cli compile -b esp32:esp32:featheresp32 .
